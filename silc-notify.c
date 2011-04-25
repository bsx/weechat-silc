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
void silc_notify_cmode(SilcClient client, SilcClientConnection conn, va_list va);
void silc_notify_cumode(SilcClient client, SilcClientConnection conn, va_list va);

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
        case SILC_NOTIFY_TYPE_CMODE_CHANGE:
            silc_notify_cmode(client, conn, va);
            break;
        case SILC_NOTIFY_TYPE_CUMODE_CHANGE:
            silc_notify_cumode(client, conn, va);
            break;
        case SILC_NOTIFY_TYPE_NICK_CHANGE:
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

char *silc_util_name_for_type(SilcIdType id_type, void *entry) {
    switch (id_type) {
        case SILC_ID_CLIENT:
            return ((SilcClientEntry) entry)->nickname;
        case SILC_ID_SERVER:
            return ((SilcServerEntry) entry)->server_name;
        case SILC_ID_CHANNEL:
            return ((SilcChannelEntry) entry)->channel_name;
        default:
            return "<unknown>";
    }
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
    setter_name = silc_util_name_for_type(id_type, setter);
    weechat_buffer_set(channel->context, "title", channel->topic);
    if (old_topic) {
        weechat_printf(channel->context, "%s%s has changed topic from \"%s\" to \"%s\"", weechat_prefix("network"), setter_name, old_topic, new_topic);
        free(old_topic);
    } else {
        weechat_printf(channel->context, "%s%s has set topic to \"%s\"", weechat_prefix("network"), setter_name, new_topic);
    }
}

void silc_notify_cumode(SilcClient client, SilcClientConnection conn, va_list va) {
    SilcIdType id_type;
    void *changer;
    char *changer_name;
    SilcUInt32 mode;
    SilcClientEntry target;
    SilcChannelEntry channel;

    id_type = va_arg(va, int);
    changer = va_arg(va, void *);
    mode = va_arg(va, SilcUInt32);
    target = va_arg(va, SilcClientEntry);
    channel = va_arg(va, SilcChannelEntry);
    changer_name = silc_util_name_for_type(id_type, changer);

    silc_nicklist_update(silc_client_on_channel(channel, target));
    weechat_printf(channel->context, "%schannel user mode %s [%s] by %s", weechat_prefix("network"), target->nickname, silc_client_chumode(mode), changer_name);
    weechat_bar_item_update("input_prompt");
}

void silc_notify_cmode(SilcClient client, SilcClientConnection conn, va_list va) {
    SilcIdType id_type;
    void *changer;
    char *changer_name;
    SilcUInt32 mode;
    char *cipher_name;
    char *hmac_name;
    char *passphrase;
    SilcPublicKey founder_key;
    SilcDList channel_pubkeys;
    SilcChannelEntry channel;

    id_type = va_arg(va, int);
    changer = va_arg(va, void *);
    mode = va_arg(va, SilcUInt32);
    cipher_name = va_arg(va, char *);
    hmac_name = va_arg(va, char *);
    passphrase = va_arg(va, char *);
    founder_key = va_arg(va, SilcPublicKey);
    channel_pubkeys = va_arg(va, SilcDList);
    channel = va_arg(va, SilcChannelEntry);
    changer_name = silc_util_name_for_type(id_type, changer);

    weechat_printf(channel->context, "%schannel mode [%s] by %s", weechat_prefix("network"), silc_client_chmode(mode, cipher_name, hmac_name), changer_name);
    weechat_bar_item_update("buffer_name");
}
