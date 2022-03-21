#include "BCD.h"

BCD::BCD(int pin_data, int pin_latch, int pin_clock) {
  _pin_data = pin_data;
  _pin_latch = pin_latch;
  _pin_clock = pin_clock;
}

void BCD::setup() {
  pinMode(_pin_data, OUTPUT);
  pinMode(_pin_latch, OUTPUT);
  pinMode(_pin_clock, OUTPUT);
}

void BCD::write(String text) {
  if (text.length() != 2) {
    this->_push_byte(DISP_DASH);
    this->_push_byte(DISP_DASH);
  }
  text.toLowerCase();

  this->_push_byte(this->_char_to_bcd(text[1]));
  this->_push_byte(this->_char_to_bcd(text[0]));
}

int BCD::_char_to_bcd(char character) {
  switch (character) {
    case 'a':
      return DISP_A;
    case 'b':
      return DISP_B;
    case 'c':
      return DISP_C;
    case 'd':
      return DISP_D;
    case 'e':
      return DISP_E;
    case 'f':
      return DISP_F;
    case 'g':
      return DISP_G;
    case 'h':
      return DISP_H;
    case 'i':
      return DISP_I;
    case 'j':
      return DISP_J;
    case 'k':
      return DISP_K;
    case 'l':
      return DISP_L;
    case 'm':
      return DISP_M;
    case 'n':
      return DISP_N;
    case 'o':
      return DISP_O;
    case 'p':
      return DISP_P;
    case 'q':
      return DISP_Q;
    case 'r':
      return DISP_R;
    case 's':
      return DISP_S;
    case 't':
      return DISP_T;
    case 'u':
      return DISP_U;
    case 'v':
      return DISP_V;
    case 'w':
      return DISP_W;
    case 'x':
      return DISP_X;
    case 'y':
      return DISP_Y;
    case 'z':
      return DISP_Z;
    case '0':
      return DISP_0;
    case '1':
      return DISP_1;
    case '2':
      return DISP_2;
    case '3':
      return DISP_3;
    case '4':
      return DISP_4;
    case '5':
      return DISP_5;
    case '6':
      return DISP_6;
    case '7':
      return DISP_7;
    case '8':
      return DISP_8;
    case '9':
      return DISP_9;
    case '-':
      return DISP_DASH;
    default:
      return DISP_NULL;
  }
}

void BCD::_push_byte(int character) {
  digitalWrite(_pin_latch, LOW);
  shiftOut(_pin_data, _pin_clock, LSBFIRST, character);
  digitalWrite(_pin_latch, HIGH);
}
