
[![](media/weechat.png)](https://weechat.org/)
![](media/plus128.png)
[![](media/silc.png)](//https://en.wikipedia.org/wiki/SILC_(protocol))

A WeeChat [plugin](https://weechat.org/files/doc/stable/weechat_plugin_api.en.html) allowing you to use you to use silc

[![Total Downloads](https://poser.pugx.org/bsx/weechat-silc/downloads)](https://packagist.org/packages/bsx/weechat-silc)
[![Latest Stable Version](https://poser.pugx.org/bsx/weechat-silc/v/stable)](https://packagist.org/packages/bsx/weechat-silc)
[![License](https://poser.pugx.org/bsx/weechat-silc/license)](https://packagist.org/packages/bsx/weechat-silc)

# Installation

* Install [weechat](https://weechat.org/download/) either from source or with your favorite manager
  * Make sure `weechat.pc` is in either `/usr/lib/pkgconfig/` or /usr/local/lib/pkgconfig/` 
* Install [the silc tookit](https://sourceforge.net/projects/silc/files/silc/toolkit/) either from source or your favorite package manager
  * Make sure `silcclient.pc` is in either `/usr/lib/pkgconfig/` or /usr/local/lib/pkgconfig/` 
* Clone the latest version of this plugin
```bash
git clone git@github.com:bsx/weechat-silc.git
```
* Make a build directory and cmake/make
```bash
cd weechat-silc
mkdir build
cd build
cmake ..
make
```

* Install the now built `libweechatsilc.so`
```bash
sudo make install
```

# Troubleshooting

If you get a CMake Error that either module `silccliient` or `weemake` you might need to set your `PKG_CONFIG_PATH` so it can find both of the `.pc` files
```bash
export PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/local/lib/pkgconfig:/usr/lib64/pkgconfig:/usr/share/pkgconfig
```

# Usage


## What you can do 

* connecting to a silc server: /silc connect <servername>
* joining a channel: /silc join <channelname>
* talking in a channel
* nicklist and modes should work as well
* private messages: /silc msg <nick> [<message>]
* disconnecting from a server: /silc disconnect


## What you can't do yet (PR's very much appreciated)

* leaving a channel (you can close the buffer, but that will just lead to madness)
* changing channel or user modes
* setting a channel topic
* nick changes (both yours and others)
* kicking people and being kicked (see: leaving a channel)
* password prompts for keys (currently just loads/generates a key in the current working directory)

References
----------
* http://weechat.org/files/doc/stable/weechat_plugin_api.en.html
* http://www.silcnet.org/docs/toolkit/

