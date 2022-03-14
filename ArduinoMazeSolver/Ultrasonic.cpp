#include "Ultrasonic.h"

/*
 * ========== TODO ==========
 * Fix distance calculation in [void Ultrasonic::echo_in()]
 * Is ISR the right choice?
 * pulseIn is slow
 * ========== TODO ==========
 */
 
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
  _distance_cm = (micros() - _last_micros) / 29 / 4;
  _wait = false;
}

Ultrasonic* Ultrasonic::_instance = NULL;

void _echo_in() {
  Ultrasonic::_instance->echo_in();
}

int Ultrasonic::distance_cm() {
  return _distance_cm;
}
