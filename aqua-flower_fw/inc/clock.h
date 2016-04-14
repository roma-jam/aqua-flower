/*
 * clock.h
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */

#ifndef INC_CLOCK_H_
#define INC_CLOCK_H_

#include "kl_lib_f100.h"
#include "lcd.h"

struct time_t {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    void Update()
    {
        seconds++;
    }
};

class clock_t
{
private:
    void EnterConfMode()
    {
        while(!(RTC->CRL && RTC_CRL_RTOFF)); // Wait RTOFF
        RTC->CRL |= RTC_CRL_CNF; // Enter conf mode
    }
    void LeaveConfMode() {
        RTC->CRL &= ~RTC_CRL_CNF; // Leave conf mode
        while(!(RTC->CRL && RTC_CRL_RTOFF)); // Wait RTOFF
    }

    void Sync() {
        while(!(RTC->CRL && RTC_CRL_RSF));
    }
public:
    time_t CurrentTime, CurrentAlarm;

    void Init();
    void SetTime(time_t* Time);
    void SetAlarm(time_t* Time);
    void Display();

    void IrqHandler();
    void AlarmHandler();
};

extern clock_t Clock;

#endif /* INC_CLOCK_H_ */
