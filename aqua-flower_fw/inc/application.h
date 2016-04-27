/*
 * application.h
 *
 *  Created on: 19 февр. 2014 г.
 *      Author: r.leonov
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_


#include <kl_lib_f100.h>
#include <string.h>
#include "application_config.h"
#include "clock.h"

enum screen_type_t {
    st_ScreenSaver = 0,
    st_Settings,
    st_TimeSettings,
    st_PumpSettings,
    st_Pump1Settings,
    st_Pump2Settings
};

extern void AppScreenSaverTimeout(void *p);

class App_t
{
private:
    VirtualTimer SSTimer;
    void SSTimerStart()
    {
        chVTReset(&SSTimer);
        chVTSet(&SSTimer, MS2ST(APP_SCREENSAVER_TIMEOUT_MS), AppScreenSaverTimeout, nullptr);
    }

    screen_type_t CurrScreen;
    void Button();
    void DrawScreen();

    uint8_t PointerX, PointerY;

    time_t SetUpTime;
    bool SetHours;
public:
    Thread *PThd;
    void Init();
    void Task();
    void ScreenSaver();
    void SendEvent(eventmask_t mask)  { chEvtSignal(PThd, mask); }
    void SendEventI(eventmask_t mask)  { chEvtSignalI(PThd, mask); }
};

extern App_t App;

#endif /* APPLICATION_H_ */
