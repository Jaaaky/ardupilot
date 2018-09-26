/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

/*
  backend driver for airspeed from I2C
 */

#include <AP_HAL/AP_HAL.h>
#include <AP_Param/AP_Param.h>
#include <AP_HAL/utility/OwnPtr.h>
#include <AP_HAL/I2CDevice.h>
//#include <utility>

#include "AP_Airspeed_Backend.h"
#include "AP_Airspeed.h"

/*
 * Thanks to PX4 for registers definitions
 */

class AP_Airspeed_i2cFAN : public AP_Airspeed_Backend
{
public:
    AP_Airspeed_i2cFAN(AP_Airspeed &frontend, uint8_t _instance);
    ~AP_Airspeed_i2cFAN(void) {}

    // probe and initialise the sensor
    bool init() override;

    // return the current differential_pressure in Pascal
    bool get_differential_pressure(float &pressure) override { pressure = airspeed; return true; };

    // return the current temperature in degrees C, if available
    bool get_temperature(float &temperature) override { temperature = 0; return false; };

private:
    void _timer();
    bool _send_command(uint8_t);

    float airspeed;

    AP_HAL::OwnPtr<AP_HAL::I2CDevice> _dev;
};
