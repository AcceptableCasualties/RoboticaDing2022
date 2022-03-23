#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int pin_echo, int pin_trig) {
  _pin_echo = pin_echo;
  _pin_trig = pin_trig;
}

void Ultrasonic::setup() {
  pinMode(_pin_echo, INPUT);
  pinMode(_pin_trig, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(_pin_echo), _echo_in, HIGH);
  _instance = this;
}

void Ultrasonic::update() {
  if (!_wait) {
    digitalWrite(_pin_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(_pin_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_pin_trig, LOW);
    _wait = true;
    _last_micros = micros();
  }
}

void Ultrasonic::echo_in() {
  if (!_wait) return;
  _state = _state == 1 ? 0 : 1;
  if (_state == 1) {
    return;
  }
  float difference = (micros() - _last_micros);
  _distance_cm = difference / (BASE_MULTIPLIER + (US_AT_30CM - difference < 0 ? 0 : US_AT_30CM - difference) / 20);
  _wait = false;
}

Ultrasonic* Ultrasonic::_instance = NULL;

void _echo_in() {
  Ultrasonic::_instance->echo_in();
}

float Ultrasonic::distance_cm() {
  return _distance_cm;
}
