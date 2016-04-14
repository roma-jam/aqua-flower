/*
 * main.cpp
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */


#include "ch.h"
#include "hal.h"

#include "lcd.h"
#include "buzzer.h"
#include "buttons.h"
#include "eeprom.h"
#include "clock.h"

#include "application.h"

#include "clock_digits.h"

static inline void Init();

int main(void)
{
    // ==== Init clock system ====
    uint8_t ClkEnable = 1;
    ClkEnable = Clk.ClockConfigure(ciHSI, pllMul12, ahbDiv1, apbDiv1, apbDiv1, psPLLOn);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    if(!ClkEnable) Uart.Printf("CF=%u\r", ClkEnable);

    while(TRUE)
    {
        Clock.IrqHandler();
        chThdSleepMilliseconds(999);
    }



}

void Init()
{
    JtagDisable();
    Uart.Init(115200);
    Uart.Printf("\rAqua Flower AHB=%u MHz\r", Clk.AHBFreqHz/1000000);

    Lcd.Init();
    Buzzer.Init();
    Buttons.Init();
    Clock.Init();

    EE.Init();
    EE.ReadConf();

    Lcd.Printf(1, 0, "Aqua  Flower");
//    Buzzer.BeepBeep();
    Clock.Display();

}



