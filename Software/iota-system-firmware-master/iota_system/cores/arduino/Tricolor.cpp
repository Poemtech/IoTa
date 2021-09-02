/*
  Tricolor.cpp - Class controls the RGB LED..

  https://hologram.io

  Copyright (c) 2016 Konekt, Inc.  All rights reserved.

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

#include "Tricolor.h"
#include "wiring_analog.h"
#include "WMath.h"
#include "wiring_constants.h"
#include "wiring_digital.h"
#include <ctype.h>
#include <cstring>

typedef struct {
    char name[24];
    uint32_t value;
}color_table_entry;

static const color_table_entry COLOR_TABLE[] = {
    {"MAROON", 0x800000},
    {"DARK_RED", 0x8B0000},
    {"BROWN", 0xA52A2A},
    {"FIREBRICK", 0xB22222},
    {"CRIMSON", 0xDC143C},
    {"RED", 0xFF0000},
    {"TOMATO", 0xFF6347},
    {"CORAL", 0xFF7F50},
    {"INDIAN_RED", 0xCD5C5C},
    {"LIGHT_CORAL", 0xF08080},
    {"DARK_SALMON", 0xE9967A},
    {"SALMON", 0xFA8072},
    {"LIGHT_SALMON", 0xFFA07A},
    {"ORANGE_RED", 0xFF4500},
    {"DARK_ORANGE", 0xFF8C00},
    {"ORANGE", 0xFFA500},
    {"GOLD", 0xFFD700},
    {"DARK_GOLDEN_ROD", 0xB8860B},
    {"GOLDEN_ROD", 0xDAA520},
    {"PALE_GOLDEN_ROD", 0xEEE8AA},
    {"DARK_KHAKI", 0xBDB76B},
    {"KHAKI", 0xF0E68C},
    {"OLIVE", 0x808000},
    {"YELLOW", 0xFFFF00},
    {"YELLOW_GREEN", 0x9ACD32},
    {"DARK_OLIVE_GREEN", 0x556B2F},
    {"OLIVE_DRAB", 0x6B8E23},
    {"LAWN_GREEN", 0x7CFC00},
    {"CHART_REUSE", 0x7FFF00},
    {"GREEN_YELLOW", 0xADFF2F},
    {"DARK_GREEN", 0x006400},
    {"GREEN", 0x008000},
    {"FOREST_GREEN", 0x228B22},
    {"LIME", 0x00FF00},
    {"LIME_GREEN", 0x32CD32},
    {"LIGHT_GREEN", 0x90EE90},
    {"PALE_GREEN", 0x98FB98},
    {"DARK_SEA_GREEN", 0x8FBC8F},
    {"MEDIUM_SPRING_GREEN", 0x00FA9A},
    {"SPRING_GREEN", 0x00FF7F},
    {"SEA_GREEN", 0x2E8B57},
    {"MEDIUM_AQUA_MARINE", 0x66CDAA},
    {"MEDIUM_SEA_GREEN", 0x3CB371},
    {"LIGHT_SEA_GREEN", 0x20B2AA},
    {"DARK_SLATE_GRAY", 0x2F4F4F},
    {"TEAL", 0x008080},
    {"DARK_CYAN", 0x008B8B},
    {"AQUA", 0x00FFFF},
    {"CYAN", 0x00FFFF},
    {"LIGHT_CYAN", 0xE0FFFF},
    {"DARK_TURQUOISE", 0x00CED1},
    {"TURQUOISE", 0x40E0D0},
    {"MEDIUM_TURQUOISE", 0x48D1CC},
    {"PALE_TURQUOISE", 0xAFEEEE},
    {"AQUA_MARINE", 0x7FFFD4},
    {"POWDER_BLUE", 0xB0E0E6},
    {"CADET_BLUE", 0x5F9EA0},
    {"STEEL_BLUE", 0x4682B4},
    {"CORN_FLOWER_BLUE", 0x6495ED},
    {"DEEP_SKY_BLUE", 0x00BFFF},
    {"DODGER_BLUE", 0x1E90FF},
    {"LIGHT_BLUE", 0xADD8E6},
    {"SKY_BLUE", 0x87CEEB},
    {"LIGHT_SKY_BLUE", 0x87CEFA},
    {"MIDNIGHT_BLUE", 0x191970},
    {"NAVY", 0x000080},
    {"DARK_BLUE", 0x00008B},
    {"MEDIUM_BLUE", 0x0000CD},
    {"BLUE", 0x0000FF},
    {"ROYAL_BLUE", 0x4169E1},
    {"BLUE_VIOLET", 0x8A2BE2},
    {"INDIGO", 0x4B0082},
    {"DARK_SLATE_BLUE", 0x483D8B},
    {"SLATE_BLUE", 0x6A5ACD},
    {"MEDIUM_SLATE_BLUE", 0x7B68EE},
    {"MEDIUM_PURPLE", 0x9370DB},
    {"DARK_MAGENTA", 0x8B008B},
    {"DARK_VIOLET", 0x9400D3},
    {"DARK_ORCHID", 0x9932CC},
    {"MEDIUM_ORCHID", 0xBA55D3},
    {"PURPLE", 0x800080},
    {"THISTLE", 0xD8BFD8},
    {"PLUM", 0xDDA0DD},
    {"VIOLET", 0xEE82EE},
    {"MAGENTA", 0xFF00FF},
    {"ORCHID", 0xDA70D6},
    {"MEDIUM_VIOLET_RED", 0xC71585},
    {"PALE_VIOLET_RED", 0xDB7093},
    {"DEEP_PINK", 0xFF1493},
    {"HOT_PINK", 0xFF69B4},
    {"LIGHT_PINK", 0xFFB6C1},
    {"PINK", 0xFFC0CB},
    {"ANTIQUE_WHITE", 0xFAEBD7},
    {"BEIGE", 0xF5F5DC},
    {"BISQUE", 0xFFE4C4},
    {"BLANCHED_ALMOND", 0xFFEBCD},
    {"WHEAT", 0xF5DEB3},
    {"CORN_SILK", 0xFFF8DC},
    {"LEMON_CHIFFON", 0xFFFACD},
    {"LIGHT_GOLDEN_ROD_YELLOW", 0xFAFAD2},
    {"LIGHT_YELLOW", 0xFFFFE0},
    {"SADDLE_BROWN", 0x8B4513},
    {"SIENNA", 0xA0522D},
    {"CHOCOLATE", 0xD2691E},
    {"PERU", 0xCD853F},
    {"SANDY_BROWN", 0xF4A460},
    {"BURLY_WOOD", 0xDEB887},
    {"TAN", 0xD2B48C},
    {"ROSY_BROWN", 0xBC8F8F},
    {"MOCCASIN", 0xFFE4B5},
    {"NAVAJO_WHITE", 0xFFDEAD},
    {"PEACH_PUFF", 0xFFDAB9},
    {"MISTY_ROSE", 0xFFE4E1},
    {"LAVENDER_BLUSH", 0xFFF0F5},
    {"LINEN", 0xFAF0E6},
    {"OLD_LACE", 0xFDF5E6},
    {"PAPAYA_WHIP", 0xFFEFD5},
    {"SEA_SHELL", 0xFFF5EE},
    {"MINT_CREAM", 0xF5FFFA},
    {"SLATE_GRAY", 0x708090},
    {"LIGHT_SLATE_GRAY", 0x778899},
    {"LIGHT_STEEL_BLUE", 0xB0C4DE},
    {"LAVENDER", 0xE6E6FA},
    {"FLORAL_WHITE", 0xFFFAF0},
    {"ALICE_BLUE", 0xF0F8FF},
    {"GHOST_WHITE", 0xF8F8FF},
    {"HONEYDEW", 0xF0FFF0},
    {"IVORY", 0xFFFFF0},
    {"AZURE", 0xF0FFFF},
    {"SNOW", 0xFFFAFA},
    {"BLACK", 0x000000},
    {"DIM_GRAY", 0x696969},
    {"GRAY", 0x808080},
    {"DARK_GRAY", 0xA9A9A9},
    {"SILVER", 0xC0C0C0},
    {"LIGHT_GRAY", 0xD3D3D3},
    {"GAINSBORO", 0xDCDCDC},
    {"WHITE_SMOKE", 0xF5F5F5},
    {"WHITE", 0xFFFFFF},
};

Tricolor::Tricolor(uint32_t r, uint32_t g, uint32_t b, bool invert)
: rpin(r), gpin(g), bpin(b), enabled(false), r(0), g(0), b(0), invert(invert)
{
}

void Tricolor::enable(bool en)
{
    enabled = en;
    on(r,g,b);
}

void Tricolor::off()
{
    on(0, 0, 0);
}

void Tricolor::set(uint32_t pin, uint8_t value)
{
    if(value == 0) {
        pinMode(pin, DISABLE);
    } else if(value == 255) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, invert ? LOW : HIGH);
    } else {
        analogWrite(pin, invert ? 255-value : value);
    }
}

void Tricolor::on(uint8_t r, uint8_t g, uint8_t b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    set(rpin, enabled ? r : 0);
    set(gpin, enabled ? g : 0);
    set(bpin, enabled ? b : 0);
}

void Tricolor::on(ColorRGB color)
{
    on((uint32_t)color);
}

void Tricolor::on(uint32_t rgb)
{
    on((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF,  rgb & 0xFF);
}

void Tricolor::rand()
{
    on((uint32_t)random(1, 0x1000000));
}

void Tricolor::setR(uint8_t r) {
    on(r, this->g, this->b);
}

void Tricolor::setG(uint8_t g) {
    on(this->r, g, this->b);
}

void Tricolor::setB(uint8_t b) {
    on(this->r, this->g, b);
}

bool Tricolor::on(const char* name) {
    char cname[24];
    for(int i=0; i<23; i++) {
        cname[i] = toupper(name[i]);
    }
    cname[23] = 0;
    for(int i=0; i<sizeof(COLOR_TABLE) / sizeof(color_table_entry); i++) {
        if(strcmp(cname, COLOR_TABLE[i].name) == 0) {
            on(COLOR_TABLE[i].value);
            return true;
        }
    }
    return false;
}
