// Get the MotorShield Library from github:
// https://github.com/hanseartic/MotorShield
#include <MotorShield.h>

#define uptime   19800000 // 5.5 hours in milliseconds
#define downtime 1800000  // 0.5 hours in milliseconds

MS_DCMotor motor(MOTOR_A);
int led = 13;
int closedSwitch = 2;
int openedSwitch = 4;
long long sketchtime = 0;
long long sleeptime = uptime;
long long waketime = downtime;

int soundPin = 6;
int lightPin = 7;

int motor_up = FORWARD;
int motor_down = BACKWARD;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(closedSwitch, INPUT);
  pinMode(openedSwitch, INPUT);
  pinMode(soundPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  
  motor.setSpeed(255);
  motor.run(RELEASE | motor_down);
  while (
    (digitalRead(closedSwitch) == HIGH) && 
    (digitalRead(openedSwitch) == HIGH)) {}
  motor.run(BRAKE);
  
  if (digitalRead(openedSwitch) == LOW) {
    motor_up = BACKWARD;
    motor_down = FORWARD;
    motor.run(RELEASE | motor_down);
    while (digitalRead(closedSwitch) == HIGH) {}
    motor.run(BRAKE);
  }
  
  Serial.begin(9600);
}

void loop() {
  sketchtime = millis();
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
    if (motor_direction == motor_up) {
      if (digitalRead(openedSwitch) == LOW) {
        brakeMotor("top");
#ifdef DEBUG        
        Serial.print("Now waiting ");
        Serial.print(int((waketime-millis()-downtime) / 1000));
        Serial.println(" seconds before lowering");
#endif        
        takePicture();
      }
    } else if (motor_direction == motor_down) {
      if (digitalRead(closedSwitch) == LOW) {
        brakeMotor("bottom");
#ifdef DEBUG        
        Serial.print("Now waiting ");
        Serial.print(int((sleeptime-millis()-uptime) / 1000));
        Serial.println(" seconds before lifting");
#endif
      }
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
#ifdef DEBUG
  Serial.println("Lifting box");
#endif
  digitalWrite(lightPin, HIGH);
  digitalWrite(soundPin, HIGH);
  motor.run(RELEASE | motor_up);
  boolean ledstate = false;
  while(digitalRead(openedSwitch) == HIGH) {
    ledstate = !ledstate;
    digitalWrite(led, ledstate);
    delay(20);
  }
  digitalWrite(led, LOW);
  motor.run(BRAKE);
}

void windDown() {
  takePicture();
#ifdef DEBUG
  Serial.println("Lowering box");
#endif
  motor.run(RELEASE | motor_down);
  boolean ledstate = false;
  while(digitalRead(closedSwitch) == HIGH) {
    ledstate = !ledstate;
    digitalWrite(led, ledstate);
    delay(20);
  }
  digitalWrite(led, LOW);
  motor.run(BRAKE);
  digitalWrite(lightPin, LOW);
  digitalWrite(soundPin, LOW);
}

void brakeMotor(String position) {
  motor.run(BRAKE);
#ifdef DEBUG
  Serial.println("Box reached " + position);
#endif
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

