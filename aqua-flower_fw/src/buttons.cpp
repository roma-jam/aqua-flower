/*
 * buttons.cpp
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */

#include "buttons.h"
#include "buzzer.h"
#include <string.h>

buttons_t Buttons;


void TmrButtonCallback(void *p)
{
    chSysLockFromIsr();
    Buttons.Task();
    chSysUnlockFromIsr();
}


void buttons_t::Init()
{
    InitGpios();
    memset((uint8_t*)&BtnState, 0x00, sizeof(btn_state_t) * BUTTONS_CNT);
    chVTSet(&Timer, MS2ST(BUTTON_STATE_TIME_MS), TmrButtonCallback, nullptr);
}

void buttons_t::Task()
{
    // check state button 1
    for(uint8_t i = 0; i < BUTTONS_CNT; i++)
    {
        if(!PinIsSet(BUTTON_GPIO, i) && BtnState[i] == bs_Released)
        {
            Uart.Printf("%u pressed\r\n", i);
            BtnState[i] = bs_Pressed;
        }
        else if(PinIsSet(BUTTON_GPIO, i) && BtnState[i] == bs_Pressed)
        {
            Uart.Printf("%u released\r\n", i);
            BtnState[i] = bs_Released;
        }
    }
    // send Evt to App thread
    chVTSetI(&Timer, MS2ST(BUTTON_STATE_TIME_MS), TmrButtonCallback, nullptr);
}
