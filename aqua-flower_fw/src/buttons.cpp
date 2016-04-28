/*
 * buttons.cpp
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */

#include "buttons.h"
#include <string.h>
#include "buzzer.h"
#include "events.h"
#include "application.h"

buttons_t Buttons;

// =============================== Thread ======================================
static WORKING_AREA(waButtonThd, 128);
__attribute__ ((__noreturn__))
static void BtnThd(void *arg)
{
    chRegSetThreadName("ButtonTask");
    while(1)
        Buttons.Task();
}

void buttons_t::Init()
{
    InitGpios();
    memset((uint8_t*)&BtnState, 0x00, sizeof(btn_state_t) * BUTTONS_CNT);
    pBtnState = BtnState;
    chThdCreateStatic(waButtonThd, sizeof(waButtonThd), NORMALPRIO, (tfunc_t)BtnThd, NULL);
}

void buttons_t::Task()
{
    // check state button 1
    bool StateChanged = false;
    for(uint8_t i = 0; i < BUTTONS_CNT; i++)
    {
        if(!PinIsSet(BUTTON_GPIO, i) && pBtnState[i] == bs_Released)
        {
            pBtnState[i] = bs_Pressed;
            StateChanged = true;
        }
        else if(PinIsSet(BUTTON_GPIO, i) && pBtnState[i] == bs_Pressed)
            pBtnState[i] = bs_Released;
    }
    if(StateChanged)
        App.SendEvent(EVTMSK_BUTTON_UPDATE);

    chThdSleepMilliseconds(BUTTON_STATE_TIME_MS);
}
