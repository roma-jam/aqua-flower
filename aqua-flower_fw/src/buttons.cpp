/*
 * buttons.cpp
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */

#include "buttons.h"

buttons_t Buttons;

// =============================== Thread ======================================
static WORKING_AREA(waBtnThread, 128);
__attribute__ ((__noreturn__))
static void BtnThread(void *arg)
{
    chRegSetThreadName("ButtonTask");
    while(1)
        Buttons.Task();
}

void buttons_t::Init()
{
    InitGpios();

    //chThdCreateStatic(waBtnThread, sizeof(waBtnThread), NORMALPRIO, (tfunc_t)BtnThread, NULL);
}

void buttons_t::Task()
{
    chThdSleepMilliseconds(999);
}
