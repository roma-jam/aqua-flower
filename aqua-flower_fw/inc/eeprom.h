/*
 * eeprom.h
 *
 *  Created on: 03 апр. 2015 г.
 *      Author: RomaJam
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "kl_lib_f100.h"

#define EEPROM_SYS_GPIO GPIOA
#define EEPROM_HOLD  2
#define EEPROM_WRITE 3

#define EEPROM_GPIO     GPIOB
#define EEPROM_CS       6
#define EEPROM_MISO     7
#define EEPROM_MOSI     8
#define EEPROM_SCK      9

class eeprom_t {
private:
    void InitGpios()
    {
        PinSetupOut(EEPROM_SYS_GPIO, EEPROM_WRITE, omPushPull, ps50MHz);
        PinSetupOut(EEPROM_SYS_GPIO, EEPROM_HOLD, omPushPull, ps50MHz);
        PinSetupOut(EEPROM_GPIO, EEPROM_CS, omPushPull, ps50MHz);
        PinSetupOut(EEPROM_GPIO, EEPROM_MISO, omPushPull, ps50MHz);
        PinSetupOut(EEPROM_GPIO, EEPROM_MOSI, omPushPull, ps50MHz);
        PinSetupIn(EEPROM_GPIO, EEPROM_MISO, pudNone);
    }

    void WR_Enable()    { PinSet  (EEPROM_SYS_GPIO, EEPROM_WRITE);  }
    void WR_Disable()   { PinClear(EEPROM_SYS_GPIO, EEPROM_WRITE);  }
    void HOLD_On()      { PinClear(EEPROM_SYS_GPIO, EEPROM_HOLD);   }
    void HOLD_Off()     { PinSet  (EEPROM_SYS_GPIO, EEPROM_HOLD);   }
    void CS_Hi()        { PinSet  (EEPROM_GPIO, EEPROM_CS);         }
    void CS_Lo()        { PinClear(EEPROM_GPIO, EEPROM_CS);         }
    void MOSI_Hi()      { PinSet  (EEPROM_GPIO, EEPROM_MOSI);       }
    void MOSI_Lo()      { PinClear(EEPROM_GPIO, EEPROM_MOSI);       }
    void SCLK_Hi()      { PinSet  (EEPROM_GPIO, EEPROM_SCK);        }
    void SCLK_Lo()      { PinClear(EEPROM_GPIO, EEPROM_SCK);        }
    bool isMISO_Hi()    { return PinIsSet(EEPROM_GPIO, EEPROM_MISO);}

    void WriteByte(uint8_t AByte);
    uint8_t ReadByte();
    uint8_t WriteReadByte(uint8_t AByte);

    uint32_t readU32();
public:
    void Init();
    Rslt_t ReadConf();
};

extern eeprom_t EE;

#endif /* INC_EEPROM_H_ */
