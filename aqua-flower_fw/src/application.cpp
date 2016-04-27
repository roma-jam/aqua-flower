/*
 * application.cpp
 *
 *  Created on: 19 февр. 2014 г.
 *      Author: r.leonov
 */

#include "application.h"
#include "events.h"

// Periph
#include "buttons.h"
#include "lcd.h"

App_t App;

static WORKING_AREA(waAppThread, 256);
__attribute__ ((__noreturn__))
static void AppThread(void *arg)
{
    chRegSetThreadName("App");
    while(1)
        App.Task();
}

void AppScreenSaverTimeout(void *p) {
    chSysLockFromIsr();
    App.ScreenSaver();
    chSysUnlockFromIsr();
}

void App_t::Task()
{
    uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);

    if(EvtMsk & EVTMSK_SEC_UPDATE)
    {
        if(CurrScreen == st_ScreenSaver)
        {
            Clock.Toggle();
            if(Clock.CurrentTime.Update())
                Clock.Display();
        }
    }

    if(EvtMsk & EVTMSK_BUTTON_UPDATE)
        Button();


    DrawScreen();
}

void App_t::Init()
{
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    SSTimerStart();
}


// ============================= Application Logic ==============================

void App_t::Button()
{
    Lcd.BacklightOn();
    SSTimerStart();
    for(uint8_t Btn = 0; Btn < BUTTONS_CNT; Btn++)
    {
        if(Buttons.pBtnState[Btn] == bs_Pressed)
        {
            switch(CurrScreen)
            {
                case st_ScreenSaver:
                {
                    if(Btn != bt_OK)
                        break;
                    CurrScreen = st_Settings;
                    PointerX = (uint8_t)(APP_MENU_POSITION - 1);
                    PointerY = APP_TIME_POSITION;
                    break;
                }

                case st_Settings:
                {
                    if(Btn == bt_Up || Btn == bt_Down)
                    {
                        PointerY = (PointerY == APP_TIME_POSITION)? APP_PUMP_POSITION : APP_TIME_POSITION;
                    }
                    if(Btn == bt_OK)
                    {
                        if(PointerY == APP_TIME_POSITION)
                        {
                            CurrScreen = st_TimeSettings;
                        }
                        else if (PointerY == APP_PUMP_POSITION)
                        {
                            CurrScreen = st_PumpSettings;
                        }
                    }
                    break;
                }
                case st_TimeSettings:
                {
                    if(Btn == bt_Cancel)
                        CurrScreen = st_Settings;
                }
                    break;

                case st_PumpSettings:
                {
                    if(Btn == bt_Cancel)
                        CurrScreen = st_Settings;
                }
                    break;

            }
        }
    }
}

void App_t::ScreenSaver()
{
    CurrScreen = st_ScreenSaver;
    Lcd.Cls();
    Clock.Display();
}

void App_t::DrawScreen()
{
    switch(CurrScreen)
    {
        case st_ScreenSaver:
            break;

        case st_Settings:
            Lcd.Cls();
            Lcd.Printf(APP_MENU_POSITION, APP_TIME_POSITION, "TIME");
            Lcd.Printf(APP_MENU_POSITION, APP_PUMP_POSITION, "PUMP");
            Lcd.Printf(PointerX, PointerY, "%c", 0x10);
            break;

        case st_TimeSettings:
            Lcd.Cls();
            Lcd.Printf(0, 0, "TIME  SETTINGS");
            break;

        case st_PumpSettings:
            Lcd.Cls();
            Lcd.Printf(0, 0, "PUMP  SETTINGS");
            break;
    }
}
