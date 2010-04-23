#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

WEECHAT_PLUGIN_NAME("silc");
WEECHAT_PLUGIN_DESCRIPTION("SILC plugin for WeeChat");
WEECHAT_PLUGIN_AUTHOR("Bernd Stolle <bsx+silc@0xcafec0.de>");
WEECHAT_PLUGIN_VERSION("0.1");
WEECHAT_PLUGIN_LICENSE("BSD");

int weechat_plugin_init (struct t_weechat_plugin *plugin, int argc, char *argv[]) {
    // DoSomething(tm)
    return WEECHAT_RC_OK;
}

int weechat_plugin_end (struct t_weechat_plugin *plugin) {
    return WEECHAT_RC_OK;
}
