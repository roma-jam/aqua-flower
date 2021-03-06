/*
 * wpump.h
 *
 *  Created on: 16 ���. 2016 �.
 *      Author: RomaJam
 */

#ifndef INC_WPUMP_H_
#define INC_WPUMP_H_

#include "kl_lib_f100.h"
#include "config.h"

#pragma pack(push, 1)
struct water_pump_conf_t {
    uint8_t wpumpNum;
    uint8_t everyH;
    uint8_t durationS;
    uint8_t powerCounter;
    void Flush() { wpumpNum = 0; everyH = 0; durationS = 0; powerCounter = 0; }
    void operator= (const water_pump_conf_t wPumpCfg) {
        wpumpNum        = wPumpCfg.wpumpNum;
        everyH          = wPumpCfg.everyH;
        durationS       = wPumpCfg.durationS;
        powerCounter    = wPumpCfg.powerCounter;
    }
};
#pragma pack(pop)

class wpump_t {
private:
    VirtualTimer TimerPump1, TimerPump2;
    void InitGpios() {
        PinSetupOut(WPUMP_GPIO, WPUMP1, omPushPull, ps50MHz);
        PinSetupOut(WPUMP_GPIO, WPUMP2, omPushPull, ps50MHz);
        PinClear(WPUMP_GPIO, WPUMP1);
        PinClear(WPUMP_GPIO, WPUMP2);
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
