#pragma once
#include <Arduino.h>

/*
   Ultrasonic distance sensor library based on the HC-SR04
*/

void _echo_in();

class Ultrasonic {
  public:
    Ultrasonic(int pin_echo, int pin_trig);
    void setup();
    void update();
    void echo_in();
    int distance_cm();
    static Ultrasonic* _instance;

  private:
    long microsecondsToCentimeters(long microseconds);
    int _pin_echo, _pin_trig, _state, _distance_cm;
    long _last_micros;
    bool _wait;
};
