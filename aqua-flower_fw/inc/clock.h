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

#define SEC_PER_MIN         59
#define MIN_PER_HOUR        59
#define SEC_PER_HOUR        (SEC_PER_MIN*MIN_PER_HOUR)
#define HOUR_PER_DAY        23

struct time_t {
    int8_t hours;
    int8_t minutes;
    int8_t seconds;

    void operator = (const time_t Time) { hours = Time.hours; minutes = Time.minutes; seconds = Time.seconds; }
    bool Update()
    {
        seconds++;
        if(seconds >= SEC_PER_MIN)
        {
            seconds = 0;
            minutes++;
            if(minutes >= MIN_PER_HOUR)
            {
                minutes = 0;
                hours++;
                if(hours == HOUR_PER_DAY)
                    hours = 0;
                return true;
            }
            return true;
        }
        return false;
    }
    void incH() { if(hours++ >= HOUR_PER_DAY) hours = 0;     }
    void incM() { if(minutes++ >= MIN_PER_HOUR) minutes = 0; }
    void decH() { if(hours-- <= 0) hours = HOUR_PER_DAY;      }
    void decM() { if(minutes-- <= 0) minutes = MIN_PER_HOUR;  }
};

class clock_t
{
private:
    void ClearIRQ() { RTC->CRL &= ~RTC_CRL_SECF; }
    void EnterConfMode()
    {
        while(!(RTC->CRL & RTC_CRL_RTOFF)); // Wait RTOFF
        RTC->CRL |= RTC_CRL_CNF; // Enter conf mode
    }
    void LeaveConfMode() {
        RTC->CRL &= ~RTC_CRL_CNF; // Leave conf mode
        while(!(RTC->CRL & RTC_CRL_RTOFF)); // Wait RTOFF
    }

    void Sync() {
        while(!(RTC->CRL & RTC_CRL_RSF));
    }
public:
    time_t CurrentTime, CurrentAlarm;

    void Init();
    void SetTime(time_t* Time) { CurrentTime = *Time; }
    void SetAlarm(time_t* Time);
    void Display();
    void Toggle();

    void DisplayForSetting(time_t Time, bool SetH);
    void IrqHandler();
    void AlarmHandler();
};

extern clock_t Clock;

#endif /* INC_CLOCK_H_ */
