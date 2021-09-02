/*
  variant.cpp - Set static flash identification for IoTa System

  https://poemtechnology.com

  Copyright (c) 2017 Konekt, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "variant.h"

#ifdef __cplusplus
extern "C" {
#endif

    // When working one a new version, set the version in the project settings for the
    // compiler, execute a "clean" and then build.
    // The variant does not build on a rebuild, but that is OK as long as the above step
    // is done whenever the build version changes.

    __attribute__((used)) const hologram_flash_id_t __attribute__((section (".idSection"))) FLASH_ID  __attribute__ ((aligned (4))) = {
        0x30505041, //APP0
        0x324D5044, //DPM2
        VERSION_MAJOR,
        VERSION_MINOR,
        VERSION_REVISION,
        0, // padding
        0x4F4C4F48, // 'HOLO' (byte-swapped)
        "POEM TECHNOLOGY",
        "IOTA",
        "FIRMWARE",
        "SYSTEM",
        "SYSTEM CUSTOM " STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "."  STR(VERSION_REVISION)
    };

#ifdef __cplusplus
}
#endif

void initVariant(void)
{
}
