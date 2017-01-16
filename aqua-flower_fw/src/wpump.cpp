/*
 * wpump.cpp
 *
 *  Created on: 28 апр. 2016 г.
 *      Author: RLeonov
 */


#include "wpump.h"


wpump_t WaterPump;

void Pump1Tmr(void *p)
{
    chSysLockFromIsr();
#if (APP_WATER_PUMP_DEBUG)
    Uart.Printf("D1\r\n");
#endif
    WaterPump.Disable(WPUMP1);
    chSysUnlockFromIsr();
}

void Pump2Tmr(void *p)
{
    chSysLockFromIsr();
#if (APP_WATER_PUMP_DEBUG)
    Uart.Printf("D2\r\n");
#endif
    WaterPump.Disable(WPUMP2);
    chSysUnlockFromIsr();
}

void wpump_t::EnablePump1(uint32_t ms)
{
#if (APP_WATER_PUMP_DEBUG)
    Uart.Printf("E1\r\n");
#endif
    Enable(WPUMP1);
    chVTSet(&TimerPump1, MS2ST(ms), Pump1Tmr, nullptr);
}

void wpump_t::EnablePump2(uint32_t ms)
{
#if (APP_WATER_PUMP_DEBUG)
    Uart.Printf("E2\r\n");
#endif
    Enable(WPUMP2);
    chVTSet(&TimerPump2, MS2ST(ms), Pump2Tmr, nullptr);
}

