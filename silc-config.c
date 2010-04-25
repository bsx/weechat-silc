#include "silc.h"
#include "silcclient.h"

#include "silc-plugin.h"
#include "silc-config.h"

int silc_config_reload(void *data, struct t_config_file *config_file) {
    // reload called
    return WEECHAT_RC_OK;
}

int silc_plugin_config_init() {
    config_file = weechat_config_new(SILC_CONFIG_NAME, &silc_config_reload, NULL);
    return 0;
}

int silc_plugin_config_write() {
    return weechat_config_write(config_file);
}

int silc_plugin_config_read() {
    return weechat_config_read(config_file);
}

void silc_plugin_config_free() {
    return weechat_config_free(config_file);
}
