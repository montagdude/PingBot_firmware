#include <Arduino.h>
#include "FlapServo.h"

// Constants. CX, SX are cos, sin of fixed chute angle (23 deg),
// ROLLDIST is distance we let ball roll, in mm, with flap open,
// G is acceleration due to gravity in mm/sec^2
#define DEG2RAD 0.017453292519943295
#define CX 0.9205048534524404
#define SX 0.39073112848927377
#define ROLLDIST 60.0
#define G 9810.0

/** Attach to pin and set initial angle to closed angle**/
void FlapServo::initialize(int pin) {
  attach(pin);
  write(_closed_angle);
  _current_angle = _closed_angle;
}


/** Calculate the sine of the chute incline angle.
 *  Inputs:
 *    Launcher pitch and pan angles, in degrees
 *  Output:
 *    Sine of the incine angle **/
double FlapServo::sineInclineAngle(int launcher_pitch, int launcher_pan) const {
  double theta, psi;

  // Launcher pitch angle limits - needs to be between ~ -20 and 80
  // (and convert to radians)
  theta = DEG2RAD*double(min(max(-20, launcher_pitch), 80));

  // Pan angle with zero-reference at pan servo angle 90 degrees
  psi = DEG2RAD*double(launcher_pan-90);

  // Sine of chute incline angle; equal to absolute value of z-component
  // of a unit vector along the chute axis
  return abs(-CX*cos(psi)*sin(theta) - SX*cos(theta));
}


/** Set open delay based on launcher angles **/
void FlapServo::setOpenDelay(int launcher_pitch, int launcher_pan) {
  double sinphi;
  
  // Sin of chute incline angle
  sinphi = sineInclineAngle(launcher_pitch, launcher_pan);

  // Calculate time flap needs to be open, in milliseconds
  _open_delay = int(1000.*sqrt(5.*ROLLDIST/(3.*G*sinphi)));
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
