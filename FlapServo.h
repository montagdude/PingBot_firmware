#pragma once

#include "MyServo.h"

class FlapServo: public MyServo {
  protected:
    int _open_angle = 90;
    int _closed_angle = 130;
    int _open_delay = 150;

  public:
    void initialize(int pin);
    void releaseBall();
    void closeFlap();
};
