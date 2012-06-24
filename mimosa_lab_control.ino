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

void setup() {
  pinMode(led, OUTPUT);
  pinMode(closedSwitch, INPUT);
  pinMode(openedSwitch, INPUT);
  motor.run(RELEASE);
  motor.setSpeed(255);
  Serial.begin(9600);
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
    switch (motor.getDirection()) {
    case FORWARD:
      if (digitalRead(openedSwitch) == HIGH) {
        brakeMotor();
      }
      break;
    case BACKWARD:
      if (digitalRead(closedSwitch) == HIGH) {
        brakeMotor();
      }
      break;
    }
    if ((digitalRead(closedSwitch) == LOW) && 
        (digitalRead(openedSwitch) == LOW)) {
        // wind up the box initially
        windUp();
        wakeUp();
      }
      
  }
}

void wakeUp() {
  sleeptime = millis() + uptime;
  waketime = sleeptime + downtime;
  // TODO:
  // set off all the other switches
  digitalWrite(led, HIGH);
}

void gotoSleep() {
  waketime = millis() + downtime;
  sleeptime = waketime + uptime;
  // TODO:
  // set off all the other switches
  digitalWrite(led, LOW);
}

void windUp() {
  motor.run(RELEASE | FORWARD);
}

void windDown() {
  motor.run(RELEASE | BACKWARD);
}

void brakeMotor() {
  motor.run(BRAKE);
}

