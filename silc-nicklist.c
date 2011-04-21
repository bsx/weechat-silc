#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "silc-connections.h"

struct t_gui_nick *silc_nicklist_add(SilcChannelEntry channel_entry, SilcClientEntry user_client) {
    struct t_gui_buffer *channelbuffer = NULL;

    channelbuffer = find_buffer_for_channel(channel_entry);
    return weechat_nicklist_add_nick(channelbuffer, NULL, user_client->nickname,
            (channel_entry->founder_key != NULL && user_client->public_key != NULL &&
            silc_pkcs_public_key_compare(user_client->public_key, channel_entry->founder_key)) ? "red": "white",
            "", "red", 1);
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

