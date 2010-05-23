#include "weechat-plugin.h"
#include "silc.h"
#include "silcclient.h"

#include "silc-plugin.h"
#include "silc-connections.h"

char *silc_bar_buffer_name(void *data, struct t_gui_bar_item *item, struct t_gui_window *window) {
    struct t_gui_buffer *buffer;
    SilcPluginServerList server;
    SilcPluginChannelList channel;
    SilcPluginQueryList query;
    char *title, *buffer_name;
    size_t length;

    if (!window) {
        window = weechat_current_window();
    }

    buffer = weechat_window_get_pointer(window, "buffer");

    if (buffer) {
        server = find_server_for_buffer(buffer);
        if (server) {
            channel = find_channel_for_buffer(buffer);
            query = find_query_for_buffer(buffer);
            if (channel) {
                title = channel->channel_name;
            } else if (query) {
                title = query->client_entry->nickname;
            } else {
                title = server->server_name;
            }
            length = strlen(title) + 5;
            buffer_name = malloc(length);
            snprintf(buffer_name, length, "%s(+?)", title);
            return buffer_name;
        }
    }
    return NULL;
}

char *silc_bar_input_prompt(void *data, struct t_gui_bar_item *item, struct t_gui_window *window) {
    struct t_gui_buffer *buffer;
    SilcPluginServerList server;
    char *prompt, *nick;
    size_t length;

    if (!window) {
        window = weechat_current_window();
    }

    buffer = weechat_window_get_pointer(window, "buffer");

    if (buffer) {
        server = find_server_for_buffer(buffer);
        if (server) {
            nick = server->connection->local_entry->nickname;
            length = strlen(nick) + 5;
            prompt = malloc(length);
            snprintf(prompt, length, "%s(+?)", nick);
            return prompt;
        }
    }
    return NULL;
}

void silc_bar_init() {
    weechat_bar_item_new("buffer_name", &silc_bar_buffer_name, NULL);
    weechat_bar_item_new("input_prompt", &silc_bar_input_prompt, NULL);
}
