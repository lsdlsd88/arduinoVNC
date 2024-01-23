/*
 * @file VNC_ILI9341.cpp
 * @date 12.05.2015
 * @author Markus Sattler
 *
 * Copyright (c) 2015 Markus Sattler. All rights reserved.
 * This file is part of the VNC client for Arduino.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, a copy can be downloaded from
 * http://www.gnu.org/licenses/gpl.html, or obtained by writing to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *
 */

#include "VNC_config.h"

#ifdef VNC_ILI9341_ALT

#include "VNC.h"

#include <Adafruit_ILI9341.h>
#include "VNC_ILI9341_alt.h"

ILI9341VNC_ALT::ILI9341VNC_ALT(int8_t _CS, int8_t _DC, int8_t _RST, SPIClass * spiClass)
    : Adafruit_ILI9341(spiClass, _CS, _DC, _RST) {
}

bool ILI9341VNC_ALT::hasCopyRect(void) {
    return false;
}

uint32_t ILI9341VNC_ALT::getHeight(void) {
    return static_cast<uint32_t>(Adafruit_ILI9341::_height);
}

uint32_t ILI9341VNC_ALT::getWidth(void) {
    return static_cast<uint32_t>(Adafruit_ILI9341::_width);
}

void ILI9341VNC_ALT::draw_area(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t * data) {
    if(validate_area(x, y, w, h) == false) {
        return;
    }
#ifdef ESP32
    Adafruit_ILI9341::drawRGBBitmap(static_cast<uint16_t>(x), static_cast<uint16_t>(y), (uint16_t *)data, static_cast<uint16_t>(w), static_cast<uint16_t>(h));
#else
    Adafruit_ILI9341::area_update_start(static_cast<uint16_t>(x), static_cast<uint16_t>(y), static_cast<uint16_t>(w), static_cast<uint16_t>(h));
    Adafruit_ILI9341::area_update_data(data, (w * h));
    Adafruit_ILI9341::area_update_end();
#endif
}

void ILI9341VNC_ALT::draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t color) {
    if(validate_area(x, y, w, h) == false) {
        return;
    }
    Adafruit_ILI9341::fillRect(static_cast<uint16_t>(x), static_cast<uint16_t>(y), static_cast<uint16_t>(w), static_cast<uint16_t>(h), ((((color) & 0xff) << 8) | (((color) >> 8))));
}

void ILI9341VNC_ALT::copy_rect(uint32_t src_x, uint32_t src_y, uint32_t dest_x, uint32_t dest_y, uint32_t w, uint32_t h) {
}

void ILI9341VNC_ALT::area_update_start(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    if(validate_area(x, y, w, h) == false) {
        return;
    }
#ifndef ESP32
    Adafruit_ILI9341::area_update_start(static_cast<uint16_t>(x), static_cast<uint16_t>(y), static_cast<uint16_t>(w), static_cast<uint16_t>(h));
#else
    area_x = x;
    area_y = y;
    area_w = w;
    area_h = h;
#endif
}

void ILI9341VNC_ALT::area_update_data(char * data, uint32_t pixel) {
#ifndef ESP32
    Adafruit_ILI9341::area_update_data((uint8_t *)data, pixel);
#else
    if(validate_area(area_x, area_y, area_w, area_h) == false) {
        return;
    }

    Adafruit_ILI9341::drawRGBBitmap(static_cast<uint16_t>(area_x), static_cast<uint16_t>(area_y), (uint16_t *)data, static_cast<uint16_t>(area_w), static_cast<uint16_t>(area_h));
#endif
}

void ILI9341VNC_ALT::area_update_end(void) {
#ifndef ESP32
    Adafruit_ILI9341::area_update_end();
#endif
}

bool ILI9341VNC_ALT::validate_area(uint32_t & x, uint32_t & y, uint32_t & w, uint32_t & h) {
    if(max_height == 0 || max_width == 0) {
        max_height = Adafruit_ILI9341::_height;
        max_width  = Adafruit_ILI9341::_width;
    }

    if(x > max_width || y > max_height) {
        Serial.printf("validate_area exceeds screen bounds: x:%d y:%d w:%d h:%d\n", x, y, w, h);
        return false;
    }

    if(x + w > max_width) {
        w = max_width - x;
    }
    if(y + h > max_height) {
        h = max_height - y;
    }
    return true;
}

#endif
