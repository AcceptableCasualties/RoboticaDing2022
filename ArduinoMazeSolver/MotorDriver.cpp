#include "MotorDriver.h"

MotorDriverPWM::MotorDriverPWM(int pin_e1, int pin_m1, int pin_e2, int pin_m2) {
  _pin_e1 = pin_e1;
  _pin_m1 = pin_m1;
  _pin_e2 = pin_e2;
  _pin_m2 = pin_m2;
}

void MotorDriverPWM::setup() {
  pinMode(_pin_e1, OUTPUT);
  pinMode(_pin_m1, OUTPUT);
  pinMode(_pin_e2, OUTPUT);
  pinMode(_pin_m2, OUTPUT);
}

void MotorDriverPWM::flipLeftMotorDirection() {
  _invert_left_motor = !_invert_left_motor;
}

void MotorDriverPWM::flipRightMotorDirection() {
  _invert_right_motor = !_invert_right_motor;
}

void MotorDriverPWM::setLeftSpeed(int speed) {
  if (speed == 0) {
    digitalWrite(_pin_e2, LOW);
    digitalWrite(_pin_m2, LOW);
    return;
  }

  digitalWrite(_pin_m2, speed >= 0 ? HIGH : LOW);
  analogWrite(_pin_e2, abs(speed));
}

void MotorDriverPWM::setRightSpeed(int speed) {
  if (speed == 0) {
    digitalWrite(_pin_e1, LOW);
    digitalWrite(_pin_m1, LOW);
    return;
  }
  
  digitalWrite(_pin_m1, speed >= 0 ? HIGH : LOW);
  analogWrite(_pin_e1, abs(speed));
}

void MotorDriverPWM::stop() {
  digitalWrite(_pin_e1, LOW);
  digitalWrite(_pin_m1, LOW);
  digitalWrite(_pin_e2, LOW);
  digitalWrite(_pin_m2, LOW);
}

MotorDriverPLL::MotorDriverPLL(int pin_e1, int pin_m1, int pin_e2, int pin_m2) {
  _pin_e1 = pin_e1;
  _pin_m1 = pin_m1;
  _pin_e2 = pin_e2;
  _pin_m2 = pin_m2;
}

void MotorDriverPLL::setup() {
  pinMode(_pin_e1, OUTPUT);
  pinMode(_pin_m1, OUTPUT);
  pinMode(_pin_e2, OUTPUT);
  pinMode(_pin_m2, OUTPUT);
}

void MotorDriverPLL::flipLeftMotorDirection() {
  _invert_left_motor = !_invert_left_motor;
}

void MotorDriverPLL::flipRightMotorDirection() {
  _invert_right_motor = !_invert_right_motor;
}

void MotorDriverPLL::setLeftSpeed(int speed) {
  if (speed == 0) {
    digitalWrite(_pin_e2, LOW);
    digitalWrite(_pin_m2, LOW);
    return;
  }
  speed = speed < -253 ? -253 : speed;
  speed = speed > 255 ? 255 : speed;
  speed = map(speed, -255, 255, 0, 255);

  digitalWrite(_pin_e2, LOW);
  digitalWrite(_pin_e2, HIGH);
  analogWrite(_pin_m2, _invert_left_motor ? -speed : speed);
}

void MotorDriverPLL::setRightSpeed(int speed) {
  if (speed == 0) {
    digitalWrite(_pin_e1, LOW);
    digitalWrite(_pin_m1, LOW);
    return;
  }
  speed = speed < -253 ? -253 : speed;
  speed = speed > 255 ? 255 : speed;
  speed = map(speed, -255, 255, 0, 255);

  digitalWrite(_pin_e1, LOW);
  digitalWrite(_pin_e1, HIGH);
  //  digitalWrite(_pin_e1, speed >= 0 ? HIGH : LOW);
  analogWrite(_pin_m1, _invert_right_motor ? -speed : speed);
}

void MotorDriverPLL::stop() {
  digitalWrite(_pin_e1, LOW);
  digitalWrite(_pin_m1, LOW);
  digitalWrite(_pin_e2, LOW);
  digitalWrite(_pin_m2, LOW);
}
