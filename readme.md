
[![](media/weechat.png)](https://weechat.org/)
![](media/plus128.png)
[![](media/silc.png)](https://en.wikipedia.org/wiki/SILC_(protocol))

A WeeChat [plugin](https://weechat.org/files/doc/stable/weechat_plugin_api.en.html) for silc

[![GitHub version](https://badge.fury.io/gh/bsx%2Fweechat-silc.svg)](https://badge.fury.io/gh/bsx%2Fweechat-silc)
[![GitHub forks](https://img.shields.io/github/forks/bsx/weechat-silc.svg)](https://github.com/bsx/weechat-silc/network)
[![GitHub issues](https://img.shields.io/github/issues/bsx/weechat-silc.svg)](https://github.com/bsx/weechat-silc/issues)
[![GitHub license](https://img.shields.io/badge/license-Apache%202-blue.svg)](https://raw.githubusercontent.com/bsx/weechat-silc/master/license)

# Installation

* Install [weechat](https://weechat.org/download/) either from source or with your favorite package manager
  * Make sure `weechat.pc` is in either `/usr/lib/pkgconfig/` or /usr/local/lib/pkgconfig/` 
* Install [the silc tookit](https://sourceforge.net/projects/silc/files/silc/toolkit/) either from source or your favorite package manager
  * Make sure `silcclient.pc` is in either `/usr/lib/pkgconfig/` or /usr/local/lib/pkgconfig/` 
* Clone the latest version of this plugin
```bash
git clone git@github.com:bsx/weechat-silc.git
```
* Make a build directory and run `cmake` and `make`
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

If you get a CMake Error that either module `silccliient` or `weemake` were not found 

```
-- checking for module 'weechat>=1.5'
--   package 'weechat>=1.5' not found
CMake Error at /usr/share/cmake/Modules/FindPkgConfig.cmake:279 (message):
  A required package was not found
Call Stack (most recent call first):
  /usr/share/cmake/Modules/FindPkgConfig.cmake:333 (_pkg_check_modules_internal)
  CMakeLists.txt:14 (pkg_check_modules)

-- checking for module 'silcclient'
--   package 'silcclient' not found
CMake Error at /usr/share/cmake/Modules/FindPkgConfig.cmake:279 (message):
  A required package was not found
Call Stack (most recent call first):
  /usr/share/cmake/Modules/FindPkgConfig.cmake:333 (_pkg_check_modules_internal)
  CMakeLists.txt:15 (pkg_check_modules)
```

You either don't have the appropiate `.pc` files in their right place or you might need to set your `PKG_CONFIG_PATH`
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

