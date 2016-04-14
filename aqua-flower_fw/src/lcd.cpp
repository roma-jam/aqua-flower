/*
 * lcd.cpp
 *
 *  Created on: 27.10.2013
 *      Author: Roma Jam
 */

#include "lcd.h"
Lcd_t Lcd;

// =============================== Thread ======================================
static WORKING_AREA(waLcdThread, 128);
__attribute__ ((__noreturn__))
static void LcdThread(void *arg)
{
    chRegSetThreadName("LcdTask");
    while(1)
        Lcd.Task();
}


#ifdef ENABLE_DMA_MODE
#define LCD_DMA             STM32_DMA1_STREAM5
#define LCD_DMA_MODE        DMA_PRIORITY_LOW |                              \
                            STM32_DMA_CR_MSIZE_HWORD | /* Size byte */          \
                            STM32_DMA_CR_PSIZE_HWORD |                          \
                            STM32_DMA_CR_MINC |    /* Mem pointer increase */  \
                            STM32_DMA_CR_DIR_M2P | /* Mem to peripheral */     \
                            STM32_DMA_CR_TCIE      /* Enable Transmission Int C */ \
                            STM32_DMA_CR_CIRC       /* Circular */

// ==== Inner DMA ====
extern "C"
{
    void LcdDmaCompIrq(void *p, uint32_t flags);
}
#endif

void Lcd_t::Task() {

//    for (uint16_t i = LCD_VIDEOBUF_SIZE; i > 0; i--)
    if(ShouldUpdate)
    {
        for (uint16_t i = 0; i < LCD_VIDEOBUF_SIZE; i++)
            WriteData(IBuf[i]);

        SetX(0);
        SetY(0);
        ShouldUpdate = false;
    }
    else
    chThdSleepMilliseconds(REFRESH_TIME_MS);
}

void Lcd_t::Init()
{
    InitGpios();
    // ==== Backlight: Timer2 Ch3 ====
    AFIO->MAPR |= (2 << 8); // Remap Timer2 to PB10
    // Setup timer2
    rccEnableAPB1(RCC_APB1ENR_TIM2EN, false);

    LCD_TIM->CR1 = 0x01;      // Enable timer
    LCD_TIM->CR2 = 0;
    LCD_TIM->PSC = 0;         // Do not divide input freq
    LCD_TIM->ARR = 100;       // Autoreload register: full brightness=100
    LCD_TIM->BDTR = 0xC000;   // Main output Enable
    LCD_TIM->CCMR2 = 0x0060;  // PWM mode1 on Ch3 enabled
    LCD_TIM->CCER = 0x0100;   // Output3 enabled
    LCD_TIM->CCER |= (1 << 9);     // polarity inverted

#ifdef LCD_SPI
    rccEnableSPI2(FALSE);
    LCD_SPI->CR1 = SPI_MODE;
    LCD_SPI->CR2  = 0;
    LCD_SPI->SR   = 0x02;
    SPI_SetBaud(brDiv16); // Set baudrate: Fpclk/16 = 48 MHz/16 = 3 Mhz NOTE: max freq 5MHz
    LCD_SPI->CR1 |=  SPI_CR1_SPE; // ENABLE
#endif

    Backlight(15);
    RESET_Hi();
    CS_Hi();
    SCLK_Hi();
    chThdSleepMilliseconds(9);
    // ========================= Init LCD ======================================
    // Reset display again
    RESET_Lo();
    chThdSleepMilliseconds(99);
    RESET_Hi();

    WriteCmd(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);    // display ON
    WriteCmd(PCD8544_SETBIAS | 0x04);
    WriteCmd(PCD8544_SETVOP | 0x31);
    WriteCmd(PCD8544_FUNCTIONSET);
    WriteCmd(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

    Cls();             // clear LCD buffer

    ShouldUpdate = false;
    toggle = true;
    DrawMode = OVERWRITE;

// ====================== Switch to USART + DMA ============================
#ifdef ENABLE_DMA_MODE
    LCD_SPI->CR2 |= SPI_CR2_TXDMAEN;

    // ==== DMA ====
    rccEnableDMA1();
    dmaStreamAllocate(LCD_DMA, IRQ_PRIO_MEDIUM, LcdDmaCompIrq, NULL);
    dmaStreamSetPeripheral(LCD_DMA, &LCD_SPI->DR);
    dmaStreamSetMemory0(LCD_DMA, (uint32_t)&IBuf[0]);
    dmaStreamSetMode(LCD_DMA, LCD_DMA_MODE);

    // Start transmission
    dmaStreamSetMemory0(LCD_DMA, (uint32_t)&IBuf[0]);
    dmaStreamSetTransactionSize(LCD_DMA, LCD_VIDEOBUF_SIZE);
    dmaStreamEnable(LCD_DMA);

#else
    for(uint16_t i = 0; i < LCD_VIDEOBUF_SIZE; i++)
        WriteData(0x00);
#endif

    chThdCreateStatic(waLcdThread, sizeof(waLcdThread), NORMALPRIO, (tfunc_t)LcdThread, NULL);
}

void Lcd_t::Shutdown(void) {
#ifdef ENABLE_DMA_MODE
    dmaStreamDisable(LCD_DMA);
#endif
    RESET_Lo();
    CS_Lo();
    SCLK_Lo();
    DIN_Lo();
    Backlight(0);
}

void Lcd_t::WriteCmd(uint8_t AByte)
{
    CS_Lo();
    CmdMode();
#ifdef LCD_SPI
    LCD_SPI->DR = AByte;
    while (!(LCD_SPI->SR & SPI_SR_TXE));
    while (LCD_SPI->SR & SPI_SR_BSY);
#else
    for(uint8_t i=0; i<8; i++)
    {
        SCLK_Lo();
        if(AByte & 0x80)
            DIN_Hi();
        else
            DIN_Lo();
        AByte <<= 1;
        SCLK_Hi();
    }
#endif

    CS_Hi();
}

void Lcd_t::WriteData(uint8_t AByte)
{
    CS_Lo();
    DataMode();
#ifdef LCD_SPI
    LCD_SPI->DR = AByte;
    while (!(LCD_SPI->SR & SPI_SR_TXE));
    while (LCD_SPI->SR & SPI_SR_BSY);
#else
    for(uint8_t i=0; i<8; i++)
    {
        SCLK_Lo();
        if(AByte & 0x01)
            DIN_Hi();
        else
            DIN_Lo();
        AByte >>= 1;
        SCLK_Hi();
    }
#endif
    CS_Hi();
}


void Lcd_t::Symbols(int column, int row, ...) {
    uint32_t index = column*6 + row*LCD_WIDTH;
    uint8_t FCharCode=1, RepeatCount;
    va_list Arg;
    va_start(Arg, row);    // Set pointer to last argument
    while(1) {
        FCharCode = (uint8_t)va_arg(Arg, int32_t);
        if(FCharCode == 0) break;
        RepeatCount = (uint8_t)va_arg(Arg, int32_t);
        for(uint8_t j=0; j<RepeatCount; j++) DrawChar(&index, FCharCode);
    }
    va_end(Arg);
}


void Lcd_t::Printf(uint32_t column, uint32_t row, const char *format, ...) {
    char buf[LCD_STR_HEIGHT*LCD_STR_WIDTH+1];
    va_list args;
    va_start(args, format);
    __vsprintf(buf, format, args);
    va_end(args);

    uint32_t index = column*6 + row*LCD_WIDTH;
    for (int i = 0; buf[i] != 0; i++)
        DrawChar(&index, buf[i]);

    ShouldUpdate = true;
}

// ================================ Graphics ===================================
void Lcd_t::DrawChar(uint32_t *index, uint8_t AChar) {
    for(uint8_t i=0; i < 6; i++) {
        DrawBlock((*index)++, Font_6x8_Data[AChar][i], 0xFF);
        if (*index >= LCD_VIDEOBUF_SIZE) *index = 0;
    }
}

void Lcd_t::DrawImage(uint32_t x, uint32_t y, const uint8_t* img) {
//    assert(y % 8 == 0);
    y /= 8; // y is now rows, not pixels
    uint8_t width = *img++;
    uint8_t height = *img++;
    for(uint32_t fy = y; fy < y+height; fy++) {
        uint32_t index = x + fy*LCD_WIDTH;
        for(uint32_t fx = x; fx < x+width; fx++) {
            DrawBlock(index++, *img++, 0xFF);
            if (index > LCD_VIDEOBUF_SIZE) break;
        }
    }

    ShouldUpdate = true;
}

void Lcd_t::DrawClockDigit(uint8_t Pos, uint8_t Digit)
{
    uint32_t index = LCD_WIDTH + (CLOCK_DIGIT_WIDTH * Pos);
    if(Pos > 1)
        index += CLOCK_DELIMETER_WIDTH;
    for (uint8_t j = 0; j < CLOCK_DIGIT_HEIGTH; j++)
    {
        for (uint8_t i = 0; i < CLOCK_DIGIT_WIDTH; i++)
        {
            DrawBlock(index++, Clock_Digits[Digit][i + CLOCK_DIGIT_WIDTH*j], 0xFF);
        }
        index -= CLOCK_DIGIT_WIDTH;
        index += LCD_WIDTH;
    }
}

void Lcd_t::DelimeterToggle()
{
    if(toggle)
    {
        DrawDelimeter();
        toggle = false;
    }
    else
    {
        ClearDelimeter();
        toggle = true;
    }

    ShouldUpdate = true;
}

void Lcd_t::ClearDelimeter()
{
    DrawMode = CLEAR;
    DrawDelimeter();
    DrawMode = OVERWRITE;
}

void Lcd_t::DrawDelimeter()
{
    uint32_t index = LCD_WIDTH + (CLOCK_DIGIT_WIDTH * 2);
    for (uint8_t j = 0; j < CLOCK_DIGIT_HEIGTH; j++)
    {
        for (uint8_t i = 0; i < CLOCK_DELIMETER_WIDTH; i++)
        {
            DrawBlock(index++, Delimeter[i + CLOCK_DELIMETER_WIDTH*j], 0xFF);
        }
        index -= CLOCK_DELIMETER_WIDTH;
        index += LCD_WIDTH;
    }
}

void Lcd_t::DrawBlock(uint32_t index, uint8_t data, uint8_t mask) {
//    assert((data & ~mask) == 0);
    uint16_t *w = &IBuf[index];
#ifdef ENABLE_DMAUSART_MODE
    uint16_t data2 = data << 1;
    uint16_t mask2 = mask << 1;
#else
    uint16_t data2 = data;
    uint16_t mask2 = mask;
#endif
    switch (DrawMode) {
        case DRAW:                  *w |= data2;                            break;
        case CLEAR:                 *w &= ~data2;                           break;
        case OVERWRITE:             *w = (*w & ~mask2) | data2;             break;
        case OVERWRITE_INVERTED:    *w = (*w & ~mask2) | (data2 ^ mask2);    break;
        case INVERT:                *w ^= data2;                            break;
    }
}

// inner printf lcd
static inline char *put_uint(char *p,
               unsigned int n,
               unsigned int base, int width, bool zero_padded) {
    char digits[10];
    int len = 0;
    do {
        unsigned int digit = n%base;
        n /= base;
        digits[len++] = digit < 10 ? '0'+digit : 'A'+digit-10;
    } while (n > 0);

    for (int i = len; i < width; i++)
        if (zero_padded)
            *p++ = '0';
        else
            *p++ = ' ';

    while (len > 0)
        *p++ = digits[--len];
    return p;
}

int Lcd_t::__vsprintf(char *buf, const char *format, va_list args)
{
    const char *f = format;
        char *p = buf;
        char c;
        while ((c = *f++) != 0) {
            if (c != '%') {
                *p++ = c;
                continue;
            }

            // Here goes optional width specification.
            // If it starts with zero (zero_padded is true), it means we use '0'
            // instead of ' ' as a filler.
            int width = 0;
            bool zero_padded = false;
            while (true) {
                c = *f++;
                if (c < '0' || c > '9')
                    break;
                if (width == 0 && c == '0')
                    zero_padded = true;
                width *= 10;
                width += c-'0';
            }

            if (c == 's') {
                char *s = va_arg(args, char*);
                while (*s != 0)
                    *p++ = *s++;
            }
            else if (c == 'c') {
                *p++ = va_arg(args, int);
            }
            else if (c == 'X') {
                unsigned int n = va_arg(args, unsigned int);
                p = put_uint(p, n, 16, width, zero_padded);
            }
            else if (c == 'u') {
                unsigned int n = va_arg(args, unsigned int);
                p = put_uint(p, n, 10, width, zero_padded);
            }
            else if (c == 'd') {
                int n = va_arg(args, int);
                if (n < 0) {
                    *p++ = '-';
                    n = -n;
                }
                p = put_uint(p, n, 10, 0, false);
            }
            else if (c == 'A') {
                uint8_t *arr = va_arg(args, uint8_t*);
                int n = va_arg(args, int);
                for (int i = 0; i < n; i++) {
                    if (i > 0)
                        *p++ = ' ';
                    p = put_uint(p, arr[i], 16, 2, true);
                }
            }
        }
        *p = 0;
        return p-buf;
}
