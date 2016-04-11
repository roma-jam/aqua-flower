/*
 * main.cpp
 *
 *  Created on: 10 ���. 2016 �.
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
        chThdSleepMilliseconds(999);
        Lcd.DelimeterToggle();
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
    Lcd.DrawClockDigit(0, 2);
    Lcd.DrawClockDigit(1, 3);
    Lcd.DrawClockDigit(2, 1);
    Lcd.DrawClockDigit(3, 2);
//    Buzzer.BeepBeep();
}



