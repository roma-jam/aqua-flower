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
#include "clock.h"
#include "config.h"
#include "wpump.h"

enum screen_type_t {
    st_ScreenSaver = 0,
    st_Settings,
    st_TimeSettings,
    st_PumpSettings,
    st_Pump1Settings,
    st_Pump2Settings,
    st_Pump1SetUp,
    st_Pump2SetUp,
};

enum pump_config_step_t {
    pcs_Periodic = 0,
    pcs_Duration
};

extern void AppScreenSaverTimeout(void *p);

class App_t
{
private:
    // Lcd & Graphics
    VirtualTimer        SSTimer;
    screen_type_t       CurrScreen;
    uint8_t             PointerX, PointerY;
    time_t              SetUpTime;
    bool                SetHours;
    pump_config_step_t  SetPumpConfig;
    water_pump_conf_t   wPumpSetup;


    void Button();
    void DrawScreen();
    void DrawPumpInfo(uint8_t PumpNum);
    void SSTimerStart()
    {
        chVTReset(&SSTimer);
        chVTSet(&SSTimer, MS2ST(APP_SCREENSAVER_TIMEOUT_MS), AppScreenSaverTimeout, nullptr);
    }

public:
    // WPump
    water_pump_conf_t wPump1_Conf, wPump2_Conf;

    Thread *PThd;
    void Init();
    void Task();
    void ScreenSaver();
    void SendEvent(eventmask_t mask)   { chEvtSignal(PThd, mask);  }
    void SendEventI(eventmask_t mask)  { chEvtSignalI(PThd, mask); }
};

extern App_t App;

#endif /* APPLICATION_H_ */
