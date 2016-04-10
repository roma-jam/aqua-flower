/*
 * clock_digits.h
 *
 *  Created on: 10 ���. 2016 �.
 *      Author: RomaJam
 */

#ifndef INC_CLOCK_DIGITS_H_
#define INC_CLOCK_DIGITS_H_


#define CLOCK_DIGIT_HEIGTH      3
#define CLOCK_DIGIT_WIDTH       20
#define CLOCK_DELIMETER_WIDTH   4

const uint8_t Clock_Digits[10][CLOCK_DIGIT_HEIGTH * CLOCK_DIGIT_WIDTH] = {
        { // 0
            0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3C, 0x70, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x70, 0x3C, 0x0F, 0x03, 0x01, 0x00, 0x00,
            0x00, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3C, 0x00,
            0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0x3C, 0x0E, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0E, 0x3C, 0xF0, 0xC0, 0x80, 0x00, 0x00
        },
        { // 1
            0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0x3C, 0x0E, 0x07, 0x07, 0x0E, 0x3C, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3C, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3C, 0x70, 0xE0, 0xE0, 0x70, 0x3C, 0x0F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00
        },
        { // 2
            0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0x3C, 0x0E, 0x07, 0x07, 0x0E, 0x3C, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3C, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3C, 0x70, 0xE0, 0xE0, 0x70, 0x3C, 0x0F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00
        },
        {  // 3
            0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0x3C, 0x0E, 0x07, 0x07, 0x0E, 0x3C, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3C, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3C, 0x70, 0xE0, 0xE0, 0x70, 0x3C, 0x0F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00
        },
        {
            // 4
            0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0x3C, 0x0E, 0x07, 0x07, 0x0E, 0x3C, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3C, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3C, 0x70, 0xE0, 0xE0, 0x70, 0x3C, 0x0F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00
        },
        {
            // 5
            0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0x3C, 0x0E, 0x07, 0x07, 0x0E, 0x3C, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3C, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3C, 0x70, 0xE0, 0xE0, 0x70, 0x3C, 0x0F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00
        },
        {
            // 6
            0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0x3C, 0x0E, 0x07, 0x07, 0x0E, 0x3C, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3C, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3C, 0x70, 0xE0, 0xE0, 0x70, 0x3C, 0x0F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00
        },
        {
            // 7
            0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0x3C, 0x0E, 0x07, 0x07, 0x0E, 0x3C, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3C, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3C, 0x70, 0xE0, 0xE0, 0x70, 0x3C, 0x0F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00
        },
        {
            // 8
            0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0x3C, 0x0E, 0x07, 0x07, 0x0E, 0x3C, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3C, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3C, 0x70, 0xE0, 0xE0, 0x70, 0x3C, 0x0F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00
        },
        {
            // 9
            0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0x3C, 0x0E, 0x07, 0x07, 0x0E, 0x3C, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3C, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x3C, 0x70, 0xE0, 0xE0, 0x70, 0x3C, 0x0F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00
        }
};

const uint8_t Delimeter[CLOCK_DIGIT_HEIGTH * CLOCK_DELIMETER_WIDTH] =
{
        0x00, 0x01, 0x01, 0x00,
        0x00, 0xC3, 0xC3, 0x00,
        0x00, 0x80, 0x80, 0x00
};

#endif /* INC_CLOCK_DIGITS_H_ */
