#ifndef SILC_PLUGIN_H
#define SILC_PLUGIN_H

#include "weechat-plugin.h"

#define SILC_CONFIG_NAME "silc"
#define SILC_PLUGIN_NAME "silc"
#define SILC_PLUGIN_DESCRIPTION "bring secure internet live conferencing (SILC) to weechat"
#define SILC_PLUGIN_VERSION "0.1-" HGVERSION
#define SILC_PLUGIN_AUTHOR "Bernd Stolle <bsx+silc@0xcafec0.de>"
#define SILC_PLUGIN_LICENSE "BSD"

struct t_weechat_plugin *weechat_plugin;

typedef struct {
    SilcClient client;
    bool running;
} *SilcPlugin;

SilcPlugin silc_plugin;

#endif /* SILC_PLUGIN_H */
