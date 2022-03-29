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

String path = "S";
int totalIntersectionsCrossed = 0;
bool isAtIntersection = false;
String lastTurnDirection = "-";
bool hasFinished = false;
bool finishConcat = false;

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

  if (hasFinished) {
    bcd.write("FI");
    if (finishConcat == false) {
      finishConcat = true;
      path.replace("LU","T");
      path.replace("LTL","D");
      path.replace("SL","S");
    }
    Serial.println("F:::");
    Serial.println(path);
    countDownFrom10();
    bcd.write("RE");
    delay(2000);
    rerun(path);
    return;
  } else {
    bcd.write(String(totalIntersectionsCrossed) + lastTurnDirection);
  }

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

  if (LineSensors[0] == '1' || LineSensors[4] == '1') {
    if (!isAtIntersection) {
      totalIntersectionsCrossed++;
      isAtIntersection = true;
    }
  } else {
    isAtIntersection = false;
  }

  if (busy == false) {
    bcd.write("--");
  }
  busy = false;

  if (distance <= 10) {
    Serial.println("Val: " + (distance + log1 + log2 + log3 + log4) / 5);
    if ((distance + log1 + log2 + log3 + log4) / 5 >= 10) {
      busy = true;
      simpleStop();
      return;
    }
  }

  if (LineSensors == "00100") {
    motorDriver.setRightSpeed(65);
    motorDriver.setLeftSpeed(65);
    //    bcd.write("A1");/
    busy = true;
  } else if ( LineSensors == "01100" || LineSensors == "01000") {
    motorDriver.setRightSpeed(65);
    motorDriver.setLeftSpeed(0);
    //    bcd.write("C1");/
    busy = true;
  } else if ( LineSensors == "00110" || LineSensors == "00010") {
    motorDriver.setRightSpeed(0);
    motorDriver.setLeftSpeed(65);
    //    bcd.write("C2");/
    busy = true;
  } else if ( LineSensors == "11111") {
tsec:
    // Check forward.
    motorDriver.setLeftSpeed(65);
    motorDriver.setRightSpeed(65);
    delTime = millis();
    //    bcd.write("Ch");/
    while (true) {
      lineSensor.update();
      LineSensors = lineSensor.toString();
      if (LineSensors == "00100") {
        motorDriver.setLeftSpeed(0);
        motorDriver.setRightSpeed(0);
        //        bcd.write("tu/");
        delay(700);
        turnLeft();
        break;
      }
      if (LineSensors == "00000") {
        motorDriver.setLeftSpeed(0);
        motorDriver.setRightSpeed(0);
        //        bcd.write("tL/");
        delay(700);
        turnLeft();
        break;
      }
    }
  } else if (LineSensors == "00000") {
    //    bcd.write("E1"/);
    delay(100);
    motorDriver.setLeftSpeed(0);
    motorDriver.setRightSpeed(0);
    delay(700);
    turnLeft();
    path = path + "U";
  } else if (LineSensors == "00111") {
    //    bcd.write("A2");/
    motorDriver.setLeftSpeed(65);
    motorDriver.setRightSpeed(65);
    busy = true;
    delTime = millis();
    while (true) {
      lineSensor.update();
      LineSensors = lineSensor.toString();
      curTime = millis();
      if (LineSensors == "00100" && curTime - delTime >= 250) {
        motorDriver.setLeftSpeed(65);
        motorDriver.setRightSpeed(65);
        break;
      }
      if (LineSensors == "00000") {
        motorDriver.setLeftSpeed(0);
        motorDriver.setRightSpeed(0);
        //        bcd.write("t/R");
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
    //    bcd.write("E0/");
    busy = true;
  }
}

void turnLeft() {
  lastTurnDirection = "L";
  delTime = millis();
  //  bcd.write("T1");/
  motorDriver.setRightSpeed(65);
  motorDriver.setLeftSpeed(-65);
  delay(250);
  while (true) {
    motorDriver.setRightSpeed(65);
    motorDriver.setLeftSpeed(-65);
    lineSensor.update();
    LineSensors = lineSensor.toString();
    curTime = millis();
    if (curTime - delTime >= 2000) {
      bcd.write("FI");
      path = path + "F";
      hasFinished = true;
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(0);
      return;
    }
    if (LineSensors == "00100") {
      //      bcd.write("DO");/
      path = path + "L";
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(0);
      delay(700);
      break;
    }
  }
  return;
}

void turnRight() {
  lastTurnDirection = "R";
  delTime = millis();
  //  bcd.write("T2");/
  motorDriver.setRightSpeed(-70);
  motorDriver.setLeftSpeed(70);
  delay(250);
  while (true) {
    motorDriver.setRightSpeed(-70);
    motorDriver.setLeftSpeed(70);
    lineSensor.update();
    LineSensors = lineSensor.toString();
    curTime = millis();
    if (curTime - delTime >= 2000) {
      bcd.write("FI");
      path = path + "F";
      hasFinished = true;
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(0);
      return;
    }
    if (LineSensors == "00100") {
      //      bcd.write("DO");/
      path = path + "R";
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(0);
      delay(700);
      break;
    }
  }
  return;
}

void simpleStop() {
  //  bcd.write("E2");/
  motorDriver.setRightSpeed(0);
  motorDriver.setLeftSpeed(0);
  delay(750);
  turnLeft();
  return;
}

void countDownFrom10() {
  bcd.write("10");
  delay(1000);
  for (int i=9; i>0; i--) {
    bcd.write("0" + String(i));
    delay(1000);
    bcd.write("Fi");
    delay(500);
  }
  bcd.write("00");
  delay(500);
  bcd.write("Fi");
  delay(2000);
}

void rerun(String inPath) {
  Serial.println("Debug");
  int pathLength = inPath.length() + 1;
  char path_array[pathLength];
  inPath.toCharArray(path_array, pathLength);
  for (char turn : path_array) {
    if (turn == 'S') {
      Serial.println("Start found.");
      driveIdle();
    } else if (turn == 'L') {
      turnLeft();
    } else if (turn == 'R') {
      turnRight();
    } else if (turn == 'F') {
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(0);
      Serial.println("Finish!");
    } else if (turn == 'D') {
      driveIdle();
    }
  }
}

void driveIdle() {
  motorDriver.setLeftSpeed(70);
  motorDriver.setRightSpeed(70);
  delay(250);
  while (true) {
    lineSensor.update();
    LineSensors = lineSensor.toString();
    if ( LineSensors == "01100" || LineSensors == "01000") {
    motorDriver.setRightSpeed(70);
    motorDriver.setLeftSpeed(0);
    } else if ( LineSensors == "00110" || LineSensors == "00010") {
    motorDriver.setRightSpeed(0);
    motorDriver.setLeftSpeed(70);
    } else if ( LineSensors == "11111" || LineSensors == "11100" || LineSensors == "00111") {
      break;
    }
  }
  motorDriver.setRightSpeed(0);
  motorDriver.setLeftSpeed(0);
  return;
}
