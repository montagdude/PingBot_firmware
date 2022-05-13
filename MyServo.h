#pragma once

#include <Servo.h>

class MyServo: public Servo {
  protected:
    int _current_angle;

    // Avoid name collision with min/max in Servo library
    int myMin(int val1, int val2) const;
    int myMax(int val1, int val2) const;

  public:
    // Constructor
    MyServo();

    // Attach to pin and set initial angle
    void initialize(int pin, int initial_angle);

    // Change servo angle subject to optional rotation speed in deg/sec
    void setAngle(int new_angle, unsigned int rotation_speed=0);

    // Report current angle
    int angle() const;
};
