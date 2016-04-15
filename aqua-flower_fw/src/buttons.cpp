/*
 * buttons.cpp
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */

#include "buttons.h"
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
    chVTSetI(&Timer, MS2ST(BUTTON_STATE_TIME_MS), TmrButtonCallback, nullptr);
}

void buttons_t::Task()
{
    // check state button 1
    if(PinIsSet(BUTTON_GPIO, bt_OK) && BtnState[bt_OK] == bs_Released)
    {
        Uart.Printf("OK pressed\r\n");
        BtnState[bt_OK] = bs_Pressed;
    }
    else if(BtnState[bt_OK] == bs_Pressed)
    {
        Uart.Printf("OK released\r\n");
        BtnState[bt_OK] = bs_Released;
    }
    // send Evt to App thread
    chVTSetI(&Timer, MS2ST(BUTTON_STATE_TIME_MS), TmrButtonCallback, nullptr);
}
