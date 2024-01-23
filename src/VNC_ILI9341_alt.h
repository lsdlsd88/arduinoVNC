/*
 * @file VNC_ILI9341.h
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

#ifndef VNC_ILI9341_ALT_H_
#define VNC_ILI9341_ALT_H_

#include "VNC_config.h"

#ifdef VNC_ILI9341_ALT

#include <Adafruit_ILI9341.h>
#include "VNC_ILI9341_alt.h"
#include "VNC.h"
#include <SPI.h>

class ILI9341VNC_ALT: public VNCdisplay, public Adafruit_ILI9341 {
    public:
      //  ILI9341VNC(SPIClass *wspi=&SPI,int8_t _CS, int8_t _DC, int8_t _RST);
  ILI9341VNC_ALT(int8_t _CS, int8_t _DC, int8_t _RST =-1,SPIClass *wspi=&SPI);

        bool hasCopyRect(void);

        uint32_t getHeight(void);
        uint32_t getWidth(void);

        void draw_area(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t *data);


        void draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t color);

        void copy_rect(uint32_t src_x, uint32_t src_y, uint32_t dest_x, uint32_t dest_y, uint32_t w, uint32_t h);

        void area_update_start(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
        void area_update_data(char *data, uint32_t pixel);
        void area_update_end(void);

    private:
        uint32_t area_x, area_y, area_w, area_h;
        uint32_t max_height, max_width;
        bool validate_area(uint32_t& x, uint32_t& y, uint32_t& w, uint32_t& h);
};

#endif

#endif /* MARKUS_VNC_ILI9341_H_ */
