#pragma once
#include <Arduino.h>
#include "MotorDriver.h"

/**
   Handle maze solving using sensor data
*/

#define FINISH_TARGET_MICROS    140000
#define TURN_OBJECT_DISTANCE_CM 6
#define MOTOR_DRIVE_SPEED       60

#define STATE_STOPPED         0
#define STATE_FOLLOWING_LINE  1
#define STATE_NO_LINE         2
#define STATE_CONT_LINE       3
#define STATE_ROTATE_180      4
#define STATE_POS_LINE        5
#define STATE_RIGHT_TURN      6
#define STATE_LEFT_TURN       7

class Algorithm {
  public:
    Algorithm();

    void setup();
    void update();

    bool handleTurning();
    bool handleOffsetMovement();
    
    bool updateState();
    void setLineData(bool hasLine, bool LL, bool LM, bool MM, bool MR, bool RR);
    void setDistanceCM(float cm);

    int getLeftMotorSpeed();
    int getRightMotorSpeed();
    bool stopMotors();

    String getStatusCode();
  private:
    int _state = STATE_STOPPED;
    
    long _last_turn_timer_micros;
    bool _is_turning_180;
    int _turn_180_count;
    bool _is_turning_left, _turn_left_lost_line;
    bool _is_turning_right, _turn_right_lost_line;

    bool _is_moving_bit_forward, _is_moving_bit_back;
    long _last_bit_movement_timer_micros;
    //
    bool _initial_line_found;

    bool _has_reached_finish;
    long _finish_timer_micros;

    float _measured_distance_cm = 1000;
    bool _line_found, _line_ll, _line_lm, _line_mm, _line_mr, _line_rr;
    bool _last_line_found, _last_line_ll, _last_line_lm, _last_line_mm, _last_line_mr, _last_line_rr;

    bool _stop_motors;
    int _motor_left_speed = 0;
    int _motor_right_speed = 0;

    int _num_crossings_passed = 0;
    String _last_turn_direction = "-";
};
