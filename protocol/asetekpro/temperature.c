/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017,2018  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenCorsairLink.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>
#include "../../lowlevel/asetek.h"
#include "../../device.h"
#include "../../driver.h"
#include "../../print.h"

int corsairlink_asetekpro_tempsensorscount(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            uint8_t *temperature_sensors_count)
{
    int rr = 0;
    // not defined - set default value of 3
    *(temperature_sensors_count) = 1;
    return rr;
}

int corsairlink_asetekpro_temperature(struct corsair_device_info *dev,
            struct libusb_device_handle *handle, uint8_t selector,
            char *temperature, uint8_t temperature_str_len)
{
    int rr;
    uint8_t response[32];
    uint8_t commands[32] ;
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0xa9;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 1);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 6);

    msg_debug("%02X %02X %02X %02X %02X\n", response[0], response[1],
                response[2], response[3], response[4]);

    if (response[0] != 0xa9 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    // *(temperature) = (response[3]<<8) + response[4];
    snprintf(temperature, temperature_str_len, "%d.%d C",
                response[3], response[4]);

    return rr;
}
