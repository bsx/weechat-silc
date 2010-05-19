#ifndef __SILC_CONNECTIONS_H
#define __SILC_CONNECTIONS_H

typedef struct {
    struct t_gui_buffer *server_buffer;
} *SilcConnectionContext;

struct SilcPluginChannel {
    struct t_gui_buffer *channel_buffer;
    char *channel_name;
    SilcChannelEntry channel_entry;
    SilcChannelPrivateKey channel_key;
    struct SilcPluginChannel *prev;
    struct SilcPluginChannel *next;
};

typedef struct SilcPluginChannel *SilcPluginChannelList;

struct SilcPluginQuery {
    struct t_gui_buffer *query_buffer;
    SilcClientEntry client_entry;
    struct SilcPluginQuery *prev;
    struct SilcPluginQuery *next;
};

typedef struct SilcPluginQuery *SilcPluginQueryList;

struct SilcPluginServer {
    struct t_gui_buffer *server_buffer;
    char *server_name;
    SilcClientConnection connection;
    SilcPublicKey server_key;
    SilcPluginChannelList channels;
    SilcPluginQueryList queries;
    struct SilcPluginServer *prev;
    struct SilcPluginServer *next;
};

typedef struct SilcPluginServer *SilcPluginServerList;

SilcPluginServerList server_list;

struct SilcPluginServer *find_server_for_buffer(struct t_gui_buffer *server_buffer);
struct SilcPluginChannel *find_channel_for_buffer(struct t_gui_buffer *channel_buffer);
struct SilcPluginQuery *find_query_for_buffer(struct t_gui_buffer *query_buffer);
struct SilcPluginQuery *find_query_for_nick(struct SilcPluginServer *server, char *nickname);
struct SilcPluginServer *add_server(char *server_name, SilcClientConnection connection, SilcPublicKey server_key, struct t_gui_buffer *server_buffer);
struct SilcPluginChannel *add_channel(char *channel_name, struct SilcPluginServer *server, SilcChannelEntry channel_entry, SilcChannelPrivateKey channel_key, struct t_gui_buffer *channel_buffer);
struct SilcPluginQuery *add_query(struct SilcPluginServer *server, SilcClientEntry client_entry, struct t_gui_buffer *query_buffer);
void remove_server(struct SilcPluginServer *server);
void remove_channel(struct SilcPluginChannel *channel);
void remove_query(struct SilcPluginQuery *query);

struct t_gui_buffer *find_buffer_for_channel(SilcChannelEntry channel_entry);
struct t_gui_buffer *find_buffer_for_query(SilcClientEntry client_entry);

void list_connections(struct t_gui_buffer *buffer);

#endif /* __SILC_CONNECTIONS_H */
