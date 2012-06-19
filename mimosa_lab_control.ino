// Get the MotorShield Library from github:
// https://github.com/hanseartic/MotorShield
#include <MotorShield.h>

#define uptime 14400000  // four hours in milliseconds
#define downtime 1800000 // half an hour in milliseconds

MS_DCMotor motor(MOTOR_A);
//int led = 13;
int closedSwitch = 2;
int openedSwitch = 4;

void setup() {
  motor.setSpeed(0);
  motor.run(BRAKE);
  pinMode(closedSwitch, INPUT);
  pinMode(openedSwitch, INPUT);
}

void loop() {
  // wind up the box
  // wait for half an hour
  // wind down the box
  // wait for 4.5 hours
}

// run the motor until the end-switch is hit
void windUp() {
  motor.setSpeed(255);
  motor.run(FORWARD);
  while (digitalRead(openedSwitch) != 1) {
    motor.run(RELEASE);
  }
  motor.setSpeed(0);
  motor.run(BRAKE);
}

// run the motor until the end-switch is hit
void windDown() {
    motor.setSpeed(255);
  motor.run(BACKWARD);
  while (digitalRead(closedSwitch) != 1) {
    motor.run(RELEASE);
  }
  motor.setSpeed(0);
  motor.run(BRAKE);
}

