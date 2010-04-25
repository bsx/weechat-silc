#ifndef SILC_PLUGIN_CRYPTO_H
#define SILC_PLUGIN_CRYPTO_H

int silc_plugin_get_keypair(char *basename, char *passphrase, bool allow_generate, SilcPublicKey *public_key, SilcPrivateKey *private_key);

#endif /* SILC_PLUGIN_CRYPTO_H */
