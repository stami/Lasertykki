#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "constants.h"

// Display setup
Adafruit_SSD1306 display(4);

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
  display.clearDisplay();
  display.display();
}

// Draw semicircles for radar
void drawRound() {
  display.drawCircle(RADIUS, RADIUS, RADIUS, WHITE);
  display.drawCircle(RADIUS, RADIUS, 10, WHITE);
}

// Draw sweep line at given angle (from left, 0...180)
void drawSweepLine(double angle) {
  // Angle to radians
  double radians = (double)angle * 3.14159 / 180;

  // X and Y components
  int xPos = RADIUS - cos(radians) * RADIUS;
  int yPos = RADIUS - sin(radians) * RADIUS;

  // Draw the Line
  display.drawLine(RADIUS, RADIUS, xPos, yPos, WHITE);
}

void drawPoint(int angle, double distance, bool fill = false, int radius = 2) {
  // Angle to radians
  double radians = (double)angle * 3.14159 / 180;

  if (distance > 100) distance = 100;

  // X and Y coordinates
  int xPos = RADIUS - cos(radians) * map(distance, 0, 100, 10, RADIUS);
  int yPos = RADIUS - sin(radians) * map(distance, 0, 100, 10, RADIUS);

  if (fill) {
    display.fillCircle(xPos, yPos, radius, WHITE);
  } else {
    display.drawCircle(xPos, yPos, radius, WHITE);
  }
}

void drawHits(double angles[], double hits[]) {
  for (int i = 0; i < POINTS; i++) {
    if (hits[i] != 0) {
      drawPoint(angles[i], hits[i], true);
    }
  }
}

void drawMeasurements(double angles[], double measurements[]) {
  for (int i = 0; i < POINTS; i++) {
    if (measurements[i] != 0) {
      drawPoint(angles[i], measurements[i]);
    }
  }
}

void drawCalibrationPoints(double angles[], double calibration[]) {
  for (int i = 0; i < POINTS; i++) {
    drawPoint(angles[i], calibration[i], false, 0);
  }
}

// Print value to top left corner of display (for debugging)
void printDistance(double distance) {
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print(distance, 1);
  display.print("cm");
}
