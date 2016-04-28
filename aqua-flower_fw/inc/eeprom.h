/*
 * eeprom.h
 *
 *  Created on: 03 апр. 2015 г.
 *      Author: RomaJam
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "kl_lib_f100.h"
#include "config.h"

#define EEPROM_SYS_GPIO     GPIOA
#define EEPROM_HOLD         2
#define EEPROM_WRITE        3

#define EEPROM_GPIO         GPIOB
#define EEPROM_CS           6
#define EEPROM_MISO         7
#define EEPROM_MOSI         8
#define EEPROM_SCK          9

#define EEPROM_CMD_WRSR     0x01
#define EEPROM_CMD_WRITE    0x02
#define EEPROM_CMD_READ     0x03
#define EEPROM_CMD_WRDI     0x04
#define EEPROM_CMD_RDSR     0x05
#define EEPROM_CMD_WREN     0x06

#define EEPROM_PAGE_READ    0x80
#define EEPROM_ADDR_MASK    0x01FF;

#define EEPROM_PAGE_SZ      16 // max page size = 16 bytes

class eeprom_t {
private:
    bool _IsInit;
    void InitGpios()
    {
        PinSetupOut(EEPROM_SYS_GPIO, EEPROM_WRITE, omPushPull, ps50MHz);
        PinSetupOut(EEPROM_SYS_GPIO, EEPROM_HOLD, omPushPull, ps50MHz);
        PinSetupOut(EEPROM_GPIO, EEPROM_CS, omPushPull, ps50MHz);
        PinSetupOut(EEPROM_GPIO, EEPROM_SCK, omPushPull, ps50MHz);
        PinSetupOut(EEPROM_GPIO, EEPROM_MOSI, omPushPull, ps50MHz);
        PinSetupIn(EEPROM_GPIO, EEPROM_MISO, pudPullDown);
    }
    uint8_t WriteReadByte(uint8_t AByte);

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

    uint8_t ReadByte()  { return WriteReadByte(0x00);               }

    void WriteEnable()  { CS_Lo(); WriteReadByte(EEPROM_CMD_WREN); CS_Hi(); }
    void WriteDisable() { CS_Lo(); WriteReadByte(EEPROM_CMD_WRDI); CS_Hi(); }

    void writeU32(uint16_t Addr, uint32_t AByte);
    void readU32(uint16_t Addr, uint32_t *pByte);
#if (EEPROM_READ_WRITE_TEST)
    bool Test();
#endif

public:
    bool isInit() { return _IsInit; }
    void Init();
    Rslt_t WriteConf(uint16_t Addr, uint32_t *Conf);
    Rslt_t ReadConf(uint16_t Addr, uint32_t *Conf);

};

extern eeprom_t EE;

#endif /* INC_EEPROM_H_ */
