#include "LineSensor.h"
#include "Ultrasonic.h"
#include "MotorDriver.h"
#include "BCD.h"
#include "Algorithm.h"

#define DEBUG

#define pin_linesensor_ll   9
#define pin_linesensor_lm   10
#define pin_linesensor_mm   11
#define pin_linesensor_mr   12
#define pin_linesensor_rr   13
#define pin_ultrasonic_echo 2
#define pin_ultrasonic_trig 3
#define pin_motor_e1        5
#define pin_motor_m1        4
#define pin_motor_e2        6
#define pin_motor_m2        7
#define pin_bcd_data        A0
#define pin_bcd_latch       A1
#define pin_bcd_clock       A2

LineSensor lineSensor = LineSensor(pin_linesensor_ll, pin_linesensor_lm, pin_linesensor_mm, pin_linesensor_mr, pin_linesensor_rr);
Ultrasonic ultrasonic = Ultrasonic(pin_ultrasonic_echo, pin_ultrasonic_trig);
MotorDriverPWM motorDriver = MotorDriverPWM(pin_motor_e1, pin_motor_m1, pin_motor_e2, pin_motor_m2);
//MotorDriverPLL motorDriver = MotorDriverPLL(pin_motor_m1, pin_motor_e1, pin_motor_m2, pin_motor_e2);
BCD bcd = BCD(pin_bcd_data, pin_bcd_latch, pin_bcd_clock);

Algorithm algo = Algorithm();
String LastDifSensor;
String LineSensors;
bool busy;
bool turnFound;
unsigned long curTime;
unsigned long delTime;
int dist;
int distance = 20;
int distance2 = 20;
int log1 = 20;
int log2 = 20;
int log3 = 20;
int log4 = 20;

void setup() {
  //#ifdef DEBUG
  Serial.begin(115200);
  //#endif

  lineSensor.setup();
  lineSensor.invert(false);

  ultrasonic.setup();

  motorDriver.setup();
  bcd.setup();

  algo.setup();

//  bcd.write("--");
  //  motorDriver.flipLeftMotorDirection();
  //  motorDriver.flipRightMotorDirection();

  delay(1000);

}

void loop() {
  start:
  lineSensor.update();
  ultrasonic.update();

  distance2 = ultrasonic.distance_cm();
  if (distance2 >= 1770) {
    Serial.println("Val err");
  } else {
    log1 = distance;
    log2 = log1;
    log3 = log2;
    log4 = log3;
    distance = ultrasonic.distance_cm();
    Serial.println(distance);
    Serial.println(log1);
    Serial.println(log2);
    Serial.println(log3);
    Serial.println(log4);
  }
  
  
  LineSensors = lineSensor.toString();
  //Serial.println(LineSensors);

  if (busy == false) {bcd.write("--");}
  busy = false;

  if (distance<=10) {
    Serial.println("Val: " + (distance+log1+log2+log3+log4)/5);
    if ((distance+log1+log2+log3+log4)/5 >= 10) {
    busy = true;
    simpleStop();
    return;
  }}

  if (LineSensors == "00100") {
    motorDriver.setRightSpeed(70);
    motorDriver.setLeftSpeed(70);
    bcd.write("A1");
    busy = true;
  } else if ( LineSensors == "01100" || LineSensors == "01000") {
    motorDriver.setRightSpeed(70);
    motorDriver.setLeftSpeed(0);
    bcd.write("C1");
    busy = true;
  } else if ( LineSensors == "00110" || LineSensors == "00010") {
    motorDriver.setRightSpeed(0);
    motorDriver.setLeftSpeed(70);
    bcd.write("C2");
    busy = true;
  } else if ( LineSensors == "11111") {
      tsec:
      // Check forward.
        motorDriver.setLeftSpeed(70);
        motorDriver.setRightSpeed(70);
        delTime = millis();
        bcd.write("Ch");
        while (true) {
          lineSensor.update();
          LineSensors = lineSensor.toString();
          if (LineSensors == "00100") {
            motorDriver.setLeftSpeed(0);
            motorDriver.setRightSpeed(0);
            bcd.write("tu");
            delay(700);
            turnLeft();
            break;
          }
          if (LineSensors == "00000") {
              motorDriver.setLeftSpeed(0);
              motorDriver.setRightSpeed(0);
              bcd.write("tL");
              delay(700);
              turnLeft();
              break; 
             }
          }
  } else if (LineSensors == "00000") {
    bcd.write("E1");
    delay(100);
    motorDriver.setLeftSpeed(0);
    motorDriver.setRightSpeed(0);
    delay(700);
    turnLeft();
  } else if (LineSensors == "00111") {
    bcd.write("A2");
    motorDriver.setLeftSpeed(70);
    motorDriver.setRightSpeed(70);
    busy = true;
    delTime = millis();
    while (true) {
      lineSensor.update();
      LineSensors = lineSensor.toString();
      curTime = millis();
      if (LineSensors == "00100" && curTime-delTime >= 250) {
        motorDriver.setLeftSpeed(70);
        motorDriver.setRightSpeed(70);
        break;
      }
      if (LineSensors == "00000") {
        motorDriver.setLeftSpeed(0);
        motorDriver.setRightSpeed(0);
        bcd.write("tR");
        delay(700);
        turnRight();
        break;
      }
      if (LineSensors == "11111") {
        goto tsec;
      }
    }
  motorDriver.setLeftSpeed(0);
  motorDriver.setRightSpeed(0);
  return;
  } else {
    bcd.write("E0");
    busy = true;
  }
}

void turnLeft() {
  delTime = millis();
  bcd.write("T1");
  motorDriver.setRightSpeed(70);
  motorDriver.setLeftSpeed(-70);
  delay(250);  
  while (true) {
    motorDriver.setRightSpeed(70);
    motorDriver.setLeftSpeed(-70);
    lineSensor.update();
    LineSensors = lineSensor.toString();
    curTime = millis();
    if (curTime-delTime >= 2000) {
      bcd.write("FI");
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(0);
      exit(0);
    }
    if (LineSensors=="00100") {
      bcd.write("DO");
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(0);
      delay(700);
      break;
    }
 }
 return;
}

void turnRight() {
  delTime = millis();
  bcd.write("T2");
  motorDriver.setRightSpeed(-70);
  motorDriver.setLeftSpeed(70);
  delay(250);  
  while (true) {
    motorDriver.setRightSpeed(-70);
    motorDriver.setLeftSpeed(70);
    lineSensor.update();
    LineSensors = lineSensor.toString();
    curTime = millis();
    if (curTime-delTime >= 2000) {
      bcd.write("FI");
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(0);
      return;
    }
    if (LineSensors=="00100") {
      bcd.write("DO");
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(0);
      delay(700);
      break;
    }
 }
 return;
}

void simpleStop() {
  bcd.write("E2");
  motorDriver.setRightSpeed(0);
  motorDriver.setLeftSpeed(0);
  delay(750);
  turnLeft();
  return;
}
