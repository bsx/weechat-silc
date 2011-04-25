#include "weechat-plugin.h"
#include "silc.h"
#include "silcclient.h"

#include "silc-plugin.h"
#include "silc-connections.h"
#include "silc-modes.h"

char *silc_bar_buffer_name(void *data, struct t_gui_bar_item *item, struct t_gui_window *window) {
    struct t_gui_buffer *buffer;
    SilcPluginServerList server;
    SilcPluginChannelList channel;
    SilcPluginQueryList query;
    char *title, *buffer_name, *mode, *pattern;
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
                mode = format_channel_mode(channel->channel_entry->mode);
            } else if (query) {
                title = query->client_entry->nickname;
                mode = format_user_mode(query->client_entry->mode);
            } else {
                title = server->server_name;
                mode = NULL;
            }
            length = strlen(title) + 3;
            if (mode) {
                length += strlen(mode) + 2;
                pattern = "%s(%s)";
            } else {
                pattern = "%s";
            }
            buffer_name = malloc(length);
            snprintf(buffer_name, length, pattern, title, mode);
            return buffer_name;
        }
    }
    return NULL;
}

char *silc_bar_input_prompt(void *data, struct t_gui_bar_item *item, struct t_gui_window *window) {
    struct t_gui_buffer *buffer;
    SilcPluginServerList server;
    SilcPluginChannelList channel;
    char *prompt, *nick, *mode, *pattern;
    size_t length;

    if (!window) {
        window = weechat_current_window();
    }

    buffer = weechat_window_get_pointer(window, "buffer");

    if (buffer) {
        server = find_server_for_buffer(buffer);
        channel = find_channel_for_buffer(buffer);
        if (server) {
            nick = server->connection->local_entry->nickname;
            if (channel) {
                mode = format_channeluser_mode(silc_client_on_channel(channel->channel_entry, server->connection->local_entry)->mode);
            } else {
                mode = format_user_mode(server->connection->local_entry->mode);
            }
            length = strlen(nick) + 3;
            if (mode) {
                length += strlen(mode) + 2;
                pattern = "%s(%s)";
            } else {
                pattern = "%s";
            }
            prompt = malloc(length);
            snprintf(prompt, length, pattern, nick, mode);
            return prompt;
        }
    }
    return NULL;
}

char *silc_bar_buffer_plugin (void *data, struct t_gui_bar_item *item, struct t_gui_window *window) {
    struct t_gui_buffer *buffer;
    struct t_weechat_plugin *ptr_plugin;
    SilcPluginServerList server;
    SilcPluginChannelList channel;
    const char *name;
    char buf[512];

    if (!window) {
        window = weechat_current_window();
    }

    buffer = weechat_window_get_pointer(window, "buffer");

    if (buffer) {
        ptr_plugin = weechat_buffer_get_pointer(buffer, "plugin");
        name = weechat_plugin_get_name(ptr_plugin);
        if (ptr_plugin == weechat_plugin) {
            server = find_server_for_buffer(buffer);
            channel = find_channel_for_buffer(buffer);
            if (server && channel) {
                snprintf(buf, sizeof(buf), "%s/%s", name, server->server_name);
            } else {
                snprintf(buf, sizeof(buf), "%s", name);
            }
        } else {
            snprintf(buf, sizeof(buf), "%s", name);
        }
        return strdup(buf);
    }
    return NULL;
}

void silc_bar_init() {
    weechat_bar_item_new("buffer_name", &silc_bar_buffer_name, NULL);
    weechat_bar_item_new("buffer_plugin", &silc_bar_buffer_plugin, NULL);
    weechat_bar_item_new("input_prompt", &silc_bar_input_prompt, NULL);
}
