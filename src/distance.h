#include "Arduino.h"
#include "constants.h"

void setupDistanceMeter() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// returns distance in cm
double getDistance() {
  double duration, distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Pulse's duration in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);

  distance = duration / 58.3;

  if (distance > MAX_DISTANCE) distance = 100;
  return distance;
}

// Bubble sort (http://hwhacks.com/2016/05/03/bubble-sorting-with-an-arduinoc-application/)
void sort(int a[], int size) {
  for(int i=0; i<(size-1); i++) {
    for(int o=0; o<(size-(i+1)); o++) {
      if(a[o] > a[o+1]) {
        int t = a[o];
        a[o] = a[o+1];
        a[o+1] = t;
      }
    }
  }
}

double median(int array[], int size) {
  sort(array, size);
  int middle = size / 2;
  return array[middle];
}

double getMedianDistance() {
  const int count = 21;
  int values[count] = {0};
  for (int i=0; i<count; i++) {
    values[i] = (int)getDistance();
  }
  return median(values, count);
}
