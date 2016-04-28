/*
 * application_config.h
 *
 *  Created on: 27 апр. 2016 г.
 *      Author: RomaJam
 */

#ifndef APPLICATION_CONFIG_H_
#define APPLICATION_CONFIG_H_

// ============================ EEPROM & CONFIG ================================
#define EEPROM_READ_WRITE_TEST              0

#if (EEPROM_READ_WRITE_TEST)
#define EEPROM_TEST_BYTE                    0x5DADFACE
#define EEPROM_TEST_ADDR                    0x100
#endif

#define APP_EEPROM_CONFIG_ADDR              0

// ============================== LCD & GRAPHICS ===============================

#define APP_SCREENSAVER_TIMEOUT_MS          9999

#define APP_HEADER_POSITION_X               0
#define APP_HEADER_POSITION_Y               0

#define APP_BOTTOM_POSITION_X               0
#define APP_BOTTOM_POSITION_1Y              4
#define APP_BOTTOM_POSITION_2Y              5


#define APP_MENU_POSITION                   5
#define APP_TIME_POSITION                   2
#define APP_PUMP_POSITION                   3

#define APP_PUMP1_POSITIONS                 2
#define APP_PUMP2_POSITIONS                 3

#define APP_LINE_1_POSITION                 1
#define APP_LINE_2_POSITION                 2
#define APP_LINE_3_POSITION                 3

#define LCD_DEFAULT_BACKLIGTH_MS            4999

// ================================ DEBUG  =====================================

#define APP_WATER_PUMP_DEBUG                0
#define APP_CLOCK_DEBUG                     1
#define APP_EEPROM_DEBUG                    1

#endif /* APPLICATION_CONFIG_H_ */
