#include <Arduino.h>
#include "PanServo.h"

/** Set the next angle in oscillating mode **/
void PanServo::nextAngle(int neutral_angle, int range,
                         unsigned int nsteps, unsigned int rotation_speed,
                         unsigned int random_oscillation) {
  int next_angle, dangle;
  if (random_oscillation == 1) {
    // Random oscillation: pick a random angle
    next_angle = random(neutral_angle-range/2, neutral_angle+range/2);
  }
  else {
    // Otherwise do a sinusoidal oscillation
    dangle = int(float(range)/float(nsteps));
    if (_direction == 0) {
      next_angle = _current_angle + dangle;
      if (next_angle >= neutral_angle + range/2) {
        next_angle = neutral_angle + range/2;
        _direction = 1;
      }
    }
    else {
      next_angle = _current_angle - dangle;
      if (next_angle <= neutral_angle - range/2) {
        next_angle = neutral_angle - range/2;
        _direction = 0;
      }
    }
  }

  // Move to the next angle
  setAngle(next_angle, rotation_speed);
}
