/*
 * buzzer.h
 *
 *  Created on: 08 рту. 2015 у.
 *      Author: RomaJam
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "kl_lib_f100.h"

//#define BUZZER_ON_TIMER

#define BUZZER_DURATION_MS  51
#define BUZZER_DELAY_MS     99

#define BUZZER_GPIO         GPIOA
#define BUZZER_PIN          8

#define BUZZER_TIM          TIM1
#define BUZZER_CH           CCR1


class buzzer_t {
private:
    VirtualTimer Timer;
    bool isOn;
    uint8_t BeepCnt;
    void BuzzerOn()         {
#ifdef BUZZER_ON_TIMER
        BUZZER_TIM->CCR1 = 80;
#else
        PinSet(BUZZER_GPIO, BUZZER_PIN); isOn = true;
#endif
    }
    void BuzzerOff()        {
#ifdef BUZZER_ON_TIMER
        BUZZER_TIM->CCR1 = 100;
#else
        PinClear(BUZZER_GPIO, BUZZER_PIN); isOn = false;
#endif
    }
public:
    void Init();
    void Sequence(uint8_t BuzzTimes);
    void Beep() { Sequence(1); }
    void BeepBeep() { Sequence(2); }
    void Timeout();
    bool isBeep() { return isOn; }
};

extern buzzer_t Buzzer;


#endif /* INC_BUZZER_H_ */
