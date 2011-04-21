#ifndef __SILC_NICKLIST_H
#define __SILC_NICKLIST_H 1

struct t_gui_nick *silc_nicklist_add(SilcChannelUser user);
void silc_nicklist_remove(SilcChannelEntry channel_entry, SilcClientEntry user_client);
void silc_nicklist_update(SilcChannelUser user);

#endif /* __SILC_NICKLIST_H */

