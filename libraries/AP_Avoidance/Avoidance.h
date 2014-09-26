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

#ifndef __AP_AVOIDANCE_H__
#define __AP_AVOIDANCE_H__

#include <AP_Common.h>
#include <AP_HAL.h>

//Maximum number of avoidance instances available on this platform
#define AVOIDANCE_MAX_INSTANCES 4

class Avoidance
{
public:
   Avoidance(void);

   struct Avoidance_State
   {
      uint8_t  instance;         //the instance number of this Avoidance device
      uint16_t distance_cm;      //distance: in cm
      bool     healthy;          //sensor is communicating correctly
   };

   AP_Int8  _addr[AVOIDANCE_MAX_INSTANCES];
   AP_Int8  _cmd[AVOIDANCE_MAX_INSTANCES];
   AP_Int16 _min_distance_cm[AVOIDANCE_MAX_INSTANCES];
   AP_Int16 _max_distance_cm[AVOIDANCE_MAX_INSTANCES];

   //detect and initialise any available avoidance devices
   void init(void);

   //update state of all avoidance device. Should be called at around 10Hz from main loop
   void update(void);

#define  _Avoidance_STATE(instance) state[instance]

   uint16_t distance_cm(uint8_t instance) const {
      return _Avoidance_STATE(instance).distance_cm;
   }

private:
   Avoidance_State state[AVOIDANCE_MAX_INSTANCES];
}


#endif   //__AP_AVOIDANCE_H__