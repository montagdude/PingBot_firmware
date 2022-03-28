#pragma once

#include <Servo.h>

class MyServo: public Servo {
  protected:
    int _current_angle;

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
