#pragma once
#include <Arduino.h>

/*
   Dual 7 segment display interface to display up to two characters at once using shift-registers
*/

#define DISP_A B11111011
#define DISP_B B00111111
#define DISP_C B00011011
#define DISP_D B01111011
#define DISP_E B10011111
#define DISP_F B10001111
#define DISP_G B10111101
#define DISP_H B00101111
#define DISP_I B00001101
#define DISP_J B01110001
#define DISP_K B10101111
#define DISP_L B00011101
#define DISP_M B10101011
#define DISP_N B00101011
#define DISP_O B00111011
#define DISP_P B11001111
#define DISP_Q B11100111
#define DISP_R B00001011
#define DISP_S B10110101
#define DISP_T B00011111
#define DISP_U B00111001
#define DISP_V B01010101
#define DISP_W B01010111
#define DISP_X B00101001
#define DISP_Y B01110111
#define DISP_Z B11011001

#define DISP_0 B11111100
#define DISP_1 B01100000
#define DISP_2 B11011010
#define DISP_3 B11110010
#define DISP_4 B01100110
#define DISP_5 B10110110
#define DISP_6 B00111110
#define DISP_7 B11100000
#define DISP_8 B11111110
#define DISP_9 B11100110

#define DISP_DASH       B00000010
#define DISP_UNDERSCORE B00010000
#define DISP_DOT        B00000001
#define DISP_NULL       B00000000

class BCD {
  public:
    BCD(int pin_data, int pin_latch, int pin_clock);
    void setup();
    void write(String text);
    int _char_to_bcd(char character);
    void _push_byte(int character);

  private:
    int _pin_data, _pin_latch, _pin_clock;
};
