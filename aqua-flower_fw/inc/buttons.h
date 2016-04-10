/*
 * buttons.h
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include "kl_lib_f100.h"

#define BUTTON_GPIO     GPIOB

#define BUTTON_OK       0
#define BUTTON_CANCEL   1
#define BUTTON_UP       2
#define BUTTON_DOWN     3
#define BUTTON_LEFT     4
#define BUTTON_RIGTH    5


class buttons_t {
private:
    void InitGpios()
    {
        PinSetupIn(BUTTON_GPIO, BUTTON_OK, pudPullUp);
        PinSetupIn(BUTTON_GPIO, BUTTON_CANCEL, pudPullUp);
        PinSetupIn(BUTTON_GPIO, BUTTON_UP, pudPullUp);
        PinSetupIn(BUTTON_GPIO, BUTTON_DOWN, pudPullUp);
        PinSetupIn(BUTTON_GPIO, BUTTON_LEFT, pudPullUp);
        PinSetupIn(BUTTON_GPIO, BUTTON_RIGTH, pudPullUp);
    }
public:
    void Init();
    void Task();

};


extern buttons_t Buttons;

#endif /* INC_BUTTONS_H_ */
