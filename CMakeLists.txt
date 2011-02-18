PROJECT(weechat-silc)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7)
IF(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0005 OLD)
ENDIF(COMMAND cmake_policy)

INCLUDE(CheckFunctionExists)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFiles)

FIND_PACKAGE(PkgConfig QUIET)
pkg_check_modules(WEECHAT REQUIRED weechat)
pkg_check_modules(SILC REQUIRED silcclient)

execute_process(COMMAND "git log | head -n1 | cut -d' ' -f2" OUTPUT_VARIABLE GITVERSION)

# includes

SET(WEECHATSILC_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${SILC_INCLUDE_DIRS}
  ${WEECHAT_INCLUDE_DIRS})

SET(CMAKE_REQUIRED_INCLUDES ${WEECHATSILC_INCLUDE_DIRS})
SET(CMAKE_REQUIRED_DEFINITIONS -DHAVE_CONFIG_H)

FOREACH(X ${SILC_LDFLAGS})
  SET(MAIN_LDFLAGS "${MAIN_LDFLAGS} ${X}")
ENDFOREACH(X ${SILC_LDFLAGS})

include_directories(${WEECHATSILC_INCLUDE_DIRS})

ADD_DEFINITIONS(-DHAVE_CONFIG_H -Wall -g -DGITVERSION="\\"${GITVERSION}"\\")

ADD_LIBRARY(weechatsilc SHARED silc-plugin.c silc-operations.c silc-commands.c
    silc-config.c silc-keys.c silc-connections.c silc-bar.c silc-modes.c)
TARGET_LINK_LIBRARIES(weechatsilc ${SILC_LIBRARIES})
SET_TARGET_PROPERTIES(weechatsilc PROPERTIES 
  OUTPUT_NAME "weechatsilc"
  LINK_FLAGS "${MAIN_LDFLAGS}")

INSTALL(TARGETS weechatsilc DESTINATION lib${LIB_SUFFIX}/weechat/plugins)
