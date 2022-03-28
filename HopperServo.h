#pragma once

#include "MyServo.h"

class HopperServo: public MyServo {
  protected:
    int _rotation_counts[4] = {10, 15, 20, 25};
    int _rotation_angles[4] = {113, 68, 23, 0};

  public:
    void rotate(unsigned int ball_count, unsigned int rotation_speed=0);
};
