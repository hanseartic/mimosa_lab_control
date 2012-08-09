// Get the MotorShield Library from github:
// https://github.com/hanseartic/MotorShield
#include <MotorShield.h>

#define uptime   1800000  // 0.5 hours in milliseconds
#define downtime 19800000 // 5.5 hours in milliseconds

MS_DCMotor motor(MOTOR_A);
int led = 13;
int closedSwitch = 2;
int openedSwitch = 4;
long long sketchtime = 0;
long long sleeptime = uptime;
long long waketime = downtime;

int soundPin = 6;
int lightPin = 7;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(closedSwitch, INPUT);
  pinMode(openedSwitch, INPUT);
  pinMode(soundPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  
  motor.run(BRAKE);
  motor.setSpeed(255);
  Serial.begin(9600);
  wakeUp();
}

void loop() {
  sketchtime = millis();
  Serial.print(motor.getState());
  uint8_t motor_direction = motor.getDirection();
  if (BRAKE == motor.getState()) {
    if (waketime <= sketchtime) {
      wakeUp();
      windUp();
    }
    if (sleeptime <= sketchtime) {
      gotoSleep();
      windDown();
    }  
  } else {
    switch (motor_direction) {
    case FORWARD:
      if (digitalRead(openedSwitch) == HIGH) {
        brakeMotor("top");
        Serial.print("Now waiting ");
        Serial.print(int((waketime-millis()-downtime) / 1000));
        Serial.println(" seconds before lowering");
        takePicture();
      }
      break;
    case BACKWARD:
      if (digitalRead(closedSwitch) == HIGH) {
        brakeMotor("bottom");
        Serial.print("Now waiting ");
        Serial.print(int((sleeptime-millis()-uptime) / 1000));
        Serial.println(" seconds before lifting");
      }
      break;
    }
  }
}

void wakeUp() {
  sleeptime = millis() + uptime;
  waketime = sleeptime + downtime;
}

void gotoSleep() {
  waketime = millis() + downtime;
  sleeptime = waketime + uptime;
}

void windUp() {
  Serial.println("Lifting box");
  digitalWrite(lightPin, HIGH);
  digitalWrite(soundPin, HIGH);
  motor.run(RELEASE | FORWARD);
  boolean ledstate = false;
  while(digitalRead(openedSwitch) == LOW) {
    ledstate = !ledstate;
    digitalWrite(led, ledstate);
    delay(20);
  }
  digitalWrite(led, LOW);
}

void windDown() {
  Serial.println("Lowering box");
  takePicture();
  motor.run(RELEASE | BACKWARD);
   boolean ledstate = false;
  while(digitalRead(closedSwitch) == LOW) {
    ledstate = !ledstate;
    digitalWrite(led, ledstate);
    delay(20);
  }
  digitalWrite(led, LOW);
  digitalWrite(lightPin, LOW);
  digitalWrite(soundPin, LOW);
}

void brakeMotor(String position) {
  motor.run(BRAKE);
  Serial.println("Box reached " + position);
}

void takePicture() {
  digitalWrite(led, HIGH);
  Serial.println("shutter");
  boolean pictureOK = false;
  String serialRead = "";
  while(! pictureOK) {
    while (Serial.available()) {
      char inChar = (char)Serial.read();
      serialRead += inChar;
    }
    pictureOK = (serialRead.indexOf("OK") >= 0);
  }
  digitalWrite(led, LOW);
}
