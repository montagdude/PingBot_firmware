#include <Arduino.h>
#include "HopperServo.h"

/** Constructor **/
HopperServo::HopperServo(int min_angle, int max_angle,
                         unsigned int rotation_speed) {
  _min_angle = min_angle;
  _max_angle = max_angle;
  _rotation_speed = rotation_speed;

}


/** Rotates stirring arm in hopper. Input is current time in milliseconds. **/
void HopperServo::rotate(unsigned long time_now) {
  int next_angle, dt, dangle;

  // Calculate angular change based on rotation speed, subject to a maximum
  // limit
  dt = time_now - _time_last;
  dangle = myMin(int(float(_rotation_speed*dt)/1000.), _max_dangle);

  // Use min_angle to make sure we don't try to adjust the servo through a
  // vanishingly small time/angle
  if (dangle > _min_dangle) {
    // Increase the angle
    if (_direction == 0) {
      next_angle = _current_angle + dangle;
      if (next_angle >= _max_angle) {
        next_angle = _max_angle;
        _direction = 1;
      }
    }
    else {
      // Decrease the angle
      next_angle = _current_angle - dangle;
      if (next_angle <= _min_angle) {
        next_angle = _min_angle;
        _direction = 0;
      }
    }
    setAngle(next_angle);
    _time_last = time_now;
  }
}
