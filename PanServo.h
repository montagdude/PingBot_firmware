#pragma once

#include "MyServo.h"

class PanServo: public MyServo {
  protected:
    unsigned int _direction = 0;

  public:
    void nextAngle(int neutral_angle, int range, unsigned int rotation_speed,
                   unsigned int nsteps, unsigned int random_oscillation);
};
