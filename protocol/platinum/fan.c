/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017-2019  Sean Nelson <audiohacked@gmail.com>

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

#include "device.h"
#include "driver.h"
#include "logic/print.h"
#include "logic/options.h"
#include "lowlevel/platinum.h"
#include "protocol/platinum.h"
#include "protocol/crc.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_platinum_fan_mode_read_rpm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0x00] = 0x3F;
    commands[0x01] = 0x78;

    commands[0x3F] = crc8ccitt(commands+1, 62);

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );

    ctrl->max_speed = 0xFFFF; // 

    switch (ctrl->fan_count) {
        case 1:
            msg_debug2( "Speed: %02X %02X\n", response[0x0F], response[0x10] );
            // msg_debug2( "Max Speed: %02X %02X\n", response[9], response[8] );
            ctrl->speed_rpm = ( response[0x10] << 8 ) + response[0x0F];
            // ctrl->max_speed = ( response[9] << 8 ) + response[8];
            break;
        case 2:
            msg_debug2( "Speed: %02X %02X\n", response[0x16], response[0x17] );
            // msg_debug2( "Max Speed: %02X %02X\n", response[9], response[8] );
            ctrl->speed_rpm = ( response[0x17] << 8 ) + response[0x16];
            // ctrl->max_speed = ( response[9] << 8 ) + response[8];
            break;
        default:
            msg_debug2("Fan Index out of bounds");
            break;        
    }

    return rr;
}

int
corsairlink_platinum_fan_speed(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0x00] = 0x3F;
    commands[0x01] = 0x78;

    commands[0x3F] = crc8ccitt(commands+1, 62);

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );

    ctrl->max_speed = 0xFFFF; // 

    switch (ctrl->fan_count) {
        case 1:
            msg_debug2( "Speed: %02X %02X\n", response[0x0F], response[0x10] );
            // msg_debug2( "Max Speed: %02X %02X\n", response[9], response[8] );
            ctrl->speed_rpm = ( response[0x10] << 8 ) + response[0x0F];
            // ctrl->max_speed = ( response[9] << 8 ) + response[8];
            break;
        case 2:
            msg_debug2( "Speed: %02X %02X\n", response[0x16], response[0x17] );
            // msg_debug2( "Max Speed: %02X %02X\n", response[9], response[8] );
            ctrl->speed_rpm = ( response[0x17] << 8 ) + response[0x16];
            // ctrl->max_speed = ( response[9] << 8 ) + response[8];
            break;
        default:
            msg_debug2("Fan Index out of bounds");
            break;        
    }

    return rr;
}
