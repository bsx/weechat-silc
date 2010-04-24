#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"
#include "commands.h"

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
