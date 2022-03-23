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
  if (_state == STATE_STOPPED) {
    Serial.println("STATE_STOPPED");
  }
  if (_state == STATE_FOLLOWING_LINE) {
    Serial.println("STATE_FOLLOWING_LINE");
  }
  if (_state == STATE_NO_LINE) {
    Serial.println("STATE_NO_LINE");
  }
  if (_state == STATE_CONT_LINE) {
    Serial.println("STATE_CONT_LINE");
  }
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

  updateState();
  switch (_state) {
    case STATE_STOPPED:
      _motor_left_speed = 0;
      _motor_right_speed = 0;
      break;

    case STATE_FOLLOWING_LINE:
      _motor_left_speed = _line_lm ? MOTOR_DRIVE_SPEED / 2 + MOTOR_DRIVE_SPEED / 3 : MOTOR_DRIVE_SPEED;
      _motor_right_speed = _line_mr ? MOTOR_DRIVE_SPEED / 2 + MOTOR_DRIVE_SPEED / 3 : MOTOR_DRIVE_SPEED;
      break;

    case STATE_NO_LINE:
      _state = STATE_LEFT_TURN;
      break;

    case STATE_CONT_LINE:
      _state = STATE_LEFT_TURN;
      break;

    case STATE_ROTATE_180:
      _state = STATE_LEFT_TURN;
      break;

    case STATE_POS_LINE:
      break;

    case STATE_RIGHT_TURN:
      if (!_line_mm) _turn_right_lost_line = true;
      if (_turn_right_lost_line && _line_mm) {
        _state = STATE_STOPPED;
        _turn_right_lost_line = false;
      }
      
      _motor_left_speed = MOTOR_DRIVE_SPEED;
      _motor_right_speed = -MOTOR_DRIVE_SPEED;
      break;

    case STATE_LEFT_TURN:
      if (!_line_mm) _turn_left_lost_line = true;
      if (_turn_left_lost_line && _line_mm) {
        _state = STATE_STOPPED;
        _turn_left_lost_line = false;
      }
      
      _motor_left_speed = -MOTOR_DRIVE_SPEED;
      _motor_right_speed = MOTOR_DRIVE_SPEED;
      break;

  }

  _last_line_found = _line_found;
  _last_line_ll = _line_ll;
  _last_line_lm = _line_lm;
  _last_line_mm = _line_mm;
  _last_line_mr = _line_mr;
  _last_line_rr = _line_rr;
}

bool Algorithm::updateState() {
  if (_state == STATE_LEFT_TURN || _state == STATE_RIGHT_TURN) {
    return false;
  }
  
  if (_line_ll && _line_rr) {
    _state = STATE_CONT_LINE;

  } else if (_line_ll) {
    _is_moving_bit_forward = true;
    _state = STATE_LEFT_TURN;
    delay(150);
    return false;

  } else if (_line_rr) {
    _is_moving_bit_forward = true;
    _state = STATE_RIGHT_TURN;
    delay(150);
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
