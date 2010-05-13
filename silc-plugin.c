#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "silc-operations.h"
#include "silc-commands.h"
#include "silc-config.h"
#include "silc-keys.h"
#include "silc-connections.h"

WEECHAT_PLUGIN_NAME(SILC_PLUGIN_NAME);
WEECHAT_PLUGIN_DESCRIPTION(SILC_PLUGIN_DESCRIPTION);
WEECHAT_PLUGIN_AUTHOR(SILC_PLUGIN_AUTHOR);
WEECHAT_PLUGIN_VERSION(SILC_PLUGIN_VERSION);
WEECHAT_PLUGIN_LICENSE(SILC_PLUGIN_LICENSE);

/* ===== SILC callbacks ===== */

void silc_running(SilcClient client, void *application) {
    weechat_log_printf("SILC initialized");
    silc_plugin->running = 1;
}

void silc_stopped(SilcClient client, void *application) {
    weechat_log_printf("SILC stopped");
    silc_plugin->running = 0;
    silc_client_free(silc_plugin->client);
    silc_free(silc_plugin);
}

/* ===== weechat plugin interface ===== */

// this is the simulation of a main loop that makes the silc client do its work
int timer_silc(void *data, int remaining_calls) {
    silc_client_run_one(silc_plugin->client);
    return WEECHAT_RC_OK;
}

int weechat_plugin_init(struct t_weechat_plugin *plugin, int argc, char *argv[]) {
    weechat_plugin = plugin;
    SilcClientParams params;

    if (silc_plugin_config_init() < 0) {
        weechat_log_printf("could not initialize SILC plugin config");
        return WEECHAT_RC_ERROR;
    }

    if (silc_plugin_config_read() < 0) {
        weechat_log_printf("could not read SILC plugin config file");
        return WEECHAT_RC_ERROR;
    }

    memset(&params, 0, sizeof(params));
    params.threads = TRUE;

    silc_plugin = silc_calloc(1, sizeof(*silc_plugin));
    if (!silc_plugin) {
        weechat_log_printf("could not allocate plugin context");
        return WEECHAT_RC_ERROR;
    }
    weechat_log_printf("SILC plugin context allocated");

    silc_plugin->client = silc_client_alloc(&ops, &params, silc_plugin, NULL);
    if (!silc_plugin->client) {
        weechat_log_printf("could not allocate SILC client");
        return WEECHAT_RC_ERROR;
    }
    weechat_log_printf("SILC client allocated");

    if (!silc_client_init(silc_plugin->client, weechat_config_string(option_default_username),
                silc_net_localhost(), weechat_config_string(option_default_realname), silc_running, silc_plugin)) {
        weechat_log_printf("could not initialize SILC client");
        return WEECHAT_RC_ERROR;
    }
    // tick the client once to complete the initialization
    silc_client_run_one(silc_plugin->client);
    weechat_log_printf("SILC client initialized");

    silc_plugin_get_keypair("weechat", "", 1, &silc_plugin->public_key, &silc_plugin->private_key);

    server_list = malloc(sizeof(struct SilcPluginServer));
    memset(server_list, 0, sizeof(struct SilcPluginServer));

    weechat_hook_command("silc", "This is the SILC plugin", "", "", NULL, &command_silc, NULL);
    weechat_hook_timer(50, 0, 0, &timer_silc, NULL);

    return WEECHAT_RC_OK;
}

int weechat_plugin_end(struct t_weechat_plugin *plugin) {
    silc_client_stop(silc_plugin->client, silc_stopped, NULL);
    silc_plugin_config_write();
    silc_plugin_config_free();
    return WEECHAT_RC_OK;
}

