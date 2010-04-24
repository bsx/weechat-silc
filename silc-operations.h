#ifndef SILC_OPERATIONS_H
#define SILC_OPERATIONS_H

void silc_say(SilcClient client, SilcClientConnection conn, SilcClientMessageType type, char *msg, ...);
void silc_channel_message(SilcClient client, SilcClientConnection conn, SilcClientEntry sender,
        SilcChannelEntry channel, SilcMessagePayload payload, SilcChannelPrivateKey key,
        SilcMessageFlags flags, const unsigned char *message, SilcUInt32 message_len);
void silc_private_message(SilcClient client, SilcClientConnection conn, SilcClientEntry sender,
        SilcMessagePayload payload, SilcMessageFlags flags, const unsigned char *message, SilcUInt32 message_len);
void silc_notify(SilcClient client, SilcClientConnection conn, SilcNotifyType type, ...);
void silc_command(SilcClient client, SilcClientConnection conn, SilcBool success, SilcCommand command, SilcStatus status, SilcUInt32 argc, unsigned char **argv);
void silc_command_reply(SilcClient client, SilcClientConnection conn, SilcCommand command, SilcStatus status, SilcStatus error, va_list ap);
void silc_get_auth_method(SilcClient client, SilcClientConnection conn, char *hostname, SilcUInt16 port, SilcAuthMethod auth_method, SilcGetAuthMeth completion, void *context);
void silc_verify_public_key(SilcClient client, SilcClientConnection conn, SilcConnectionType conn_type, SilcPublicKey public_key, SilcVerifyPublicKey completion, void *context);
void silc_ask_passphrase(SilcClient client, SilcClientConnection conn, SilcAskPassphrase completion, void *context);
void silc_key_agreement(SilcClient client, SilcClientConnection conn, SilcClientEntry client_entry, const char *hostname, SilcUInt16 protocol, SilcUInt16 port);
void silc_ftp(SilcClient client, SilcClientConnection conn, SilcClientEntry client_entry, SilcUInt32 session_id, const char *hostname, SilcUInt16 port);

SilcClientOperations ops = {
  silc_say,
  silc_channel_message,
  silc_private_message,
  silc_notify,
  silc_command,
  silc_command_reply,
  silc_get_auth_method,
  silc_verify_public_key,
  silc_ask_passphrase,
  silc_key_agreement,
  silc_ftp
};

#endif /* SILC_OPERATIONS_H */
