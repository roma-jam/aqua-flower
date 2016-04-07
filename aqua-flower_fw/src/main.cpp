/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "ch.h"
#include "hal.h"

#include "application.h"

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
    }
}

void Init()
{
    JtagDisable();
    Uart.Init(115200);

    Uart.Printf("\rAqua Flower AHB=%u MHz\r", Clk.AHBFreqHz/1000000);
}
