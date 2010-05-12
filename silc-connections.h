#ifndef __SILC_CONNECTIONS_H
#define __SILC_CONNECTIONS_H

typedef struct {
    struct t_gui_buffer *server_buffer;
} *SilcConnectionContext;

struct SilcPluginChannel {
    struct t_gui_buffer *channel_buffer;
    SilcChannelEntry channel_entry;
    SilcChannelPrivateKey channel_key;
    struct SilcPluginChannel *next;
};

typedef struct SilcPluginChannel *SilcPluginChannelList;

struct SilcPluginServer {
    struct t_gui_buffer *server_buffer;
    SilcClientConnection connection;
    SilcPublicKey server_key;
    SilcPluginChannelList channels;
    struct SilcPluginServer *next;
};

typedef struct SilcPluginServer *SilcPluginServerList;

SilcPluginServerList server_list;

struct SilcPluginServer *find_server_for_buffer(struct t_gui_buffer *server_buffer);
struct SilcPluginChannel *find_channel_for_buffer(struct t_gui_buffer *channel_buffer);
struct SilcPluginServer *add_server(SilcClientConnection connection, SilcPublicKey server_key, struct t_gui_buffer *server_buffer);
struct SilcPluginChannel *add_channel(struct SilcPluginServer *server, SilcChannelEntry channel_entry, SilcChannelPrivateKey channel_key, struct t_gui_buffer *channel_buffer);

#endif /* __SILC_CONNECTIONS_H */
