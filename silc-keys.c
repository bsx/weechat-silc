#include "silc.h"
#include "silcclient.h"

#include "silc-keys.h"

int silc_plugin_get_keypair(char *basename, char *passphrase, bool allow_generate, SilcPublicKey *public_key, SilcPrivateKey *private_key) {
    char *pub_name, *priv_name;

    int length = strlen(basename);
    pub_name = malloc(length + 5);
    priv_name = malloc(length + 5);

    strncpy(pub_name, basename, length);
    strncpy(priv_name, basename, length);
    strncat(pub_name, ".pub", 4);
    strncat(priv_name, ".prv", 4);

    if (!silc_load_key_pair(pub_name, priv_name, passphrase, public_key, private_key)) {
        if (!allow_generate) {
            return -1;
        }
        if (!silc_create_key_pair("rsa", 2048, pub_name, priv_name, NULL, passphrase, public_key, private_key, FALSE)) {
            return -1;
        }
    }
    return 0;
}
