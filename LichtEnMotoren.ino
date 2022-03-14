//Arduino PWM Speed Control：
#define ECHO 2
#define TRIGGER 3
#define E1 5
#define M1 4
#define E2 6
#define M2 7
// 9,10,11,12,13

void setup() {
   Serial.begin(115200); // Starting Serial Terminal
   pinMode(ECHO, INPUT);
   pinMode(TRIGGER, OUTPUT);
   pinMode(8,OUTPUT);
   pinMode(9,INPUT);
   pinMode(10,INPUT);
   pinMode(11,INPUT);
   pinMode(12,INPUT);
   pinMode(13,INPUT);
   pinMode(M1, OUTPUT);
   pinMode(M2, OUTPUT);
}

void loop() {
   digitalWrite(8,HIGH);
   long duration, cm;
   digitalWrite(TRIGGER, LOW);
   delayMicroseconds(2);
   digitalWrite(TRIGGER, HIGH);
   delayMicroseconds(10);
   digitalWrite(TRIGGER, LOW);

   duration = pulseIn(ECHO, HIGH);
   cm = microsecondsToCentimeters(duration);
   Serial.print(cm);
   Serial.print("cm\t");
//   Serial.println();
   for (int i = 9; i <= 13; i++) {
    Serial.print(!digitalRead(i) ? "*" : "-");
      if (!digitalRead(11)) {
        driveMotor(60,60);
      } else if (!digitalRead(12)) {
        driveMotor(0,60);
      } else if (!digitalRead(10)) {
        driveMotor(60,0); 
      } else {
        driveMotor(0,0);
      }
    }
   Serial.println();
//   delay(500);
}

void driveMotor(int Motor1, int Motor2) {
  digitalWrite(M1,HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, Motor1);   //PWM Speed Control
  analogWrite(E2, Motor2);
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
