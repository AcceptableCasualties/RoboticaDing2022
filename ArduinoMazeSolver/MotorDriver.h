#pragma once
#include <Arduino.h>

class MotorDriverPWM {
  public:
    MotorDriverPWM(int pin_e1, int pin_m1, int pin_e2, int pin_m2);
    void setup();
    void flipLeftMotorDirection();
    void flipRightMotorDirection();
    void setLeftSpeed(int speed);
    void setRightSpeed(int speed);
    void stop();
  private:
    int _pin_e1, _pin_m1, _pin_e2, _pin_m2;
    bool _invert_left_motor, _invert_right_motor;
};

class MotorDriverPLL {
  public:
    MotorDriverPLL(int pin_e1, int pin_m1, int pin_e2, int pin_m2);
    void setup();
    void flipLeftMotorDirection();
    void flipRightMotorDirection();
    void setLeftSpeed(int speed);
    void setRightSpeed(int speed);
    void stop();
  private:
    int _pin_e1, _pin_m1, _pin_e2, _pin_m2;
    bool _invert_left_motor, _invert_right_motor;
};
