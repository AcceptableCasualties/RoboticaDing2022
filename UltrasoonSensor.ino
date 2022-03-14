//Arduino PWM Speed Control：
#define ECHO 2
#define TRIGGER 3

void setup() {
   Serial.begin(9600); // Starting Serial Terminal
   pinMode(ECHO, INPUT);
   pinMode(TRIGGER, OUTPUT);
}

void loop() {
   long duration, cm;
   digitalWrite(TRIGGER, LOW);
   delayMicroseconds(2);
   digitalWrite(TRIGGER, HIGH);
   delayMicroseconds(10);
   digitalWrite(TRIGGER, LOW);

   duration = pulseIn(ECHO, HIGH);
   cm = microsecondsToCentimeters(duration);
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
   delay(100);
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
