# SPDX-License-Identifier: BSD-2-Clause

# Copyright (c) 2022 Maarten L. Hekkelman

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:

# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.28)

# set the project name
project(salt VERSION 4.1.0 LANGUAGES C CXX)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLang")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wno-multichar"
    )
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

option(USE_BOOST_ASIO "Use the asio library from Boost instead of the non-boost version" OFF)
option(BUILD_DOCUMENTATION "Build manual page" OFF)

set(ZEEP_USE_BOOST_ASIO ${USE_BOOST_ASIO})

include(CPM)
include(GNUInstallDirs)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CMakePackageConfigHelpers)
include(FindPkgConfig)
include(GenerateExportHeader)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${pinch_compile_flags}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-multichar")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# add_compile_definitions(BOOST_ASIO_ENABLE_HANDLER_TRACKING)
add_compile_definitions(BOOST_ASIO_NO_DEPRECATED)

if(USE_BOOST_ASIO)
    add_compile_definitions(USE_BOOST_ASIO)
endif()

# Create a revision file, containing the current git version info
include(VersionString)
write_version_header(${PROJECT_SOURCE_DIR}/src/)

# Optionally use mrc to create resources
find_package(mrc QUIET)

if(MRC_FOUND)
    option(USE_RSRC "Use mrc to create resources" ON)
else()
    message(FATAL_ERROR "salt: Cannot continue since mrc was not found")
endif()

add_compile_options(-pthread)

find_package(mcfp REQUIRED)
find_package(zeem REQUIRED)
find_package(zeep REQUIRED)

add_subdirectory(libpinch EXCLUDE_FROM_ALL)
add_subdirectory(libmgui EXCLUDE_FROM_ALL)

# Generate the manual first
add_subdirectory(manual)

file(GLOB salt_src ${CMAKE_SOURCE_DIR}/src/*.cpp)
# file(GLOB mgui_src ${CMAKE_SOURCE_DIR}/libmgui/src/*.cpp)
# file(GLOB mgui_gtk_src ${CMAKE_SOURCE_DIR}/libmgui/src/Gtk/*.cpp)

list(FILTER mgui_src EXCLUDE REGEX ${CMAKE_SOURCE_DIR}/libmgui/src/MSound.cpp)

add_executable(salt
    ${salt_src}
    ${mgui_src})

set(RESOURCES
    ${CMAKE_SOURCE_DIR}/rsrc/Menus
    ${CMAKE_SOURCE_DIR}/rsrc/Alerts
    ${CMAKE_SOURCE_DIR}/rsrc/templates
    ${CMAKE_SOURCE_DIR}/rsrc/css
    ${CMAKE_SOURCE_DIR}/rsrc/salt.desktop.in
    ${CMAKE_SOURCE_DIR}/rsrc/Dialogs
    ${CMAKE_SOURCE_DIR}/rsrc/strings.xml.nl
    ${CMAKE_SOURCE_DIR}/rsrc/Sounds
    ${CMAKE_SOURCE_DIR}/rsrc/Icons
    ${CMAKE_BINARY_DIR}/manual/salt.1)

target_compile_features(salt PUBLIC cxx_std_20)
target_include_directories(salt
    PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/libmgui/include)
target_compile_definitions(salt PUBLIC USE_RSRC)
target_link_libraries(salt pinch::pinch mcfp::mcfp zeem::zeem zeep::zeep mgui::mgui)

pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4 libcanberra gtk4-x11)
target_link_libraries(salt PkgConfig::GTK4 X11)

mrc_target_resources(salt ${RESOURCES})

set(__EXE__ ${CMAKE_INSTALL_FULL_BINDIR}/salt)
set(__ICON__ ${CMAKE_INSTALL_FULL_DATAROOTDIR}/icons/hicolor/48x48/apps/com.hekkelman.salt.png)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rsrc/salt.desktop.in
    ${CMAKE_CURRENT_BINARY_DIR}/salt.desktop @ONLY)

install(TARGETS salt
    RUNTIME DESTINATION bin)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/salt.desktop
    DESTINATION share/applications
    RENAME com.hekkelman.salt.desktop)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/rsrc/Icons/appicon.png
    RENAME com.hekkelman.salt.png
    DESTINATION share/icons/hicolor/48x48/apps/)

