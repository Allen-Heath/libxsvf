# Lib(X)SVF  -  A library for implementing SVF and XSVF JTAG players
#
# Copyright (C) 2009  RIEGL Research ForschungsGmbH
# Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

cmake_minimum_required(VERSION 3.15)

project(
  LibXSVF
  VERSION       0.0.0
  LANGUAGES     C CXX
  HOMEPAGE_URL  http://www.clifford.at/libxsvf/
)

option(XSVFTOOL_DUMMY   "Build xsvftool-dummy"    OFF)
option(XSVFTOOL_GPIO    "Build xsvftool-gpio"     OFF)
option(XSVFTOOL_FT232H  "Build xsvftool-ft232h"   OFF)

add_library(xsvf STATIC)

target_include_directories(
  xsvf
  PUBLIC .
)

target_sources(
  xsvf
  PUBLIC    libxsvf.h
  PRIVATE   tap.c play.c scan.c svf.c xsvf.c statename.c memname.c
)

if (XSVFTOOL_DUMMY)
  add_executable(xsvftool-dummy xsvftool-dummy.c)
  target_link_libraries(xsvftool-dummy xsvf)
endif ()

if (XSVFTOOL_GPIO)
  add_executable(xsvftool-gpio xsvftool-gpio.c)
  target_link_libraries(xsvftool-gpio xsvf)
endif ()

if (XSVFTOOL_FT232H)
  add_executable(xsvftool-ft232h xsvftool-ft232h.c)
  target_link_libraries(xsvftool-ft232h xsvf ftdi m)
endif ()
