/*
 * eeprom.cpp
 *
 *  Created on: 03 апр. 2015 г.
 *      Author: RomaJam
 */


#include "eeprom.h"

eeprom_t EE;

void eeprom_t::Init() {
    Uart.Printf("EE init\r");
    InitGpios();

    Test();
}

bool eeprom_t::Test()
{
#ifdef EEPROM_READ_WRITE_TEST
    uint32_t TestByte = EEPROM_TEST_BYTE;
    writeU32(EEPROM_TEST_ADDR, TestByte);
    readU32(EEPROM_TEST_ADDR, &TestByte);
    if(TestByte != EEPROM_TEST_BYTE)
    {
        Uart.Printf("EE test FAILURE\r\n");
        return false;
    }
#endif

    uint8_t Status = 0;
    CS_Lo();
    WriteReadByte(EEPROM_CMD_RDSR);
    Status = ReadByte();
    CS_Hi();
    Uart.Printf("EE read status: %X\r\n", Status);
    return true;
}


Rslt_t eeprom_t::ReadConf()
{
    Uart.Printf("Read Conf\r");
    return OK;
}

void eeprom_t::writeU32(uint16_t Addr, uint32_t AByte)
{
    CS_Lo();
    for(uint8_t i = 0; i < sizeof(uint32_t); i++)
    {
        WriteReadByte(EEPROM_CMD_WRITE); // Ins + Address MSB
        WriteReadByte(Addr + i);
        WriteReadByte((uint8_t)(AByte >> (i << 3)));
    }
    CS_Hi();
    chThdSleepMilliseconds(5); // wait EEPROM
}

void eeprom_t::readU32(uint16_t Addr, uint32_t *pByte)
{
    *pByte = 0;
    CS_Lo();
    for(uint8_t i = 0; i < sizeof(uint32_t); i++)
    {
        WriteReadByte(EEPROM_CMD_READ); // Ins + Address MSB
        WriteReadByte(Addr + i);
        *pByte <<= 8;
        *pByte |= ReadByte();
    }

    CS_Hi();
}

uint8_t eeprom_t::WriteReadByte(uint8_t AByte) {
    uint8_t res = 0;
    for (uint8_t i=0; i<8; i++) {
        if (AByte & 0x80)
            MOSI_Hi();
        else
            MOSI_Lo();
        AByte <<= 1;
        SCLK_Hi();
        res >>= 1;
        if(isMISO_Hi())
            res |= 0x80;
        SCLK_Lo();
    }
    return res;
}
