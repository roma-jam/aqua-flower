/*
 * application.cpp
 *
 *  Created on: 19 ����. 2014 �.
 *      Author: r.leonov
 */

#include "application.h"

#if 1 // ==== Init's ====
App_t App;

void AppTask();

static WORKING_AREA(waAppThread, 128);
__attribute__ ((__noreturn__))
static void AppThread(void *arg)
{
    chRegSetThreadName("App");
    while(1)
    {
        AppTask();
    }
}
#endif

void AppTask()
{
    uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
    Uart.Printf("App Evt: %u\r\n", EvtMsk);
}

void App_t::Init()
{
//    chVTSet(&ITmr, MS2ST(999), MeasureTmrCallback, nullptr);
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
}