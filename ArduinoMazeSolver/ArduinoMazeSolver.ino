#include "LineSensor.h"
#include "Ultrasonic.h"

#define DEBUG

#define pin_linesensor_ll   9
#define pin_linesensor_lm   10
#define pin_linesensor_mm   11
#define pin_linesensor_mr   12
#define pin_linesensor_rr   13
#define pin_ultrasonic_echo 2
#define pin_ultrasonic_trig 3

LineSensor lineSensor = LineSensor(pin_linesensor_ll, pin_linesensor_lm, pin_linesensor_mm, pin_linesensor_mr, pin_linesensor_rr);
Ultrasonic ultrasonic = Ultrasonic(pin_ultrasonic_echo, pin_ultrasonic_trig);

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif

  lineSensor.setup();
  lineSensor.invert(false);

  ultrasonic.setup();


  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

}

void loop() {
  lineSensor.update();
  ultrasonic.update();

#ifdef DEBUG
  Serial.print(lineSensor.hasLine());
  Serial.print("\t");
  Serial.print(lineSensor.toString());
  Serial.print("\t");
  Serial.print(lineSensor.toPrettyString());

  Serial.print("\t");
  Serial.print(ultrasonic.distance_cm());
  Serial.println("cm");
#endif
}
