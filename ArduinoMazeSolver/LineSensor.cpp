#include "LineSensor.h"

LineSensor::LineSensor(int pin_ll, int pin_lm, int pin_mm, int pin_mr, int pin_rr) {
  _pin_ll = pin_ll;
  _pin_lm = pin_lm;
  _pin_mm = pin_mm;
  _pin_mr = pin_mr;
  _pin_rr = pin_rr;
}

void LineSensor::setup() {
  pinMode(_pin_ll, INPUT);
  pinMode(_pin_lm, INPUT);
  pinMode(_pin_mm, INPUT);
  pinMode(_pin_mr, INPUT);
  pinMode(_pin_rr, INPUT);
}

void LineSensor::update() {
  /*
     !(*portInputRegister(digitalPinToPort(PIN_NUM)) & digitalPinToBitMask(PIN_NUM))
        2.4us

     !digitalRead(PIN_NUM)
        4.8us
  */

  det_ll = !(*portInputRegister(digitalPinToPort(_pin_ll)) & digitalPinToBitMask(_pin_ll));// !digitalRead(_pin_ll);
  det_lm = !(*portInputRegister(digitalPinToPort(_pin_lm)) & digitalPinToBitMask(_pin_lm));// !digitalRead(_pin_lm);
  det_mm = !(*portInputRegister(digitalPinToPort(_pin_mm)) & digitalPinToBitMask(_pin_mm));// !digitalRead(_pin_mm);
  det_mr = !(*portInputRegister(digitalPinToPort(_pin_mr)) & digitalPinToBitMask(_pin_mr));// !digitalRead(_pin_mr);
  det_rr = !(*portInputRegister(digitalPinToPort(_pin_rr)) & digitalPinToBitMask(_pin_rr));// !digitalRead(_pin_rr);

  if (_invert) {
    det_ll = !det_ll;
    det_lm = !det_lm;
    det_mm = !det_mm;
    det_mr = !det_mr;
    det_rr = !det_rr;
  }
}

void LineSensor::invert(bool invert) {
  _invert = invert;
}

bool LineSensor::hasLine() {
  return _invert ? (det_ll || det_lm || det_mm || det_mr || det_rr) : !(det_ll && det_lm && det_mm && det_mr && det_rr);
}

bool LineSensor::readLL() {
  return det_ll;
}

bool LineSensor::readLM() {
  return det_lm;

}

bool LineSensor::readMM() {
  return det_mm;

}

bool LineSensor::readMR() {
  return det_mr;

}

bool LineSensor::readRR() {
  return det_rr;

}


String LineSensor::toString() {
  return String(det_ll) + String(det_lm) + String(det_mm) + String(det_mr) + String(det_rr);
}

String LineSensor::toPrettyString() {
  String output = toString();
  output.replace("0", "-");
  output.replace("1", "*");
  return output;

  //return String(det_ll ? "*" : "-") + String(det_lm ? "*" : "-") + String(det_mm ? "*" : "-") + String(det_mr ? "*" : "-") + String(det_rr ? "*" : "-");
}
