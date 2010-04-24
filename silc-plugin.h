#ifndef SILC_PLUGIN_H
#define SILC_PLUGIN_H

#include "weechat-plugin.h"

struct t_weechat_plugin *weechat_plugin;

typedef struct {
    SilcClient client;
    bool running;
} *SilcPlugin;

SilcPlugin silc_plugin;

#endif /* SILC_PLUGIN_H */
