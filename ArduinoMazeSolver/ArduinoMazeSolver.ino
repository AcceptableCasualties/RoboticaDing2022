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
String LineSensors;
bool busy;
bool probed;

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

  bcd.write("--");
  //  motorDriver.flipLeftMotorDirection();
  //  motorDriver.flipRightMotorDirection();

  delay(1000);

}

void loop() {
  lineSensor.update();
  ultrasonic.update();

  LineSensors = lineSensor.toString();
  Serial.println(LineSensors);

  if (busy == false) {bcd.write("--");}
  busy = false; 
  
  // LineSensors[1] == '1';
  if (LineSensors == "00100") {
    motorDriver.setRightSpeed(70);
    motorDriver.setLeftSpeed(70);
    bcd.write("A1");
    busy = true;
  } else if (LineSensors == "01100" || LineSensors == "01000") {
    bcd.write("C1");
    while (true) {
      lineSensor.update();
      LineSensors = lineSensor.toString();
      motorDriver.setRightSpeed(70);
      motorDriver.setLeftSpeed(0);
      if (!(LineSensors == "01100" || LineSensors == "01000")) {
        motorDriver.setRightSpeed(0);
        motorDriver.setLeftSpeed(0);
        break;
      }
    }
  } else if (LineSensors == "00110" || LineSensors == "00010") {
    bcd.write("C2");
    while (true) {
      lineSensor.update();
      LineSensors = lineSensor.toString();
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(70);
      if (!(LineSensors == "00110" || LineSensors == "00010")) {
        motorDriver.setRightSpeed(0);
        motorDriver.setLeftSpeed(0);
        break;
      }
    }
  } else if (LineSensors == "00111") {
     bcd.write("T2");
    while (true) {
      lineSensor.update();
      LineSensors = lineSensor.toString();
      motorDriver.setRightSpeed(0);
      motorDriver.setLeftSpeed(70);
      if (LineSensors == "00100") {
        motorDriver.setRightSpeed(0);
        motorDriver.setLeftSpeed(0);
        break;
      }
    }
  } else if ( LineSensors == "11100") {
    bcd.write("T1");
    while (true) {
      lineSensor.update();
      LineSensors = lineSensor.toString();
      motorDriver.setRightSpeed(70);
      motorDriver.setLeftSpeed(0);
      if (LineSensors == "00100") {
        motorDriver.setRightSpeed(0);
        motorDriver.setLeftSpeed(0);
        break;
      }
    }
  } else if ( LineSensors == "11111") {
    bcd.write("E1");
    while (true) {
      lineSensor.update();
      LineSensors = lineSensor.toString();
      motorDriver.setRightSpeed(70);
      motorDriver.setLeftSpeed(0);
      if (LineSensors == "00100") {
        motorDriver.setRightSpeed(0);
        motorDriver.setLeftSpeed(0);
        break;
      }
    }
  } else if ( LineSensors == "00000") {
    bcd.write("RF");
    while (true) {
      lineSensor.update();
      LineSensors = lineSensor.toString();
      motorDriver.setRightSpeed(70);
      motorDriver.setLeftSpeed(-70);
      if (LineSensors == "00100") {
        motorDriver.setRightSpeed(0);
        motorDriver.setLeftSpeed(0);
        break;
      }
    }
  } else {
    motorDriver.setRightSpeed(0);
    motorDriver.setLeftSpeed(0);
    bcd.write("E0");
    busy = true;
  }


  //#ifdef DEBUG
  //  Serial.print(lineSensor.hasLine());
  //  Serial.print("\t");
  //  Serial.print(lineSensor.toString());
  //  Serial.print("\t");
  //  Serial.print(lineSensor.toPrettyString());
  //
  //  Serial.print("\t");
  //  Serial.print(ultrasonic.distance_cm());
  //  Serial.print("cm");
  //
  //  Serial.println();
  //#endif
}
