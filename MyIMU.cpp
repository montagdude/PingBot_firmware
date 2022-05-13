#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
#include "MyIMU.h"

#define R2D 57.29577951308232

/** Constructor **/
MyIMU::MyIMU() {
  _count = 0;
}


/** Shift data in vectors **/
void MyIMU::shift(float & newpitch, float & newroll) {
  int i;

  for ( i = _count-1; i > 0; i-- ) {
    _pitches[i] = _pitches[i-1];
    _rolls[i] = _rolls[i-1];
  }
  _pitches[0] = newpitch;
  _rolls[0] = newroll;
}


/** Update moving averages **/
void MyIMU::update() {
  float x, y, z, atot, theta, phi;

  if (IMU.accelerationAvailable()) {
    if (_count < _navg)
      _count++;

    // Get static pitch and roll from g vector
    // https://mwrona.com/posts/accel-roll-pitch/
    IMU.readAcceleration(x, y, z);
    atot = sqrt(x*x + y*y + z*z);
    x /= atot;
    y /= atot;
    z /= atot;
    theta = asin(x);
    phi = atan2(y,z);

    // Append to vectors
    shift(theta, phi);
  }
}


/** Get pitch and roll values from moving average, in deg **/
float MyIMU::pitch() const {
  double theta;
  int i;

  theta = 0;
  for ( i = 0; i < _count; i++ ) {
    theta += _pitches[i];
  }

  return float(theta/double(_count)*R2D);
}
float MyIMU::roll() const {
  double phi;
  int i;

  phi = 0;
  for ( i = 0; i < _count; i++ ) {
    phi += _rolls[i];
  }

  return float(phi/double(_count)*R2D);
}
