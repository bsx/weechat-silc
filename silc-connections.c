#include "silc-connections.h"

struct SilcPluginServer *find_server_for_buffer(struct t_gui_buffer *server_buffer) {
    struct SilcPluginServer *server;
    server = server_list;
    while (server = server->next) {
        if (server->server_buffer == server_buffer) {
            return server;
        }
    }
}

struct SilcPluginChannel *find_channel_for_buffer(struct t_gui_buffer *channel_buffer) {
    struct SilcPluginServer *server;
    struct SilcPluginChannel *channel;

    server = server_list;
    while (server = server->next) {
        channel = server->channels;
        while (channel = channel->next) {
            if (channel->channel_buffer == channel_buffer) {
                return channel;
            }
        }
    }
}
