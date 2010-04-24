#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "silc-operations.h"

WEECHAT_PLUGIN_NAME("silc");
WEECHAT_PLUGIN_DESCRIPTION("SILC plugin for WeeChat");
WEECHAT_PLUGIN_AUTHOR("Bernd Stolle <bsx+silc@0xcafec0.de>");
WEECHAT_PLUGIN_VERSION("0.1");
WEECHAT_PLUGIN_LICENSE("BSD");

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

/* ===== callbacks for our commands ===== */

int command_silc(void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol) {
    if (silc_plugin->running) {
        weechat_printf(buffer, "SILC is running :)");
        return WEECHAT_RC_OK;
    } else {
        weechat_printf(buffer, "SILC is not running :(");
        return WEECHAT_RC_ERROR;
    }
}

// this is the simulation of a main loop that makes the silc client do its work
int timer_silc(void *data, int remaining_calls) {
    silc_client_run_one(silc_plugin->client);
    return WEECHAT_RC_OK;
}

/* ===== weechat plugin interface ===== */

int weechat_plugin_init(struct t_weechat_plugin *plugin, int argc, char *argv[]) {
    weechat_plugin = plugin;
    SilcClientParams params;

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

    if (!silc_client_init(silc_plugin->client, silc_get_username(), silc_net_localhost(), "Lord Vader", silc_running, silc_plugin)) {
        weechat_log_printf("could not initialize SILC client");
        return WEECHAT_RC_ERROR;
    }
    // tick the client once to complete the initialization
    silc_client_run_one(silc_plugin->client);
    weechat_log_printf("SILC client initialized");

    weechat_hook_command("silc", "This is the SILC plugin", "", "", NULL, &command_silc, NULL);
    weechat_hook_timer(50, 0, 0, &timer_silc, NULL);

    return WEECHAT_RC_OK;
}

int weechat_plugin_end(struct t_weechat_plugin *plugin) {
    silc_client_stop(silc_plugin->client, silc_stopped, NULL);
    return WEECHAT_RC_OK;
}

