/*
 * clock.h
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */

#ifndef INC_CLOCK_H_
#define INC_CLOCK_H_

#include "kl_lib_f100.h"


struct time_t {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
};

class clock_t
{
private:
public:
    void Init();
    void SetTime(time_t* Time);
};

extern clock_t Clock;

#endif /* INC_CLOCK_H_ */
