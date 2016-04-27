/*
 * clock.cpp
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */

#include <string.h>
#include "clock.h"
#include "events.h"
#include "application.h"

clock_t Clock;

void clock_t::Init()
{
    rccEnablePWRInterface(FALSE);
    rccEnableBKPInterface(FALSE);
    PWR->CR |= PWR_CR_DBP; // enable access to the Backup registers and RTC

    // RCC set RTC to LSE 32.768 kHz input
    // Enable LSE
    if(Clk.LSEEnable() == 1)
    {
        Uart.Printf("LSE Failed\r\n");
        return;
    }

    nvicEnableVector(RTC_IRQn, CORTEX_PRIORITY_MASK(IRQ_PRIO_MEDIUM));
    nvicEnableVector(RTC_Alarm_IRQn, CORTEX_PRIORITY_MASK(IRQ_PRIO_MEDIUM));

    RCC->BDCR |= RCC_BDCR_RTCEN;
    RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;

    EnterConfMode();
    RTC->DIVH = 0;
    RTC->DIVL = 0;
    RTC->PRLH = 0;
    RTC->PRLL = 0x7FFF; // input clock frequency is 32.768 kHz
    Sync();
    RTC->CRH = RTC_CRH_SECIE; // second interrupt enable
    LeaveConfMode();

}

void clock_t::Toggle()
{
    Lcd.DelimeterToggle();
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

void clock_t::DisplayForSetting(time_t Time, bool SetH)
{
    uint8_t hHi, hLo, mHi, mLo;

    hLo = Time.hours;
    hHi = Time.hours / 10;
    mLo = Time.minutes;
    mHi = Time.minutes / 10;

    if(SetH)
        Lcd.SetDrawMode(OVERWRITE_INVERTED);
    if(mHi)
        mLo -= mHi * 10;

    if(hHi)
    {
        Lcd.DrawClockDigit(0, hHi);
        hLo -= hHi * 10;
    }
    Lcd.DrawClockDigit(1, hLo);
    if(SetH)
        Lcd.SetDrawMode(OVERWRITE);

    if(!SetH)
        Lcd.SetDrawMode(OVERWRITE_INVERTED);
    Lcd.DrawClockDigit(2, mHi);
    Lcd.DrawClockDigit(3, mLo);
    if(!SetH)
        Lcd.SetDrawMode(OVERWRITE);
}


void clock_t::IrqHandler()
{
    ClearIRQ();
    App.SendEventI(EVTMSK_SEC_UPDATE);
}

void clock_t::AlarmHandler()
{
    RTC->CRL &= ~RTC_CRL_ALRF;
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
