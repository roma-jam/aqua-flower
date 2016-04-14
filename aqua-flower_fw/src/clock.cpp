/*
 * clock.cpp
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */

#include "clock.h"

clock_t Clock;

void clock_t::Init()
{

    rccEnablePWRInterface(FALSE);
    rccEnableBKPInterface(FALSE);
    PWR->CR |= PWR_CR_DBP; // enable access to the Backup registers and RTC

    // RCC set RTC to LSE 32.768 kHz input
    // Enable LSE
    if(!Clk.LSEEnable())
    {
        Uart.Printf("LSE Failed\r\n");
        return;
    }

    RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
    RCC->BDCR |= RCC_BDCR_RTCEN;

    EnterConfMode();
    RTC->DIVH = 0;
    RTC->DIVL = 0;
    RTC->PRLH = 0;
    RTC->PRLL = 0x7FFF; // input clock frequency is 32.768 kHz
    Sync();
    RTC->CRH = RTC_CRH_SECIE; // second interrupt enable
    LeaveConfMode();
}

void clock_t::SetTime(time_t* Time)
{
    uint32_t sec = 0;
    sec = (Time->hours * 60 * 60) + (Time->minutes * 60) + Time->seconds;
    EnterConfMode();
    RTC->CNTH = (uint16_t)(sec & 0xFFFF0000 >> 16);
    RTC->CNTL = (uint16_t)(sec & 0x0000FFFF);
    Sync();
    LeaveConfMode();
}


void clock_t::Display()
{
    uint8_t hHi, hLo, mHi, mLo;

    hLo = CurrentTime.hours;
    hHi = CurrentTime.hours / 10;
    mLo = CurrentTime.minutes;
    mHi = CurrentTime.minutes / 10;

    if(mHi)
        mLo -= mHi * 10;

    if(hHi)
    {
        Lcd.DrawClockDigit(0, hHi);
        hLo -= hHi * 10;
    }

    Lcd.DrawClockDigit(1, hLo);
    Lcd.DrawClockDigit(2, mHi);
    Lcd.DrawClockDigit(3, mLo);
}


void clock_t::IrqHandler()
{
    Uart.Printf("Clock IRQ: ++s\r\n");
    CurrentTime.Update();
    Lcd.DelimeterToggle();
}

void clock_t::AlarmHandler()
{
    Uart.Printf("Alarm!\r\n");
}

extern "C"
{
    CH_IRQ_HANDLER(RTC_IRQHandler)
    {
        CH_IRQ_PROLOGUE();
        chSysLockFromIsr();
        Clock.IrqHandler();
        chSysUnlockFromIsr();
        CH_IRQ_EPILOGUE();
    }
    CH_IRQ_HANDLER(RTC_Alarm_IRQHandler)
    {
        CH_IRQ_PROLOGUE();
        chSysLockFromIsr();
        Clock.AlarmHandler();
        chSysUnlockFromIsr();
        CH_IRQ_EPILOGUE();
    }
}
