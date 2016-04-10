/*
 * buzzer.cpp
 *
 *  Created on: 08 рту. 2015 у.
 *      Author: RomaJam
 */



#include <buzzer.h>

buzzer_t Buzzer;

void TmrBuzzerCallback(void *p) {
    chSysLockFromIsr();
    Buzzer.Timeout();
    chSysUnlockFromIsr();
}

void buzzer_t::Timeout() {
    if(isBeep()) { // start timer to delay and decrese the count
        BuzzerOff();
        BeepCnt--;
        if(BeepCnt != 0)
            chVTSetI(&Timer, MS2ST(BUZZER_DELAY_MS), TmrBuzzerCallback, nullptr);
    }
    else { // beep and start timer
        BuzzerOn();
        chVTSetI(&Timer, MS2ST(BUZZER_DURATION_MS), TmrBuzzerCallback, nullptr);
    }
}

void buzzer_t::Init() {
#ifdef BUZZER_ON_TIMER
    PinSetupAlterFuncOutput(BUZZER_GPIO, BUZZER_PIN, omPushPull, ps50MHz);
    // Setup timer1
    rccEnableAPB2(RCC_APB2ENR_TIM1EN, false);

    BUZZER_TIM->CR1 = 0x01;      // Enable timer
    BUZZER_TIM->CR2 = 0;
    BUZZER_TIM->PSC = 48;         // Do not divide input freq
    BUZZER_TIM->ARR = 100;       // Autoreload register: 100
    BUZZER_TIM->BDTR = 0xC000;   // Main output Enable
    BUZZER_TIM->CCMR1 = 0x0060;  // PWM mode1 on Ch1 enabled
    BUZZER_TIM->CCER = 0x0001;   // Output1 enabled

#else
    PinSetupOut(BUZZER_GPIO, BUZZER_PIN, omPushPull, ps50MHz);
#endif
    BuzzerOff();
}

void buzzer_t::Sequence(uint8_t BuzzTimes) {
    if(isBeep()) { // turn off timer and clean beeper
        chVTResetI(&Timer);
        BuzzerOff();
    }
    BeepCnt = BuzzTimes;
    BuzzerOn();
    chVTSet(&Timer, MS2ST(BUZZER_DURATION_MS), TmrBuzzerCallback, nullptr);
}
