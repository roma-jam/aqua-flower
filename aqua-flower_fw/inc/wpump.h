/*
 * wpump.h
 *
 *  Created on: 16 апр. 2016 г.
 *      Author: RomaJam
 */

#ifndef INC_WPUMP_H_
#define INC_WPUMP_H_

#include "kl_lib_f100.h"


#define WPUMP_GPIO      GPIOA

#define WPUMP_0         0
#define WPUMP_1         1

class wpump_t {
private:
    void InitGpios() {
        PinSetupOut(WPUMP_GPIO, WPUMP_0, omPushPull, ps50MHz);
        PinSetupOut(WPUMP_GPIO, WPUMP_1, omPushPull, ps50MHz);
        PinClear(WPUMP_GPIO, WPUMP_0);
        PinClear(WPUMP_GPIO, WPUMP_1);
    }
public:
    void Init() { InitGpios(); Uart.Printf("WPump Enable\r\n");  }
    void Enable(uint8_t WPUMP)  { PinSet(WPUMP_GPIO, WPUMP);     }
    void Disable(uint8_t WPUMP) { PinClear(WPUMP_GPIO, WPUMP);   }
};


extern wpump_t WaterPump;
#endif /* INC_WPUMP_H_ */
