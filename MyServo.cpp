#include <Arduino.h>
#include "MyServo.h"

/** Avoid name collisions with min and max in standard servo library **/
int MyServo::myMin(int val1, int val2) const {
  int mymin;
  if (val1 < val2)
    mymin = val1;
  else
    mymin = val2;
    
  return mymin;
}
int MyServo::myMax(int val1, int val2) const {
  int mymax;
  if (val1 > val2)
    mymax = val1;
  else
    mymax = val2;
    
  return mymax;
}

/** Constructor **/
MyServo::MyServo() {
  _current_angle = -180;
}


/** Attach to pin and set initial angle **/
void MyServo::initialize(int pin, int initial_angle) {
  attach(pin);
  write(initial_angle);
  _current_angle = initial_angle;
}


/** Change servo angle subject to optional rotation speed in deg/sec **/
void MyServo::setAngle(int new_angle, unsigned int rotation_speed) {
  if (rotation_speed == 0) {
    write(new_angle);
    _current_angle = new_angle;
  }
  else {
    unsigned long time_now, dt;
    int begin_angle = _current_angle;
    int delta_angle;
    unsigned long start_time = millis();
    
    while (_current_angle != new_angle) {
      time_now = millis();
      dt = time_now - start_time;
      delta_angle = int(float(rotation_speed*dt)/1000.);
      if (begin_angle < new_angle)
        _current_angle = myMin(begin_angle + delta_angle, new_angle);
      else
        _current_angle = myMax(begin_angle - delta_angle, new_angle);
      write(_current_angle);
    }
  }
}


/** Report current angle **/
int MyServo::angle() const {
  return _current_angle;
}
