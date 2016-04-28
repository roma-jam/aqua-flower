/*
 * wpump.h
 *
 *  Created on: 16 ���. 2016 �.
 *      Author: RomaJam
 */

#ifndef INC_WPUMP_H_
#define INC_WPUMP_H_

#include "kl_lib_f100.h"


#define WPUMP_GPIO      GPIOA

#define WPUMP_1         0
#define WPUMP_2         1

class wpump_t {
private:
    VirtualTimer TimerPump1, TimerPump2;
    void InitGpios() {
        PinSetupOut(WPUMP_GPIO, WPUMP_1, omPushPull, ps50MHz);
        PinSetupOut(WPUMP_GPIO, WPUMP_2, omPushPull, ps50MHz);
        PinClear(WPUMP_GPIO, WPUMP_1);
        PinClear(WPUMP_GPIO, WPUMP_2);
    }
    void Enable(uint8_t WPUMP)  { PinSet(WPUMP_GPIO, WPUMP);     }
public:
    void Init() { InitGpios(); }
    void EnablePump1(uint32_t ms);
    void EnablePump2(uint32_t ms);
    void Disable(uint8_t WPUMP) { PinClear(WPUMP_GPIO, WPUMP);   }
};


extern wpump_t WaterPump;
#endif /* INC_WPUMP_H_ */
