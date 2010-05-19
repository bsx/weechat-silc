#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "silc-connections.h"

struct SilcPluginServer *find_server_for_buffer(struct t_gui_buffer *server_buffer) {
    struct SilcPluginServer *server;
    struct SilcPluginChannel *channel;
    struct SilcPluginQuery *query;

    server = server_list;
    while ((server = server->next) != NULL) {
        if (server->server_buffer == server_buffer) {
            return server;
        }
        channel = server->channels;
        while ((channel = channel->next) != NULL) {
            if (channel->channel_buffer == server_buffer) {
                return server;
            }
        }
        query = server->queries;
        while ((query = query->next) != NULL) {
            if (query->query_buffer == server_buffer) {
                return server;
            }
        }
    }
    return NULL;
}

struct SilcPluginChannel *find_channel_for_buffer(struct t_gui_buffer *channel_buffer) {
    struct SilcPluginServer *server;
    struct SilcPluginChannel *channel;

    server = server_list;
    while ((server = server->next) != NULL) {
        channel = server->channels;
        while ((channel = channel->next) != NULL) {
            if (channel->channel_buffer == channel_buffer) {
                return channel;
            }
        }
    }
    return NULL;
}

struct SilcPluginQuery *find_query_for_buffer(struct t_gui_buffer *query_buffer) {
    struct SilcPluginServer *server;
    struct SilcPluginQuery *query;

    server = server_list;
    while ((server = server->next) != NULL) {
        query = server->queries;
        while ((query = query->next) != NULL) {
            if (query->query_buffer == query_buffer) {
                return query;
            }
        }
    }
    return NULL;
}

struct t_gui_buffer *find_buffer_for_channel(SilcChannelEntry channel_entry) {
    struct SilcPluginServer *server;
    struct SilcPluginChannel *channel;

    server = server_list;
    while ((server = server->next) != NULL) {
        channel = server->channels;
        while ((channel = channel->next) != NULL) {
            if (channel->channel_entry == channel_entry) {
                return channel->channel_buffer;
            }
        }
    }
    return NULL;
}

struct t_gui_buffer *find_buffer_for_query(SilcClientEntry client_entry) {
    struct SilcPluginServer *server;
    struct SilcPluginQuery *query;

    server = server_list;
    while ((server = server->next) != NULL) {
        query = server->queries;
        while ((query = query->next) != NULL) {
            if (query->client_entry == client_entry) {
                return query->query_buffer;
            }
        }
    }
    return NULL;
}

struct SilcPluginServer *add_server(char *server_name, SilcClientConnection connection, SilcPublicKey server_key, struct t_gui_buffer *server_buffer) {
    struct SilcPluginServer *server;
    struct SilcPluginServer *new_server;
    char *server_name_copy;
    size_t length;

    length = strlen(server_name);
    server_name_copy = malloc(length + 1);
    memset(server_name_copy, 0, length + 1);
    strncpy(server_name_copy, server_name, length);

    server = server_list;
    while (server->next != NULL) {
        server = server->next;
    }

    new_server = malloc(sizeof(struct SilcPluginServer));
    memset(new_server, 0, sizeof(struct SilcPluginServer));

    new_server->server_name = server_name_copy;
    new_server->connection = connection;
    new_server->server_key = server_key;
    new_server->server_buffer = server_buffer;
    new_server->channels = malloc(sizeof(struct SilcPluginChannel));
    new_server->queries = malloc(sizeof(struct SilcPluginQuery));
    memset(new_server->channels, 0, sizeof(struct SilcPluginChannel));
    memset(new_server->queries, 0, sizeof(struct SilcPluginQuery));

    new_server->prev = server;
    server->next = new_server;

    return new_server;
}

struct SilcPluginChannel *add_channel(char *channel_name, struct SilcPluginServer *server, SilcChannelEntry channel_entry, SilcChannelPrivateKey channel_key, struct t_gui_buffer *channel_buffer) {
    struct SilcPluginChannel *channel;
    struct SilcPluginChannel *new_channel;

    channel = server->channels;
    while (channel->next != NULL) {
        channel = channel->next;
    }

    new_channel = malloc(sizeof(struct SilcPluginChannel));
    memset(new_channel, 0, sizeof(struct SilcPluginChannel));

    new_channel->channel_name = channel_name;
    new_channel->channel_entry = channel_entry;
    new_channel->channel_key = channel_key;
    new_channel->channel_buffer = channel_buffer;

    new_channel->prev = channel;
    channel->next = new_channel;

    return new_channel;
}

struct SilcPluginQuery *add_query(struct SilcPluginServer *server, SilcClientEntry client_entry, struct t_gui_buffer *query_buffer) {
    struct SilcPluginQuery *query;
    struct SilcPluginQuery *new_query;

    query = server->queries;
    while (query->next != NULL) {
        query = query->next;
    }

    new_query = malloc(sizeof(struct SilcPluginQuery));
    memset(new_query, 0, sizeof(struct SilcPluginQuery));

    new_query->client_entry = client_entry;
    new_query->query_buffer = query_buffer;

    new_query->prev = query;
    query->next = new_query;

    return new_query;
}

void remove_server(struct SilcPluginServer *server) {
    struct SilcPluginServer *prev;
    struct SilcPluginServer *next;
    struct SilcPluginChannel *channel;
    struct SilcPluginChannel *next_chan;
    struct SilcPluginQuery *query;
    struct SilcPluginQuery *next_query;

    prev = server->prev;
    next = server->next;

    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }

    channel = server->channels;
    while (channel != NULL) {
        next_chan = channel->next;
        if (channel->channel_name != NULL) {
            free(channel->channel_name);
        }
        free(channel);
        channel = next_chan;
    }

    query = server->queries;
    while (query != NULL) {
        next_query = query->next;
        free(query);
        query = next_query;
    }

    free(server->server_name);
    free(server);
}

void remove_channel(struct SilcPluginChannel *channel) {
    struct SilcPluginChannel *prev;
    struct SilcPluginChannel *next;

    prev = channel->prev;
    next = channel->next;

    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }

    free(channel->channel_name);
    free(channel);
}

void remove_query(struct SilcPluginQuery *query) {
    struct SilcPluginQuery *prev;
    struct SilcPluginQuery *next;

    prev = query->prev;
    next = query->next;

    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }

    free(query);
}

void list_connections(struct t_gui_buffer *buffer) {
    struct SilcPluginServer *server;

    server = server_list;
    while (server->next != NULL) {
        server = server->next;
        weechat_printf(buffer, "Connection: %s", server->server_name);
    }
}
