// Get the MotorShield Library from github:
// https://github.com/hanseartic/MotorShield
#include <MotorShield.h>

#define uptime 1800000    // 5.5 hours in milliseconds
#define downtime 19800000 // 1/2 hour in milliseconds

MS_DCMotor motor(MOTOR_A);
int led = 13;
int closedSwitch = 2;
int openedSwitch = 4;
int motorRunning = BRAKE;
long long sketchtime = 0;
long long  sleeptime = uptime;
long long waketime = downtime;

void setup() {
  brakeMotor();
  pinMode(led, OUTPUT);
  pinMode(closedSwitch, INPUT);
  pinMode(openedSwitch, INPUT);
}

void loop() {
  sketchtime = millis();
  switch (motor.getState()) {
  case BRAKE:
    if (waketime <= sketchtime) {
      wakeUp();
    }
    if (sleeptime <= sketchtime) {
      gotoSleep();
    }
    break;
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
  default:
    if ((digitalRead(closedSwitch) == LOW) && 
      (digitalRead(openedSwitch) == LOW)) {
      // wind up the box initially
      windUp();
      wakeUp();
    }
    break;
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
  motor.run(RELEASE);
  motor.setSpeed(255);
  motor.run(FORWARD);
}

void windDown() {
  motor.run(RELEASE);
  motor.setSpeed(255);
  motor.run(BACKWARD);
}

void brakeMotor() {
  motor.setSpeed(0);
  motor.run(BRAKE);
}


