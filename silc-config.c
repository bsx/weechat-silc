#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "silc-config.h"

int silc_config_reload(void *data, struct t_config_file *config_file) {
    // reload called
    return WEECHAT_RC_OK;
}

int silc_plugin_config_init() {
    config_file = weechat_config_new(SILC_CONFIG_NAME, &silc_config_reload, NULL);
    struct t_config_section *section;

    section = weechat_config_new_section(config_file, "server_default", 0, 0,
            NULL, NULL,
            NULL, NULL,
            NULL, NULL,
            NULL, NULL,
            NULL, NULL);

    char *username = silc_get_username();
    option_default_username = weechat_config_new_option(config_file, section, "username", "string",
            "default username for SILC connections", NULL, 0, 0, "", username,
            0, NULL, NULL, NULL, NULL, NULL, NULL);
    option_default_realname = weechat_config_new_option(config_file, section, "realname", "string",
            "realname for SILC connections", NULL, 0, 0, "", username,
            0, NULL, NULL, NULL, NULL, NULL, NULL);
    option_default_nick = weechat_config_new_option(config_file, section, "nick", "string",
            "nickname for SILC connections", NULL, 0, 0, "", username,
            0, NULL, NULL, NULL, NULL, NULL, NULL);
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
