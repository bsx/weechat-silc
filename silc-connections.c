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

struct SilcPluginServer *add_server(SilcClientConnection connection, SilcPublicKey server_key, struct t_gui_buffer *server_buffer) {
    struct SilcPluginServer *server;
    struct SilcPluginServer *new_server;

    server = server_list;
    while (server->next != NULL) {
        server = server->next;
    }

    new_server = malloc(sizeof(struct SilcPluginServer));
    new_server->connection = connection;
    new_server->server_key = server_key;
    new_server->server_buffer = server_buffer;
    new_server->channels = malloc(sizeof(struct SilcPluginChannel));
    new_server->next = NULL;

    server->next = new_server;

    return new_server;
}

struct SilcPluginChannel *add_channel(struct SilcPluginServer *server, SilcChannelEntry channel_entry, SilcChannelPrivateKey channel_key, struct t_gui_buffer *channel_buffer) {
    struct SilcPluginChannel *channel;
    struct SilcPluginChannel *new_channel;

    channel = server->channels;
    while (channel->next != NULL) {
        channel = channel->next;
    }

    new_channel = malloc(sizeof(struct SilcPluginChannel));
    new_channel->channel_entry = channel_entry;
    new_channel->channel_key = channel_key;
    new_channel->channel_buffer = channel_buffer;
    new_channel->next = NULL;

    channel->next = new_channel;

    return new_channel;
}
