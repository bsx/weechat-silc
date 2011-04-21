#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "silc-connections.h"

void silc_nicklist_update_internal(SilcChannelUser user, struct t_gui_buffer *channelbuffer, struct t_gui_nick *nick);

struct t_gui_nick *silc_nicklist_add(SilcChannelUser user) {
    struct t_gui_nick *nick = NULL;
    struct t_gui_buffer *channelbuffer = NULL;

    channelbuffer = find_buffer_for_channel(user->channel);
    nick = weechat_nicklist_add_nick(channelbuffer, NULL, user->client->nickname, "white", "", "red", 1);
    if (nick) {
        silc_nicklist_update_internal(user, channelbuffer, nick);
    }
    return nick;
}

void silc_nicklist_remove(SilcChannelEntry channel_entry, SilcClientEntry user_client) {
    struct t_gui_nick *nick = NULL;
    struct t_gui_buffer *channelbuffer = NULL;

    channelbuffer = find_buffer_for_channel(channel_entry);
    nick = weechat_nicklist_search_nick(channelbuffer, NULL, user_client->nickname);
    if (nick) {
        weechat_nicklist_remove_nick(channelbuffer, nick);
    }
}

void silc_nicklist_update(SilcChannelUser user) {
    struct t_gui_nick *nick = NULL;
    struct t_gui_buffer *channelbuffer = NULL;

    channelbuffer = find_buffer_for_channel(user->channel);
    nick = weechat_nicklist_search_nick(channelbuffer, NULL, user->client->nickname);
    if (nick) {
        silc_nicklist_update_internal(user, channelbuffer, nick);
    }
}

void silc_nicklist_update_internal(SilcChannelUser user, struct t_gui_buffer *channelbuffer, struct t_gui_nick *nick) {
    if (user->mode & SILC_CHANNEL_UMODE_CHANOP) {
        weechat_nicklist_nick_set(channelbuffer, nick, "prefix", "@");
    }
    if (user->channel->founder_key != NULL && user->client->public_key != NULL &&
    silc_pkcs_public_key_compare(user->client->public_key, user->channel->founder_key)) {
        weechat_nicklist_nick_set(channelbuffer, nick, "color", "red");
    }
}
