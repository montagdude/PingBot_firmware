#include <Arduino.h>
#include "HopperServo.h"

/** Rotates barrier in hopper to release next row of balls **/
void HopperServo::rotate(unsigned int ball_count, unsigned int rotation_speed) {
  unsigned int i;
  for ( i = 0; i < 4; i++ ) {
    if (ball_count == _rotation_counts[i]) {
      setAngle(_rotation_angles[i], rotation_speed);
      break;
    }
  }
}
