# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0
#
# This CMake file is picked by the Zephyr build system because it is defined
# as the module CMake entry point (see zephyr/module.yml).
cmake_minimum_required(VERSION 3.22)

set(BOARD my_esp32_devkitc_wroom/esp32/procpu)
set(BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(wicked_dragon_fw)

message(STATUS "ZEPHYR_BASE = ${ZEPHYR_BASE}")
target_include_directories(app PRIVATE ${ZEPHYR_BASE}/include/generated/zephyr)
 zephyr_syscall_include_directories(include)
 zephyr_include_directories(include)
#
#set(BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/boards)


#add_subdirectory(drivers)
#add_subdirectory(boards/xtensa)
#add_subdirectory(lib)
#project(app LANGUAGES C)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(app PRIVATE src/main.c)
