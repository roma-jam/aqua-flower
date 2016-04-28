/*
 * lcd.h
 *
 *  Created on: 27.10.2013
 *      Author: Roma Jam
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdarg.h>
#include "kl_lib_f100.h"
#include "lcd_font.h"
#include "clock_digits.h"
#include "clock.h"
#include "config.h"

//#define ENABLE_DMA_MODE

#define REFRESH_TIME_MS             121

// ==== GPIOS ====
//#define LCD_SPI                   SPI2
#define LCD_TIM                     TIM2
#define LCD_GPIO		            GPIOB

#define LCD_BCKLT                   10
#define LCD_RESET                   11
#define LCD_CE                      12
#define LCD_SCLK                    13
#define LCD_DC                      14
#define LCD_DIN                     15

#define LCD_STR_HEIGHT              8
#define LCD_STR_WIDTH               16

#define LCD_WIDTH                   84
#define LCD_HEIGHT                  48
// Data sizes
#define LCD_VIDEOBUF_SIZE           (uint32_t)((LCD_WIDTH*LCD_HEIGHT) >> 3)     // =  84*48/8

#define LCD_DEFAULT_BACKLIGTH       15

#ifdef LCD_SPI
#define SPI_MODE    SPI_CR1_SSM |       /* NSS Software Managment Enable */    \
                    SPI_CR1_SSI |       /* Internal slave select */            \
                    SPI_CR1_CPHA |      /* 2 Edge */                           \
                    SPI_CR1_MSTR        /* Master Configuration */             \

enum brDiv_t {
    brDiv2   = 0b000,
    brDiv4   = 0b001,
    brDiv8   = 0b010,
    brDiv16  = 0b011,
    brDiv32  = 0b100,
    brDiv64  = 0b101,
    brDiv128 = 0b110,
    brDiv256 = 0b111
};
#endif

// LCD Commands
#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

enum PseudoGraph_t {
    CornerTopLeftDouble = 0x99,
    CornerTopRightDouble = 0x8B,
    CornerBottomLeftDouble = 0x98,
    CornerBottomRightDouble = 0x8C,
    LineHorizDouble = 0x9D,
    LineHorizDoubleUp = 0x9A,
    LineHorizDoubleDown = 0x9B,
    LineVertDouble = 0x8A,
    LineVertDoubleLeft = 0x89,
    LineVertDoubleRight = 0x9C,
    LineCrossDouble = 0x9E,
};

enum DrawMode_t {
	DRAW, // zero has no effect
	CLEAR, // zero has no effect, one clears
	OVERWRITE, // write both zeros and ones
	OVERWRITE_INVERTED, // write both with negated meaning
	INVERT // zero has no effect, one inverts
};

extern void LcdBackligthTmr(void *p);

class Lcd_t {
private:
    VirtualTimer Timer;
    uint16_t IBuf[LCD_VIDEOBUF_SIZE];
    bool ShouldUpdate;
    bool toggle;
    DrawMode_t DrawMode;
    // printf functions
    int __vsprintf(char *buf, const char *format, va_list args);
#ifdef LCD_SPI
    inline void SPI_SetBaud(brDiv_t brDiv)  { LCD_SPI->CR1 |= brDiv << 3;        }
#endif
    // Pin driving functions
    inline void InitGpios() {
        PinSetupOut(LCD_GPIO, LCD_RESET, omPushPull, ps50MHz);
        PinSetupOut(LCD_GPIO, LCD_CE, omPushPull, ps50MHz);
        PinSetupOut(LCD_GPIO, LCD_DC, omPushPull, ps50MHz);
        PinSetupAlterFuncOutput(LCD_GPIO, LCD_BCKLT, omPushPull, ps50MHz);
#ifdef LCD_SPI
        PinSetupAlterFuncOutput(LCD_GPIO, LCD_SCLK, omPushPull, ps50MHz);
        PinSetupAlterFuncOutput(LCD_GPIO, LCD_DIN, omPushPull, ps50MHz);
#else
        PinSetupOut(LCD_GPIO, LCD_SCLK, omPushPull, ps50MHz);
        PinSetupOut(LCD_GPIO, LCD_DIN, omPushPull, ps50MHz);
#endif
    }

    void RESET_Hi(void) { PinSet(LCD_GPIO, LCD_RESET);    }
    void RESET_Lo(void) { PinClear(LCD_GPIO, LCD_RESET);  }
    void DataMode(void){ PinSet(LCD_GPIO, LCD_DC);      }
    void CmdMode(void) { PinClear(LCD_GPIO, LCD_DC);    }
    void SCLK_Hi(void) { PinSet(LCD_GPIO, LCD_SCLK);    }
    void SCLK_Lo(void) { PinClear(LCD_GPIO, LCD_SCLK);  }
    void DIN_Hi (void) { PinSet(LCD_GPIO, LCD_DIN);     }
    void DIN_Lo (void) { PinClear(LCD_GPIO, LCD_DIN);   }
    void WriteCmd(uint8_t ACmd);
    void WriteData(uint8_t AData);
    // High-level
    void DrawBlock(uint32_t index, uint8_t data, uint8_t mask);
    void DrawChar(uint32_t *index, uint8_t AChar);

    // Clock
    void ClearDelimeter();
    void DrawDelimeter();
    void DrawClockDigit(uint8_t Pos, uint8_t Digit);
    void DelimeterToggle();
    void Backlight(uint8_t ABrightness)  { LCD_TIM->CCR3 = ABrightness; }

public:
    // IRQ
    void CS_Hi (void) { PinSet(LCD_GPIO, LCD_CE);     }
    void CS_Lo (void) { PinClear(LCD_GPIO, LCD_CE);   }
    // General use
    void Init(void);
    void Task(void);
    void Shutdown(void);


    void BacklightOn()
    {
        Backlight(LCD_DEFAULT_BACKLIGTH);
        chVTReset(&Timer);
        chVTSet(&Timer, MS2ST(LCD_DEFAULT_BACKLIGTH_MS), LcdBackligthTmr, nullptr);
    }
    void BacklightOff() { Backlight(0); }


    // High-level
    void SetDrawMode(DrawMode_t mode) { DrawMode = mode; }
    void SetX(uint8_t newX) { WriteCmd(PCD8544_SETXADDR | newX); }
    void SetY(uint8_t newY) { WriteCmd(PCD8544_SETYADDR | newY); }
    void Printf(uint32_t column, uint32_t row, const char *S, ...);
    void DrawImage(uint32_t x, uint32_t y, const uint8_t *img);

    void Cls(void) { for(uint16_t i=0; i < LCD_VIDEOBUF_SIZE; i++) IBuf[i] = 0x0000; }

    /* ==== Pseudographics ====
     *  Every command consists of PseudoGraph_t AChar, uint8_t RepeatCount.
     *  Example: LineHorizDouble, 11 => print LineHorizDouble 11 times.
     *  Cmds end up with 0.
     */
    void Symbols(int column, int row, ...);

    void PutPixel(int x, int y, int c) {
//    	assert(c == 0 || c == 1);
    	int index = x + (y/8)*LCD_WIDTH;
    	uint8_t mask = 1 << (7-y%8);
		DrawBlock(index, c ? mask : 0, mask);
    }

    friend class clock_t;
};

extern Lcd_t Lcd;
#endif /* LCD_H_ */
