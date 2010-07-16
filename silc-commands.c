#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "silc-commands.h"
#include "silc-connections.h"

/* prototypes for imported functions */

int silc_plugin_query_input(void *data, struct t_gui_buffer *buffer, const char *input_data);

/* ===== completion callbacks ===== */

void silc_plugin_connected(SilcClient client, SilcClientConnection conn, SilcClientConnectionStatus status,
        SilcStatus error, const char *message, void *context) {
    SilcConnectionContext connContext;
    SilcPluginServerList server;

    server = find_server_for_buffer(context);

    if (status == SILC_CLIENT_CONN_DISCONNECTED) {
        weechat_log_printf("Disconnected: %s", message ? message : "");
        weechat_buffer_close(context);
        remove_server(server);
        return;
    }

    if (status != SILC_CLIENT_CONN_SUCCESS && status != SILC_CLIENT_CONN_SUCCESS_RESUME) {
        weechat_log_printf("Error connecting to server: %d", status);
        return;
    }
    connContext = malloc(sizeof(SilcConnectionContext));
    connContext->server_buffer = context;

    conn->context = connContext;

    server->connection = conn;

    weechat_log_printf("connection successfull");
}

/* ===== actions ===== */

int command_silc_connect(void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol) {
    char *servername;
    struct t_gui_buffer *server_buffer;
    SilcPluginServerList server;

    if (argc < 3) {
        weechat_printf(buffer, "you need to specify a servername to connect to");
        return WEECHAT_RC_ERROR;
    }

    servername = argv[2];

    // create a new buffer for this connection but merge it into the main window
    server_buffer = weechat_buffer_new(servername, NULL, NULL, NULL, NULL);
    weechat_printf(server_buffer, "SILC: trying to connect to %s", servername);
    weechat_buffer_merge(server_buffer, weechat_buffer_search_main());

    if (!silc_client_connect_to_server(silc_plugin->client, NULL, silc_plugin->public_key, silc_plugin->private_key,
            servername, 706, silc_plugin_connected, server_buffer)) {
        weechat_printf(server_buffer, "%sSILC: connection to server failed", weechat_prefix("error"));
        weechat_buffer_close(server_buffer);
    }

    server = add_server(servername, NULL, NULL, server_buffer);

    return WEECHAT_RC_OK;
}

int command_silc_disconnect(void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol) {
    SilcPluginServerList server;

    server = find_server_for_buffer(buffer);
    if (server == NULL) {
        weechat_printf(buffer, "%sCurrent buffer is not a SILC buffer", weechat_prefix("error"));
        return WEECHAT_RC_OK;
    }

    silc_client_command_call(silc_plugin->client, server->connection, "QUIT");
    return WEECHAT_RC_OK;
}

int command_silc_join(void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol) {
    SilcPluginServerList server;
    char *channelname;
    char *command;
    size_t command_length;

    if (argc < 3) {
        weechat_printf(buffer, "you need to specify a channel to join");
        return WEECHAT_RC_ERROR;
    }

    channelname = argv[2];
    command_length = strlen(channelname) + 6;
    command = malloc(command_length);
    memset(command, 0, command_length);
    snprintf(command, command_length, "JOIN %s", channelname);

    server = find_server_for_buffer(buffer);
    if (server == NULL) {
        weechat_printf(buffer, "%sCurrent buffer is not a SILC buffer", weechat_prefix("error"));
        return WEECHAT_RC_OK;
    }

    // Join here
    silc_client_command_call(silc_plugin->client, server->connection, command);
    return WEECHAT_RC_OK;
}

int command_silc_msg(void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol) {
    char *nickname;
    char *msg;
    struct t_gui_buffer *query_buffer;
    SilcPluginQueryList query;
    SilcClientEntry client_entry;
    SilcDList list;
    SilcPluginServerList server = find_server_for_buffer(buffer);

    if (argc < 3) {
        weechat_printf(buffer, "you need to specify a recipient");
        return WEECHAT_RC_ERROR;
    }

    nickname = argv[2];
    query = find_query_for_nick(server, nickname);
    if (query == NULL) {
        list = silc_client_get_clients_local(silc_plugin->client, server->connection, nickname, FALSE);
        if (list == NULL) {
            weechat_printf(buffer, "no such nick: %s", nickname);
            return WEECHAT_RC_OK;
        }
        silc_dlist_start(list);
        client_entry = silc_dlist_get(list);
        silc_client_list_free(silc_plugin->client, server->connection, list);
        query = add_query(server, client_entry, NULL);
        query_buffer = weechat_buffer_new(client_entry->nickname, &silc_plugin_query_input, query, NULL, NULL);
        query->query_buffer = query_buffer;
    } else {
        query_buffer = query->query_buffer;
    }

    if (argc > 3) {
        msg = argv_eol[3];
        silc_plugin_query_input(query, query_buffer, msg);
    }
    return WEECHAT_RC_OK;
}

/* ===== the /silc command, our main entry point ===== */

int command_silc(void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol) {
    char *action;

    if (argc < 2) {
        if (silc_plugin->running) {
            weechat_printf(buffer, "SILC version %s is running :)", SILC_PLUGIN_VERSION);
            list_connections(buffer);
            return WEECHAT_RC_OK;
        } else {
            weechat_printf(buffer, "SILC is not running :(");
            return WEECHAT_RC_ERROR;
        }
    }

    action = argv[1];

    if (strncmp(action, "connect", 7) == 0) {
        return command_silc_connect(data, buffer, argc, argv, argv_eol);
    }
    if (strncmp(action, "disconnect", 10) == 0) {
        return command_silc_disconnect(data, buffer, argc, argv, argv_eol);
    }
    if (strncmp(action, "join", 4) == 0) {
        return command_silc_join(data, buffer, argc, argv, argv_eol);
    }
    if (strncmp(action, "msg", 3) == 0) {
        return command_silc_msg(data, buffer, argc, argv, argv_eol);
    }

    weechat_printf(buffer, "unrecognized command %s", action);
    return WEECHAT_RC_ERROR;
}

