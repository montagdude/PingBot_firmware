#include <Arduino.h>
#include "HopperServo.h"

/** Constructor **/
HopperServo::HopperServo(int min_angle, int max_angle,
                         unsigned int rotation_speed) {
  _min_angle = min_angle;
  _max_angle = max_angle;
  _rotation_speed = rotation_speed;

}


/** Rotates stirring arm in hopper. Input is dt in milliseconds. **/
void HopperServo::rotate(unsigned long dt) {
  int next_angle, dangle;

  dangle = int(float(_rotation_speed*dt)/1000.);
  if (_direction == 0) {
    next_angle = _current_angle + dangle;
    if (next_angle >= _max_angle) {
      next_angle = _max_angle;
      _direction = 1;
    }
  }
  else {
    next_angle = _current_angle - dangle;
    if (next_angle <= _min_angle) {
      next_angle = _min_angle;
      _direction = 0;
    }
  }
  setAngle(next_angle);
}
