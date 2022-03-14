#pragma once
#include <Arduino.h>

/*
   Infrared line sensor library for 5-pins line sensor array

   --------------------------
   | LL | LM | MM | MR | RR |
   --------------------------
*/

class LineSensor {
  public:
    LineSensor(int pin_ll, int pin_lm, int pin_mm, int pin_mr, int pin_rr);
    void setup();
    void update();
    void invert(bool invert);
    bool hasLine();
    bool readLL();
    bool readLM();
    bool readMM();
    bool readMR();
    bool readRR();
    String toString();
    String toPrettyString();

  private:
    int _pin_ll, _pin_lm, _pin_mm, _pin_mr, _pin_rr;
    bool _invert, det_ll, det_lm, det_mm, det_mr, det_rr;
};
