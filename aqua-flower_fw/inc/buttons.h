/*
 * buttons.h
 *
 *  Created on: 10 апр. 2016 г.
 *      Author: RomaJam
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include "kl_lib_f100.h"

#define BUTTON_STATE_TIME_MS      99

#define BUTTON_GPIO               GPIOB

#define BUTTON_OK                 0
#define BUTTON_CANCEL             1
#define BUTTON_UP                 2
#define BUTTON_DOWN               3
#define BUTTON_LEFT               4
#define BUTTON_RIGTH              5

#define BUTTONS_CNT                6

enum btn_type_t {
    bt_OK       = BUTTON_OK,
    bt_Cancel,
    bt_Up,
    bt_Down,
    bt_Left,
    bt_Right
};

enum btn_state_t {
    bs_Released = 0,
    bs_Pressed,
};

class buttons_t {
private:
    VirtualTimer Timer;
    void InitGpios()
    {
        PinSetupIn(BUTTON_GPIO, BUTTON_OK,  pudPullUp);
        PinSetupIn(BUTTON_GPIO, BUTTON_CANCEL, pudPullUp);
        PinSetupIn(BUTTON_GPIO, BUTTON_UP, pudPullUp);
        PinSetupIn(BUTTON_GPIO, BUTTON_DOWN, pudPullUp);
        PinSetupIn(BUTTON_GPIO, BUTTON_LEFT, pudPullUp);
        PinSetupIn(BUTTON_GPIO, BUTTON_RIGTH, pudPullUp);
    }

public:
    btn_state_t BtnState[BUTTONS_CNT];
    void Init();
    void Task();
};


extern buttons_t Buttons;

#endif /* INC_BUTTONS_H_ */
