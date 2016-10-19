#include "Arduino.h"
#include "display.h"
#include "distance.h"
#include "servo.h"

double calibration[POINTS] = {0};
double measurements[POINTS] = {0};
double hits[POINTS] = {0};
double angles[POINTS] = {0};

bool calibrated = false;

// Run initial calibration sweep
void runCalibration() {
  resetServo();

  for (int i = 0; i < POINTS; i++) {
    display.clearDisplay();
    driveServoToPosition(i);
    delay(stepDelay);

    double angle = map(i, 0, POINTS-1, 0, 180);
    angles[i] = angle;

    double distance = getMedianDistance();
    calibration[i] = distance;

    drawCalibrationPoints(angles, calibration);
    display.display();
  }
}

// Used without calibration
bool isHitByTreshold(double distance) {
  return distance < 50;
}

// Compare calibration sweep with the measurement
bool isHitByCalibration(int index, double distance) {
  return (abs(calibration[index] - distance) > TRESHOLD);
}

// Arduino Setup function - run once after reset
void setup()   {
  setupDisplay();
  setupDistanceMeter();
  setupServo();

  pinMode(KILL_SWITCH_PIN, INPUT_PULLUP);
}

// Arduino Loop function - Main program loop
void loop() {
  if (digitalRead(KILL_SWITCH_PIN) == LOW) {

    // First time, run calibration sweep
    if (!calibrated) {
      runCalibration();
      calibrated = true;
    }

    // Sweep
    for (int i = 0; i < POINTS; i++) {

      // Turn servo
      driveServoToPosition(i);

      // Draw radar semicircles
      display.clearDisplay();
      drawRound();

      // Draw radar sweep line
      drawSweepLine(angles[i]);

      // Measure distance and draw
      double distance = getMedianDistance();
      hits[i] = isHitByCalibration(i, distance) ? distance : 0;
      measurements[i] = !isHitByCalibration(i, distance) ? distance : 0;
      drawHits(angles, hits);
      drawMeasurements(angles, measurements);
      drawCalibrationPoints(angles, calibration);

      // Update the buffer to the display
      display.display();

      // Shoot with the laser
      if (hits[i] != 0) {
        digitalWrite(LASER_PIN, HIGH);
        tone(BUZZER_PIN, 4000, 50);
        delay(100);
        tone(BUZZER_PIN, 4000, 50);
        delay(200);
        digitalWrite(LASER_PIN, LOW);
      }
    }

    // Servo back to starting position
    resetServo();
  }
}
