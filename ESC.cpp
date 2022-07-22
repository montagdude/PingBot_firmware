#include <Arduino.h>
#include "ESC.h"

/** Converts percent throttle to servo angle **/
int throttle_to_angle(int throttle) {
  return int(float(throttle)*180./100.);
}


/** Attach to pin and set pulsewidths for low and full throttle (microseconds) **/
void ESC::initialize(int pin, int pw_low, int pw_high) {
  attach(pin, pw_low, pw_high);
}


/** Arm by sending the low-throttle signal **/
void ESC::arm() {
  write(0);
  _current_angle = 0;
}


/** Calibrate ESC programmatically. Should be done in setup so that
    the high throttle signal is the first thing the ESC sees. After
    calibrating once, it shouldn't need to be done again. 
    e.g.:
    pinMode(esc_pin, OUTPUT);
    esc.initialize(esc_pin, low, high);
    esc.calibrate();
    **/
void ESC::calibrate() {
  // Send the high throttle signal for 5 seconds, then the low throttle
  // signal for 5 seconds.
  write(180);
  delay(5000);
  write(0);
  delay(5000);
}


/** Set a new throttle subject to optional throttle change speed in percent/sec **/
void ESC::setThrottle(int new_throttle_percent, unsigned int throttle_change_speed) {
  MyServo::setAngle(throttle_to_angle(new_throttle_percent),
                    throttle_to_angle(throttle_change_speed));
}


/** Reports throttle (percent) **/
int ESC::throttle() const {
  return int(float(_current_angle)*100./180.);
}
