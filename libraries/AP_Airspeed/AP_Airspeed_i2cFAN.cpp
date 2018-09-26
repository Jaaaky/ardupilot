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

/*
  backend driver for airspeed from a I2C i2cFAN sensor
 */
#include "AP_Airspeed_i2cFAN.h"
//#include <GCS_MAVLink/GCS.h>

//#include <stdio.h>

extern const AP_HAL::HAL &hal;

AP_Airspeed_i2cFAN::AP_Airspeed_i2cFAN(AP_Airspeed &_frontend, uint8_t _instance) :
    AP_Airspeed_Backend(_frontend, _instance)
{
}

// probe and initialise the sensor
bool AP_Airspeed_i2cFAN::init()
{
    const uint8_t address = 0x21;
    bool found = false;

    _dev = hal.i2c_mgr->get_device(get_bus(), address);
    if (_dev) {
        found = true;
    }

    if (!found) {
        return false;
    }

    /*
      this sensor uses zero offset and skips cal
     */
    set_use_zero_offset();
    set_skip_cal();
    set_offset(0);
    
    // drop to 2 retries for runtime
    _dev->set_retries(2);

    _dev->register_periodic_callback(50000,
                                     FUNCTOR_BIND_MEMBER(&AP_Airspeed_i2cFAN::_timer, void));
    return true;
}

// read the values from the sensor. Called at 20Hz
void AP_Airspeed_i2cFAN::_timer()
{
    // read 1 bytes from the sensor
    uint8_t val[4];
    int ret = _dev->transfer(nullptr, 0, &val[0], sizeof(val));

    if (!ret) {
        return;
    }

    if (val[0] == val[1] && val[2] == val[3]) {
    airspeed = val[0]+val[1]*0.1f;
    //_send_command(val[0]);
    }
}

/*
  echo speed
*//*
bool AP_Airspeed_i2cFAN::_send_command(uint8_t S)
{
    uint8_t b[1] {S};
    return _dev->transfer(b, 1, nullptr, 0);
}*/