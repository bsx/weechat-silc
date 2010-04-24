#include "silc.h"
#include "silcclient.h"
#include "weechat-plugin.h"

#include "silc-plugin.h"

void silc_say(SilcClient client, SilcClientConnection conn, SilcClientMessageType type, char *msg, ...) {
    weechat_log_printf("silc_say was called");
}

void silc_channel_message(SilcClient client, SilcClientConnection conn, SilcClientEntry sender,
        SilcChannelEntry channel, SilcMessagePayload payload, SilcChannelPrivateKey key,
        SilcMessageFlags flags, const unsigned char *message, SilcUInt32 message_len) {
    weechat_log_printf("silc_channel_message was called");
}

void silc_private_message(SilcClient client, SilcClientConnection conn, SilcClientEntry sender,
        SilcMessagePayload payload, SilcMessageFlags flags, const unsigned char *message, SilcUInt32 message_len) {
    weechat_log_printf("silc_private_message was called");
}

void silc_notify(SilcClient client, SilcClientConnection conn, SilcNotifyType type, ...) {
    weechat_log_printf("silc_notify was called");
}

void silc_command(SilcClient client, SilcClientConnection conn, SilcBool success, SilcCommand command, SilcStatus status, SilcUInt32 argc, unsigned char **argv) {
    weechat_log_printf("silc_command was called");
}

void silc_command_reply(SilcClient client, SilcClientConnection conn, SilcCommand command, SilcStatus status, SilcStatus error, va_list ap) {
    weechat_log_printf("silc_command_reply was called");
}

void silc_get_auth_method(SilcClient client, SilcClientConnection conn, char *hostname, SilcUInt16 port, SilcAuthMethod auth_method, SilcGetAuthMeth completion, void *context) {
    weechat_log_printf("silc_get_auth_method was called");
}

void silc_verify_public_key(SilcClient client, SilcClientConnection conn, SilcConnectionType conn_type, SilcPublicKey public_key, SilcVerifyPublicKey completion, void *context) {
    weechat_log_printf("silc_verify_public_key was called");
}

void silc_ask_passphrase(SilcClient client, SilcClientConnection conn, SilcAskPassphrase completion, void *context) {
    weechat_log_printf("silc_ask_passphrase was called");
}

void silc_key_agreement(SilcClient client, SilcClientConnection conn, SilcClientEntry client_entry, const char *hostname, SilcUInt16 protocol, SilcUInt16 port) {
    weechat_log_printf("silc_key_agreement was called");
}

void silc_ftp(SilcClient client, SilcClientConnection conn, SilcClientEntry client_entry, SilcUInt32 session_id, const char *hostname, SilcUInt16 port) {
    weechat_log_printf("silc_ftp was called");
}
