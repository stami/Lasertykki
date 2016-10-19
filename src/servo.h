#include <Servo.h>
#include "constants.h"

// Servo parameters
int stepDelay = 100;
int resetDelay = 1000;

Servo myservo;
int pos = 0; // variable to store the servo position

void setupServo() {
  myservo.attach(SERVO_PIN);
  myservo.write(MIN_ANGLE);
  digitalWrite(LASER_PIN, HIGH);
  delay(resetDelay);
  digitalWrite(LASER_PIN, LOW);
}

// Position: int 0...19
void driveServoToPosition(int position) {

  // 0...19 -> MAX_ANGLE...MIN_ANGLE (käänteinen kulma)
  int angle = map(position, 0, POINTS-1, MAX_ANGLE, MIN_ANGLE);
  myservo.write(angle);

  delay(stepDelay);
}

void resetServo() {
  driveServoToPosition(0);
  delay(resetDelay);
}
