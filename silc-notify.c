#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "silc-connections.h"
#include "silc-nicklist.h"

/* prototypes */
void silc_notify_none(SilcClient client, SilcClientConnection conn, va_list va);
void silc_notify_motd(SilcClient client, SilcClientConnection conn, va_list va);
void silc_notify_join(SilcClient client, SilcClientConnection conn, va_list va);
void silc_notify_leave(SilcClient client, SilcClientConnection conn, va_list va);
void silc_notify_topic(SilcClient client, SilcClientConnection conn, va_list va);
void silc_notify_signoff(SilcClient client, SilcClientConnection conn, va_list va);

void silc_notify(SilcClient client, SilcClientConnection conn, SilcNotifyType type, ...) {
    va_list va;

    va_start(va, type);

    switch (type) {
        case SILC_NOTIFY_TYPE_NONE:
            silc_notify_none(client, conn, va);
            break;
        case SILC_NOTIFY_TYPE_MOTD:
            silc_notify_motd(client, conn, va);
            break;
        case SILC_NOTIFY_TYPE_JOIN:
            silc_notify_join(client, conn, va);
            break;
        case SILC_NOTIFY_TYPE_LEAVE:
            silc_notify_leave(client, conn, va);
            break;
        case SILC_NOTIFY_TYPE_TOPIC_SET:
            silc_notify_topic(client, conn, va);
            break;
        case SILC_NOTIFY_TYPE_SIGNOFF:
            silc_notify_signoff(client, conn, va);
            break;
        case SILC_NOTIFY_TYPE_NICK_CHANGE:
        case SILC_NOTIFY_TYPE_CMODE_CHANGE:
        case SILC_NOTIFY_TYPE_CUMODE_CHANGE:
        case SILC_NOTIFY_TYPE_CHANNEL_CHANGE:
        case SILC_NOTIFY_TYPE_SERVER_SIGNOFF:
        case SILC_NOTIFY_TYPE_KICKED:
        case SILC_NOTIFY_TYPE_KILLED:
        case SILC_NOTIFY_TYPE_ERROR:
        case SILC_NOTIFY_TYPE_WATCH:
        default:
            weechat_log_printf("silc_notify was called with unhandled type %d", type);
            break;
    }

    va_end(va);
}

void silc_notify_none(SilcClient client, SilcClientConnection conn, va_list va) {
    struct t_gui_buffer *buffer = NULL;
    char *message;
    SilcConnectionContext ctx = conn->context;

    message = va_arg(va, char *);
    if (ctx) {
        buffer = ctx->server_buffer;
    }
    weechat_printf(buffer, "%s%s", weechat_prefix("network"), message);
}

void silc_notify_motd(SilcClient client, SilcClientConnection conn, va_list va) {
    struct t_gui_buffer *buffer = NULL;
    char *message;
    SilcConnectionContext ctx = conn->context;

    message = va_arg(va, char *);
    if (ctx) {
        buffer = ctx->server_buffer;
    }
    weechat_printf(buffer, "%sMOTD: %s", weechat_prefix("network"), message);
}

void silc_notify_join(SilcClient client, SilcClientConnection conn, va_list va) {
    SilcClientEntry joiner;
    SilcChannelEntry channel;

    joiner = va_arg(va, SilcClientEntry);
    channel = va_arg(va, SilcChannelEntry);
    silc_nicklist_add(silc_client_on_channel(channel, joiner));
    weechat_printf(channel->context, "%s%s has joined channel %s", weechat_prefix("join"), joiner->nickname, channel->channel_name);
}

void silc_notify_leave(SilcClient client, SilcClientConnection conn, va_list va) {
    SilcClientEntry quitter;
    SilcChannelEntry channel;

    quitter = va_arg(va, SilcClientEntry);
    channel = va_arg(va, SilcChannelEntry);
    silc_nicklist_remove(channel, quitter);
    weechat_printf(channel->context, "%s%s has left channel %s", weechat_prefix("quit"), quitter->nickname, channel->channel_name);
}

void silc_notify_signoff(SilcClient client, SilcClientConnection conn, va_list va) {
    SilcClientEntry quitter;
    SilcChannelEntry channel;
    char *message;

    quitter = va_arg(va, SilcClientEntry);
    message = va_arg(va, char*);
    channel = va_arg(va, SilcChannelEntry);
    silc_nicklist_remove(channel, quitter);
    weechat_printf(channel->context, "%s%s has quit (%s)", weechat_prefix("quit"), quitter->nickname, message);
}

void silc_notify_topic(SilcClient client, SilcClientConnection conn, va_list va) {
    SilcIdType id_type;
    SilcChannelEntry channel;
    char *old_topic;
    char *new_topic;
    char *setter_name;
    void *setter;

    id_type = va_arg(va, int);
    setter = va_arg(va, void *);
    new_topic = va_arg(va, char*);
    channel = va_arg(va, SilcChannelEntry);
    old_topic = strdup(weechat_buffer_get_string(channel->context, "title"));
    weechat_buffer_set(channel->context, "title", channel->topic);
    switch (id_type) {
        case SILC_ID_CLIENT:
            setter_name = ((SilcClientEntry) setter)->nickname;
            break;
        case SILC_ID_SERVER:
            setter_name = ((SilcServerEntry) setter)->server_name;
            break;
        case SILC_ID_CHANNEL:
            setter_name = ((SilcChannelEntry) setter)->channel_name;
            break;
        default:
            setter_name = "<unknown>";
            break;
    }
    if (old_topic) {
        weechat_printf(channel->context, "%s%s has changed topic from \"%s\" to \"%s\"", weechat_prefix("network"), setter_name, old_topic, new_topic);
        free(old_topic);
    } else {
        weechat_printf(channel->context, "%s%s has set topic to \"%s\"", weechat_prefix("network"), setter_name, new_topic);
    }
}

