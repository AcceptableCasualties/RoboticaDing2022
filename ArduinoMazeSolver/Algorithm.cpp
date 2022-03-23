#include "Algorithm.h"

Algorithm::Algorithm() {

}

void Algorithm::setup() {
  //  _is_turning_180 = true;
  //  _is_turning_left = true;
  //  _is_turning_right = true;
  //  _is_moving_bit_forward = true;
  //  _is_moving_bit_back = true;

}

void Algorithm::update() {
  //  if (_state == STATE_STOPPED) {
  //    Serial.println("STATE_STOPPED");
  //  }
  //  if (_state == STATE_FOLLOWING_LINE) {
  //    Serial.println("STATE_FOLLOWING_LINE");
  //  }
  if (_state == STATE_NO_LINE) {
    Serial.println("STATE_NO_LINE");
  }
//  if (_state == STATE_CONT_LINE) {
//    Serial.println("STATE_CONT_LINE");
//  }
  if (_state == STATE_ROTATE_180) {
    Serial.println("STATE_ROTATE_180");
  }
  if (_state == STATE_POS_LINE) {
    Serial.println("STATE_POS_LINE");
  }
  if (_state == STATE_RIGHT_TURN) {
    Serial.println("STATE_RIGHT_TURN");
  }
  if (_state == STATE_LEFT_TURN) {
    Serial.println("STATE_LEFT_TURN");
  }

  if (_num_crossings_passed >= 10) {
    _num_crossings_passed = 0;
  }

  _stop_motors = false;

  if (_has_reached_finish || !_initial_line_found) {
    _motor_left_speed = 0;
    _motor_right_speed = 0;
    _stop_motors = true;
    return;
  }

  if (_state == STATE_STOPPED) {
    _motor_left_speed = 0;
    _motor_right_speed = 0;
  }

  if (!handleTurning()) {
    if (!handleOffsetMovement()) {
      if (updateState()) {
        if (_measured_distance_cm < TURN_OBJECT_DISTANCE_CM) {
          _state = STATE_ROTATE_180;
        }

        if (_state == STATE_NO_LINE) {
          _state = STATE_ROTATE_180;
        }

        if (_state == STATE_LEFT_TURN) {
          _state = STATE_STOPPED;
          _is_turning_left = true;
        }

        if (_state == STATE_RIGHT_TURN) {
          _state = STATE_STOPPED;
          _is_turning_right = true;
        }

        if (_state == STATE_ROTATE_180) {
          _state = STATE_STOPPED;
          _is_turning_180 = true;
          _is_turning_left = true;
        }

        if (_state == STATE_FOLLOWING_LINE) {
          if (_line_mm || _line_lm || _line_mr) {
            if (!_line_lm && !_line_mr) {
              _motor_left_speed = MOTOR_DRIVE_SPEED;
              _motor_right_speed = MOTOR_DRIVE_SPEED;
            } else {
              if ((_line_lm && !_line_ll)) {
                _motor_left_speed = MOTOR_DRIVE_SPEED / 2 + MOTOR_DRIVE_SPEED / 3;
                _motor_right_speed = MOTOR_DRIVE_SPEED;
              }

              if ((_line_mr && !_line_rr)) {
                _motor_left_speed = MOTOR_DRIVE_SPEED;
                _motor_right_speed = MOTOR_DRIVE_SPEED / 2 + MOTOR_DRIVE_SPEED / 3;
              }
            }
          }
        }
      }
    }
  }

  _last_line_found = _line_found;
  _last_line_ll = _line_ll;
  _last_line_lm = _line_lm;
  _last_line_mm = _line_mm;
  _last_line_mr = _line_mr;
  _last_line_rr = _line_rr;
}

bool Algorithm::handleTurning() {
  if (!_is_turning_left && !_is_turning_right) {
    _last_turn_timer_micros = micros();
    _turn_180_count = 0;
  }

  if (_is_turning_left) {
    if (_turn_left_lost_line && _line_lm && !_last_line_lm) {
      _is_turning_left = _is_turning_180 ? (_turn_180_count >= 1 || micros() - _last_turn_timer_micros > 1500000 ? false : true) : false;
      _turn_180_count += 1;
      _turn_left_lost_line = false;
      _motor_left_speed = 0;
      _motor_right_speed = 0;
      _last_turn_direction = "L";
      return false;
    }

    if (!_turn_left_lost_line && !_line_lm) {
      _turn_left_lost_line = true;
    }

    _motor_left_speed = _line_lm ? -MOTOR_DRIVE_SPEED / 2 : -MOTOR_DRIVE_SPEED;
    _motor_right_speed = _line_lm ? MOTOR_DRIVE_SPEED / 2 : MOTOR_DRIVE_SPEED;
  }

  if (_is_turning_right) {
    if (_turn_right_lost_line && _line_mr && !_last_line_mr) {
      _is_turning_right = _is_turning_180 ? (_turn_180_count >= 1 || micros() - _last_turn_timer_micros > 1500000 ? false : true) : false;
      _turn_180_count += 1;
      _turn_right_lost_line = false;
      _motor_left_speed = 0;
      _motor_right_speed = 0;
      _last_turn_direction = "R";
      return false;
    }

    if (!_turn_right_lost_line && !_line_mr) {
      _turn_right_lost_line = true;
    }

    _motor_left_speed = MOTOR_DRIVE_SPEED;
    _motor_right_speed = -MOTOR_DRIVE_SPEED;
  }

  return _is_turning_left || _is_turning_right;
}

bool Algorithm::handleOffsetMovement() {
  if ((!_is_moving_bit_forward && !_is_moving_bit_back) || _last_bit_movement_timer_micros == 0) {
    _last_bit_movement_timer_micros = micros();
  }

  if (micros() - _last_bit_movement_timer_micros > 180000 || (!_is_moving_bit_forward && !_is_moving_bit_back)) {
    _is_moving_bit_forward = false;
    _is_moving_bit_back = false;
    _motor_left_speed = 0;
    _motor_right_speed = 0;

    if (_state == STATE_CONT_LINE) {
      _has_reached_finish = true;
    }

    if (_state == STATE_NO_LINE) {
      _state = STATE_ROTATE_180;
    }

    //    if (_state == STATE_FOLLOWING_LINE) {}

  } else {
    _motor_left_speed = _is_moving_bit_forward ? MOTOR_DRIVE_SPEED : -MOTOR_DRIVE_SPEED;
    _motor_right_speed = _is_moving_bit_forward ? MOTOR_DRIVE_SPEED : -MOTOR_DRIVE_SPEED;
  }

  return _is_moving_bit_forward || _is_moving_bit_back;
}

bool Algorithm::updateState() {
  if (_line_ll && _line_rr) {
    _state = STATE_CONT_LINE;

  } else if (_line_ll) {
    _is_moving_bit_forward = true;
    _state = STATE_LEFT_TURN;
    return false;

  } else if (_line_rr) {
    _is_moving_bit_forward = true;
    _state = STATE_RIGHT_TURN;
    return false;

  } else if (!_line_found) {
    _state = STATE_NO_LINE;

  } else {
    _state = STATE_FOLLOWING_LINE;
  }

  return true;
}

void Algorithm::setLineData(bool hasLine, bool LL, bool LM, bool MM, bool MR, bool RR) {
  if (hasLine) {
    _initial_line_found = true;
  }

  _line_found = hasLine;
  _line_ll = LL;
  _line_lm = LM;
  _line_mm = MM;
  _line_mr = MR;
  _line_rr = RR;
}

void Algorithm::setDistanceCM(float cm) {
  if (cm <= 0) {
    return;
  }
  _measured_distance_cm = cm;
}

int Algorithm::getLeftMotorSpeed() {
  return _motor_left_speed;
}

int Algorithm::getRightMotorSpeed() {
  return _motor_right_speed;
}

bool Algorithm::stopMotors() {
  return _stop_motors;
}

String Algorithm::getStatusCode() {
  if (!_initial_line_found) {
    return "__";
  }

  if (_has_reached_finish) {
    return "FI";
  }

  return String(_num_crossings_passed) + _last_turn_direction;
}
