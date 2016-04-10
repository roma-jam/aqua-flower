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
}

Rslt_t eeprom_t::ReadConf()
{
    Uart.Printf("Read Conf\r");
    return OK;
}
