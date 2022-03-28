#include <Arduino.h>
#include "FlapServo.h"

/** Attach to pin and set initial angle to closed angle**/
void FlapServo::initialize(int pin) {
  attach(pin);
  write(_closed_angle);
  _current_angle = _closed_angle;
}


/** Release the next ball down the chute **/
void FlapServo::releaseBall() {
  setAngle(_open_angle);
  delay(_open_delay);
  setAngle(_closed_angle);
}


/** Close flap **/
void FlapServo::closeFlap() {
  setAngle(_closed_angle);
}
