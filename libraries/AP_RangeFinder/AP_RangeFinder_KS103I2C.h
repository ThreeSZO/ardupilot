// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#ifndef __AP_RANGEFINDER_KS103I2C_H__
#define __AP_RANGEFINDER_KS103I2C_H__

#include "RangeFinder.h"
#include "RangeFinder_Backend.h"

#define AP_RANGE_FINDER_KS103I2C_DEFAULT_ADDR (0xd6 >> 1)	//KS103 Address

#define AP_RANGE_FINDER_KS103I2C_COMMAND_REG 0x02	//command register

#define AP_RANGE_FINDER_KS103I2C_COMMAND_TAKE_RANGE 0xba	//0-11m, return mm, max time 68ms

class AP_RangeFinder_KS103I2C : public AP_RangeFinder_Backend
{
public:
    // constructor
    AP_RangeFinder_KS103I2C(RangeFinder &ranger, uint8_t instance, RangeFinder::RangeFinder_State &_state);

    // static detection function
    static bool detect(RangeFinder &ranger, uint8_t instance);

    // update state
    void update(void);

private:
    // start a reading
    static bool start_reading(void);
    static bool get_reading(uint16_t &reading_mm);
};

#endif	//__AP_RANGEFINDER_KS103I2C_H__