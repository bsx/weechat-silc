#ifndef SILC_COMMANDS_H
#define SILC_COMMANDS_H

int command_silc(void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol);
int command_sconnect(void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol);

#endif /* SILC_COMMANDS_H */
