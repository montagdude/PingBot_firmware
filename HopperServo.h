#pragma once

#include "MyServo.h"

class HopperServo: public MyServo {
  protected:
    int _min_angle = 0;
    int _max_angle = 180;
    int _min_dangle = 1;
    int _max_dangle = 5;
    unsigned int _rotation_speed = 45;
    unsigned int _direction = 0;
    unsigned long _time_last = 0;

  public:
    HopperServo(int min_angle, int max_angle, unsigned int rotation_speed);
    void rotate(unsigned long dt);
};
