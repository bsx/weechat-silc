#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "silc-connections.h"

/* callback funktions for weechat */

int silc_plugin_channel_input(void *data, struct t_gui_buffer *buffer, const char *input_data) {
    SilcPluginChannelList channel = data;
    SilcPluginServerList server = find_server_for_buffer(buffer);

    silc_client_send_channel_message(silc_plugin->client, server->connection, channel->channel_entry,
            NULL, SILC_MESSAGE_FLAG_NONE, NULL, (unsigned char *)input_data, strlen(input_data));
    weechat_printf(channel->channel_buffer, "%s\t%s", server->connection->local_entry->nickname, input_data);

    return WEECHAT_RC_OK;
}

/* silc library callbacks */

void silc_say(SilcClient client, SilcClientConnection conn, SilcClientMessageType type, char *msg, ...) {
    char str[200];
    va_list va;
    struct t_gui_buffer *server_buffer = NULL;
    SilcConnectionContext ctx = conn->context;
    if (ctx) {
        server_buffer = ctx->server_buffer;
    }
    va_start(va, msg);
    vsnprintf(str, sizeof(str) - 1, msg, va);
    weechat_printf(server_buffer, "SILC: %s", str);
    va_end(va);
}

void silc_channel_message(SilcClient client, SilcClientConnection conn, SilcClientEntry sender,
        SilcChannelEntry channel, SilcMessagePayload payload, SilcChannelPrivateKey key,
        SilcMessageFlags flags, const unsigned char *message, SilcUInt32 message_len) {
    struct t_gui_buffer *channel_buffer;

    channel_buffer = find_buffer_for_channel(channel);
    if (channel_buffer == NULL) {
        weechat_log_printf("BUG: received message on channel we don't know about! channel: %s, server: %s, message: %s, sender: %s",
               channel->channel_name, channel->server, message, sender->nickname);
    } else {
        weechat_printf(channel_buffer, "%s\t%s", sender->nickname, message);
        weechat_log_printf("TODO - handle message flags: %d", flags);
    }
}

void silc_private_message(SilcClient client, SilcClientConnection conn, SilcClientEntry sender,
        SilcMessagePayload payload, SilcMessageFlags flags, const unsigned char *message, SilcUInt32 message_len) {
    weechat_log_printf("silc_private_message was called");
}

void silc_notify(SilcClient client, SilcClientConnection conn, SilcNotifyType type, ...) {
    // "infrastructure"
    va_list va;
    struct t_gui_buffer *buffer = NULL;
    SilcConnectionContext ctx = conn->context;

    // possbile varargs
    SilcClientEntry other_client;
    SilcChannelEntry channel;
    char *str;

    va_start(va, type);

    switch (type) {
        case SILC_NOTIFY_TYPE_NONE:
            str = va_arg(va, char *);
            if (ctx) {
                buffer = ctx->server_buffer;
            }
            weechat_printf(buffer, "%s%s", weechat_prefix("network"), str);
            break;
        case SILC_NOTIFY_TYPE_MOTD:
            str = va_arg(va, char *);
            if (ctx) {
                buffer = ctx->server_buffer;
            }
            weechat_printf(buffer, "%sMOTD: %s", weechat_prefix("network"), str);
            break;
        case SILC_NOTIFY_TYPE_JOIN:
            other_client = va_arg(va, SilcClientEntry);
            channel = va_arg(va, SilcChannelEntry);
            buffer = find_buffer_for_channel(channel);
            weechat_printf(buffer, "%s%s has joined channel %s", weechat_prefix("join"), other_client->nickname, channel->channel_name);
            break;
        case SILC_NOTIFY_TYPE_LEAVE:
            other_client = va_arg(va, SilcClientEntry);
            channel = va_arg(va, SilcChannelEntry);
            buffer = find_buffer_for_channel(channel);
            weechat_printf(buffer, "%s%s has left channel %s", weechat_prefix("quit"), other_client->nickname, channel->channel_name);
            break;
        default:
            weechat_log_printf("silc_notify was called with unhandled type %d", type);
            break;
    }

    va_end(va);
}

void silc_command(SilcClient client, SilcClientConnection conn, SilcBool success, SilcCommand command, SilcStatus status, SilcUInt32 argc, unsigned char **argv) {
    weechat_log_printf("SILC command %d was called. Success: %x", command, success);
}

void silc_command_reply(SilcClient client, SilcClientConnection conn, SilcCommand command, SilcStatus status, SilcStatus error, va_list ap) {
    // "infrastructure"
    struct t_gui_buffer *channelbuffer;
    SilcConnectionContext ctx = conn->context;
    SilcPluginChannelList channel;

    // possible args
    char *str, *topic, *cipher, *hmac;
    SilcChannelEntry channel_entry;
    SilcUInt32 mode, userlimit;
    SilcHashTableList *userlist;
    SilcPublicKey key;
    SilcDList pubkeys;

    // needed for the nicklist
    SilcClientEntry user_client;
    SilcChannelUser user;

    switch (command) {
        case SILC_COMMAND_JOIN:
            str = va_arg(ap, char *);
            channel_entry = va_arg(ap, SilcChannelEntry);
            mode = va_arg(ap, SilcUInt32);
            userlist = va_arg(ap, SilcHashTableList *);
            topic = va_arg(ap, char *);
            cipher = va_arg(ap, char *);
            hmac = va_arg(ap, char *);
            key = va_arg(ap, SilcPublicKey);
            pubkeys = va_arg(ap, SilcDList);
            userlimit = va_arg(ap, SilcUInt32);

            // record a reference to this channel
            channel = add_channel(str, find_server_for_buffer(ctx->server_buffer), channel_entry, NULL, NULL);

            // create a regular chat buffer and set some senible values
            channelbuffer = weechat_buffer_new(str, &silc_plugin_channel_input, channel, NULL, NULL);
            weechat_buffer_set(channelbuffer, "title", topic);
            weechat_buffer_set(channelbuffer, "hotlist", WEECHAT_HOTLIST_LOW);
            weechat_buffer_set(channelbuffer, "nicklist", "1");

            channel->channel_buffer = channelbuffer;

            // fill the nicklist with users currently on the channel
            while (silc_hash_table_get(userlist, (void **)&user_client, (void **)&user)) {
                weechat_nicklist_add_nick(channelbuffer, NULL, user_client->nickname, "white", user->mode & SILC_CHANNEL_UMODE_CHANOP ? "@" : "", "red", 1);
            }
            break;
        default:
            weechat_log_printf("unhandled command reply for %d", command);
            break;
    }
}

void silc_get_auth_method(SilcClient client, SilcClientConnection conn, char *hostname, SilcUInt16 port, SilcAuthMethod auth_method, SilcGetAuthMeth completion, void *context) {
    weechat_log_printf("silc_get_auth_method was called");
    completion(SILC_AUTH_NONE, NULL, 0, context);
}

void silc_verify_public_key(SilcClient client, SilcClientConnection conn, SilcConnectionType conn_type, SilcPublicKey public_key, SilcVerifyPublicKey completion, void *context) {
    SilcPluginServerList server;

    weechat_log_printf("silc_verify_public_key was called");

    server = find_server_for_buffer(context);
    if (server != NULL) {
        server->server_key = public_key;
    }

    completion(TRUE, context);
}

void silc_ask_passphrase(SilcClient client, SilcClientConnection conn, SilcAskPassphrase completion, void *context) {
    weechat_log_printf("silc_ask_passphrase was called");
}

void silc_key_agreement(SilcClient client, SilcClientConnection conn, SilcClientEntry client_entry, const char *hostname, SilcUInt16 protocol, SilcUInt16 port) {
    weechat_log_printf("silc_key_agreement was called");
}

void silc_ftp(SilcClient client, SilcClientConnection conn, SilcClientEntry client_entry, SilcUInt32 session_id, const char *hostname, SilcUInt16 port) {
    weechat_log_printf("silc_ftp was called");
}

