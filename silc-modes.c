#include "weechat-plugin.h"
#include "silc.h"
#include "silcclient.h"

#include "silc-plugin.h"

char *format_channel_mode(SilcUInt32 mode) {
    char *buffer;
    int i = 1;

    if (mode) {
        buffer = malloc(15);
        buffer[0] = '+';
        if (mode & SILC_CHANNEL_MODE_PRIVATE) {
            buffer[i] = 'p';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_SECRET) {
            buffer[i] = 's';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_PRIVKEY) {
            buffer[i] = 'k';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_INVITE) {
            buffer[i] = 'i';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_TOPIC) {
            buffer[i] = 't';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_ULIMIT) {
            buffer[i] = 'l';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_PASSPHRASE) {
            buffer[i] = 'a';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_CIPHER) {
            buffer[i] = 'c';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_HMAC) {
            buffer[i] = 'h';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_FOUNDER_AUTH) {
            buffer[i] = 'f';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_SILENCE_USERS) {
            buffer[i] = 'm';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_SILENCE_OPERS) {
            buffer[i] = 'M';
            i++;
        }
        if (mode & SILC_CHANNEL_MODE_CHANNEL_AUTH) {
            buffer[i] = 'C';
            i++;
        }
        buffer[i] = '\0';
        return buffer;
    }
    return NULL;
}

char *format_user_mode(SilcUInt32 mode) {
    char *buffer;
    int i = 1;

    if (mode) {
        buffer = malloc(15);
        buffer[0] = '+';
        if (mode & SILC_UMODE_SERVER_OPERATOR) {
            buffer[i] = 's';
            i++;
        }
        if (mode & SILC_UMODE_ROUTER_OPERATOR) {
            buffer[i] = 'r';
            i++;
        }
        if (mode & SILC_UMODE_GONE) {
            buffer[i] = 'g';
            i++;
        }
        if (mode & SILC_UMODE_INDISPOSED) {
            buffer[i] = 'i';
            i++;
        }
        if (mode & SILC_UMODE_BUSY) {
            buffer[i] = 'b';
            i++;
        }
        if (mode & SILC_UMODE_PAGE) {
            buffer[i] = 'p';
            i++;
        }
        if (mode & SILC_UMODE_HYPER) {
            buffer[i] = 'h';
            i++;
        }
        if (mode & SILC_UMODE_ROBOT) {
            buffer[i] = 't';
            i++;
        }
        if (mode & SILC_UMODE_ANONYMOUS) {
            buffer[i] = 'A';
            i++;
        }
        if (mode & SILC_UMODE_BLOCK_PRIVMSG) {
            buffer[i] = 'P';
            i++;
        }
        if (mode & SILC_UMODE_DETACHED) {
            buffer[i] = 'd';
            i++;
        }
        if (mode & SILC_UMODE_REJECT_WATCHING) {
            buffer[i] = 'w';
            i++;
        }
        if (mode & SILC_UMODE_BLOCK_INVITE) {
            buffer[i] = 'I';
            i++;
        }
        buffer[i] = '\0';
        return buffer;
    }
    return NULL;
}

char *format_channeluser_mode(SilcUInt32 mode) {
    char *buffer;
    int i = 1;

    if (mode) {
        buffer = malloc(8);
        buffer[0] = '+';
        if (mode & SILC_CHANNEL_UMODE_CHANFO) {
            buffer[i] = 'f';
            i++;
        }
        if (mode & SILC_CHANNEL_UMODE_CHANOP) {
            buffer[i] = 'o';
            i++;
        }
        if (mode & SILC_CHANNEL_UMODE_BLOCK_MESSAGES) {
            buffer[i] = 'b';
            i++;
        }
        if (mode & SILC_CHANNEL_UMODE_BLOCK_MESSAGES_USERS) {
            buffer[i] = 'u';
            i++;
        }
        if (mode & SILC_CHANNEL_UMODE_BLOCK_MESSAGES_ROBOTS) {
            buffer[i] = 'r';
            i++;
        }
        if (mode & SILC_CHANNEL_UMODE_QUIET) {
            buffer[i] = 'q';
            i++;
        }
        buffer[i] = '\0';
        return buffer;
    }
    return NULL;
}
