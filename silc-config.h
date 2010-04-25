#ifndef PLUGIN_CONFIG_H
#define PLUGIN_CONFIG_H

struct t_config_file *config_file;

struct t_config_option *option_default_username;
struct t_config_option *option_default_realname;
struct t_config_option *option_default_nick;

int silc_plugin_config_init();

int silc_plugin_config_write();

int silc_plugin_config_read();

void silc_plugin_config_free();

#endif /* PLUGIN_CONFIG_H */
