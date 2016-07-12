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
#include "buzzer.h"
#include "lcd.h"
#include "eeprom.h"

App_t App;

#define GET_EEPROM_ADDR(PUMP)      ((uint32_t)(APP_EEPROM_CONFIG_ADDR + (sizeof(uint32_t)*PUMP)))

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
            Clock.Toggle();
    }

    if(EvtMsk & EVTMSK_MIN_UPDATE)
    {
        if(CurrScreen == st_ScreenSaver)
            Clock.Display();
    }

    if(EvtMsk & EVTMSK_BUTTON_UPDATE)
        Button();


    DrawScreen();
}

void App_t::Init()
{
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    SSTimerStart();
    wPump1_Conf.Flush();
    wPump2_Conf.Flush();

    if(!EE.isInit())
    {
        Uart.Printf("APP: EE not ready. PumpConf FALURE\r\n");
        return;
    }

    EE.ReadConf(GET_EEPROM_ADDR(WPUMP_1), (uint32_t*)&wPump1_Conf);
    EE.ReadConf(GET_EEPROM_ADDR(WPUMP_2), (uint32_t*)&wPump2_Conf);
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
                            SetUpTime = Clock.CurrentTime;
                            SetHours = true;
                            break;
                        }
                        if (PointerY == APP_PUMP_POSITION)
                        {
                            CurrScreen = st_PumpSettings;
                            PointerY = APP_PUMP1_POSITIONS;
                        }
                    }
                    break;
                }
                case st_TimeSettings:
                {
                    if(Btn == bt_Cancel)
                    {
                        CurrScreen = st_Settings;
                        break;
                    }
                    if(Btn == bt_OK)
                    {
                        CurrScreen = st_Settings;
                        Clock.SetTime(&SetUpTime);
                        break;
                    }
                    if(Btn == bt_Left || Btn == bt_Right)
                    {
                        SetHours = !SetHours;
                        break;
                    }
                    if(Btn == bt_Up)
                    {
                        if(SetHours)
                            SetUpTime.incH();
                        else
                            SetUpTime.incM();
                    }
                    if(Btn == bt_Down)
                    {
                        if(SetHours)
                            SetUpTime.decH();
                        else
                            SetUpTime.decM();
                    }
                }
                    break;

                case st_PumpSettings:
                {
                    if(Btn == bt_Cancel)
                        CurrScreen = st_Settings;

                    if(Btn == bt_Up || Btn == bt_Down)
                    {
                        PointerY = (PointerY == APP_PUMP1_POSITIONS)? APP_PUMP2_POSITIONS : APP_PUMP1_POSITIONS;
                    }

                    if(Btn == bt_OK)
                    {
                        if(PointerY == APP_PUMP1_POSITIONS)
                        {
                            CurrScreen = st_Pump1Settings;
                        }
                        else if (PointerY == APP_PUMP2_POSITIONS)
                        {
                            CurrScreen = st_Pump2Settings;
                        }
                    }
                    if(Btn == bt_Right)
                    {
                        if(PointerY == APP_PUMP1_POSITIONS)
                        {
                            WaterPump.EnablePump1(wPump1_Conf.durationS * 1000);
                        }
                        else if (PointerY == APP_PUMP2_POSITIONS)
                        {
                            WaterPump.EnablePump2(wPump2_Conf.durationS * 1000);
                        }
                    }
                    break;
                }

                case st_Pump1Settings:
                    if(Btn == bt_Left || Btn == bt_Cancel)
                        CurrScreen = st_PumpSettings;
                    if(Btn == bt_OK)
                    {
                        CurrScreen = st_Pump1SetUp;
                        SetPumpConfig = pcs_Periodic;
                        wPumpSetup.Flush();
                    }
                    break;

                case st_Pump2Settings:
                    if(Btn == bt_Left || Btn == bt_Cancel)
                        CurrScreen = st_PumpSettings;
                    if(Btn == bt_OK)
                    {
                        CurrScreen = st_Pump2SetUp;
                        SetPumpConfig = pcs_Periodic;
                        wPumpSetup.Flush();
                    }
                    break;

                case st_Pump1SetUp:
                    if(Btn == bt_Cancel)
                        CurrScreen = st_Pump1Settings;
                    if(Btn == bt_OK)
                    {
                        if(SetPumpConfig == pcs_Periodic)
                            SetPumpConfig = pcs_Duration;
                        else
                        {
                            wPumpSetup.wpumpNum = (WPUMP_1 + 1);
                            wPump1_Conf = wPumpSetup;
                            EE.WriteConf(GET_EEPROM_ADDR(WPUMP_1), (uint32_t*)&wPump1_Conf);
                            CurrScreen = st_Pump1Settings;
                        }
                    }
                    if(Btn == bt_Up)
                    {
                        if(SetPumpConfig == pcs_Periodic)
                            wPumpSetup.everyH++;
                        else
                            wPumpSetup.durationS++;
                    }
                    if(Btn == bt_Down)
                    {
                        if(SetPumpConfig == pcs_Periodic)
                            wPumpSetup.everyH--;
                        else
                            wPumpSetup.durationS--;
                    }
                    break;

                case st_Pump2SetUp:
                    if(Btn == bt_Cancel)
                        CurrScreen = st_Pump2Settings;
                    if(Btn == bt_OK)
                    {
                        if(SetPumpConfig == pcs_Periodic)
                            SetPumpConfig = pcs_Duration;
                        else
                        {
                            wPumpSetup.wpumpNum = (WPUMP_2 + 1);
                            wPump2_Conf = wPumpSetup;
                            EE.WriteConf(GET_EEPROM_ADDR(WPUMP_2), (uint32_t*)&wPump2_Conf);
                            CurrScreen = st_Pump2Settings;
                        }
                    }
                    if(Btn == bt_Up)
                    {
                        if(SetPumpConfig == pcs_Periodic)
                            wPumpSetup.everyH++;
                        else
                            wPumpSetup.durationS++;
                    }
                    if(Btn == bt_Down)
                    {
                        if(SetPumpConfig == pcs_Periodic)
                            wPumpSetup.everyH--;
                        else
                            wPumpSetup.durationS--;
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
            Lcd.Printf(APP_HEADER_POSITION_X, APP_HEADER_POSITION_Y, "TIME  SETTINGS");
            Lcd.Printf(APP_BOTTOM_POSITION_X, APP_BOTTOM_POSITION_1Y, "%c,%c,%c,%c to set", 0x18, 0x19, 0x1A, 0x1B);
            Lcd.Printf(APP_BOTTOM_POSITION_X, APP_BOTTOM_POSITION_2Y, "OK - SAVE");
            Clock.DisplayForSetting(SetUpTime, SetHours);
            break;

        case st_PumpSettings:
            Lcd.Cls();
            Lcd.Printf(APP_HEADER_POSITION_X, APP_HEADER_POSITION_Y, "PUMP  SETTINGS");
            Lcd.Printf(APP_MENU_POSITION, APP_PUMP1_POSITIONS, "PUMP1");
            Lcd.Printf(APP_MENU_POSITION, APP_PUMP2_POSITIONS, "PUMP2");
            Lcd.Printf(APP_BOTTOM_POSITION_X, APP_BOTTOM_POSITION_2Y, "%c - test pump", 0x1A);
            Lcd.Printf(PointerX, PointerY, "%c", 0x10);
            break;

        case st_Pump1Settings:
            Lcd.Cls();
            DrawPumpInfo(WPUMP_1);
            break;

        case st_Pump2Settings:
            Lcd.Cls();
            DrawPumpInfo(WPUMP_2);
            break;

        case st_Pump1SetUp:
            Lcd.Cls();
            Lcd.Printf(APP_HEADER_POSITION_X, APP_HEADER_POSITION_Y, "SET UP PUMP1");
            Lcd.Printf(APP_BOTTOM_POSITION_X, APP_BOTTOM_POSITION_1Y, "%c,%c to set", 0x18, 0x19);
            Lcd.Printf(APP_BOTTOM_POSITION_X, APP_BOTTOM_POSITION_2Y, "OK - SAVE");

            if(SetPumpConfig == pcs_Periodic)
                Lcd.Printf(0, APP_LINE_2_POSITION, "every: %uh", wPumpSetup.everyH);
            else
                Lcd.Printf(0, APP_LINE_2_POSITION, "duration: %us", wPumpSetup.durationS);
            break;

        case st_Pump2SetUp:
            Lcd.Cls();
            Lcd.Printf(APP_HEADER_POSITION_X, APP_HEADER_POSITION_Y, "SET UP PUMP2");
            Lcd.Printf(APP_BOTTOM_POSITION_X, APP_BOTTOM_POSITION_1Y, "%c,%c to set", 0x18, 0x19);
            Lcd.Printf(APP_BOTTOM_POSITION_X, APP_BOTTOM_POSITION_2Y, "OK - SAVE");

            if(SetPumpConfig == pcs_Periodic)
                Lcd.Printf(0, APP_LINE_2_POSITION, "every: %uh", wPumpSetup.everyH);
            else
                Lcd.Printf(0, APP_LINE_2_POSITION, "duration: %us", wPumpSetup.durationS);
            break;
    }
}

void App_t::DrawPumpInfo(uint8_t PumpNum)
{
    water_pump_conf_t *p = (PumpNum == WPUMP_1)? &wPump1_Conf : &wPump2_Conf;
    Lcd.Printf(APP_HEADER_POSITION_X, APP_HEADER_POSITION_Y, "PUMP%u SET UP", p->wpumpNum);
    Lcd.Printf(APP_BOTTOM_POSITION_X, APP_BOTTOM_POSITION_1Y, "setup: OK");
    Lcd.Printf(APP_BOTTOM_POSITION_X, APP_BOTTOM_POSITION_2Y, "back: CANCEL");

    Lcd.Printf(0, APP_LINE_1_POSITION, "every: %uh", p->everyH);
    Lcd.Printf(0, APP_LINE_2_POSITION, "duration: %us", p->durationS);
    Lcd.Printf(0, APP_LINE_3_POSITION, "on's: %u", p->powerCounter);
}
