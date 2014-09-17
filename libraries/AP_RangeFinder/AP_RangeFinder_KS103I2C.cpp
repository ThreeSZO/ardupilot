// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
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
 *       AP_RangeFinder_KS103I2C.cpp - Arduino Library for KS103 I2C sonar
 *       Code by 3601. EulerSpace.com
 *
 *       datasheet:
 *
 *       Sensor should be connected to the I2C port
 */

 #include "AP_RangeFinder_KS103I2C.h"
 #include <AP_HAL.h>

 extern const AP_HAL::HAL& hal;

 AP_RangeFinder_KS103I2C::AP_RangeFinder_KS103I2C(RangeFinder &_ranger, uint8_t instance, RangeFinder::RangeFinder_State &_state) :
    AP_RangeFinder_Backend(_ranger, instance, _state)
{
}

bool AP_RangeFinder_KS103I2C::detect(RangeFinder &_ranger, uint8_t instance)
{
   if(!start_reading()){
      return false;
   }

   hal.scheduler->delay(50);
   uint16_t reading_cm;
   return get_reading(reading_cm);
}

bool AP_RangeFinder_KS103I2C::start_reading()
{
   // get pointer to i2c bus semaphore
    AP_HAL::Semaphore* i2c_sem = hal.i2c->get_semaphore();

    // exit immediately if we can't take the semaphore
    if (i2c_sem == NULL || !i2c_sem->take(1)) {
        return false;
    }

    uint8_t tosend[1] =
        { AP_RANGE_FINDER_KS103I2C_COMMAND_TAKE_RANGE };

   if(hal.i2c->writeRegistersWithDelay(AP_RANGE_FINDER_KS103I2C_DEFAULT_ADDR,
                              AP_RANGE_FINDER_KS103I2C_COMMAND_REG,
                              1,
                              tosend) != 0) {
      i2c_sem->give();
      return false;
   }

   i2c_sem->give();

   return true;
}

bool AP_RangeFinder_KS103I2C::get_reading(uint16_t &reading_cm)
{
   uint8_t buff[2];
   uint16_t reading_mm;

   AP_HAL::Semaphore* i2c_sem = hal.i2c->get_semaphore();

   // exit immediately if we can't take the semaphore
    if (i2c_sem == NULL || !i2c_sem->take(1)) {
        return false;
    }
   if (hal.i2c->readRegistersWithDelay(AP_RANGE_FINDER_KS103I2C_DEFAULT_ADDR,
                              AP_RANGE_FINDER_KS103I2C_COMMAND_REG, 2, &buff[0]) != 0) {
      i2c_sem->give();
      return false;
   }

   i2c_sem->give();

   reading_mm = ((uint16_t)buff[0]) << 8 | buff[1];
   reading_cm = reading_mm / 10;

   start_reading();

   return true;
}

void AP_RangeFinder_KS103I2C::update(void)
{
   state.healthy = get_reading(state.distance_cm);
}