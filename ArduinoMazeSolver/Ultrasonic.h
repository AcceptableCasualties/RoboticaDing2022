#pragma once
#include <Arduino.h>

/*
   Ultrasonic distance sensor library based on the HC-SR04
*/

void _echo_in();

#define US_AT_30CM      2210
#define BASE_MULTIPLIER 73.67

class Ultrasonic {
  public:
    Ultrasonic(int pin_echo, int pin_trig);
    void setup();
    void update();
    void echo_in();
    float distance_cm();
    static Ultrasonic* _instance;

  private:
    long microsecondsToCentimeters(long microseconds);
    int _pin_echo, _pin_trig, _state;
    float _distance_cm;
    long _last_micros;
    bool _wait;
};
