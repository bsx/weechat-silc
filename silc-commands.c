#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "silc-commands.h"

/* ===== callbacks for our commands ===== */

int command_silc(void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol) {
    if (silc_plugin->running) {
        weechat_printf(buffer, "SILC version %s is running :)", SILC_PLUGIN_VERSION);
        return WEECHAT_RC_OK;
    } else {
        weechat_printf(buffer, "SILC is not running :(");
        return WEECHAT_RC_ERROR;
    }
}

void silc_plugin_connected(SilcClient client, SilcClientConnection conn, SilcClientConnectionStatus status,
        SilcStatus error, const char *message, void *context) {
    if (status == SILC_CLIENT_CONN_DISCONNECTED) {
        weechat_log_printf("Disconnected: %s", message ? message : "");
        return;
    }

    if (status != SILC_CLIENT_CONN_SUCCESS && status != SILC_CLIENT_CONN_SUCCESS_RESUME) {
        weechat_log_printf("Error connecting to server: %d", status);
        return;
    }
    silc_plugin->connection = conn;

    weechat_log_printf("connection successfull");
}

int command_sconnect(void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol) {
    char *servername;

    if (argc < 2) {
        weechat_printf(buffer, "you need to specify a servername to connect to");
        return WEECHAT_RC_ERROR;
    }

    servername = argv[1];

    // create a new buffer for this connection but merge it into the main window
    silc_plugin->server_buffer = weechat_buffer_new(servername, NULL, NULL, NULL, NULL);
    weechat_buffer_merge(silc_plugin->server_buffer, weechat_buffer_search_main());

    weechat_printf(silc_plugin->server_buffer, "trying to connect to %s", servername);
    if (!silc_client_connect_to_server(silc_plugin->client, NULL, silc_plugin->public_key, silc_plugin->private_key,
            servername, 706, silc_plugin_connected, NULL)) {
        weechat_printf(silc_plugin->server_buffer, "connection to server failed");
        weechat_buffer_close(silc_plugin->server_buffer);
        return WEECHAT_RC_ERROR;
    }
    return WEECHAT_RC_OK;
}
