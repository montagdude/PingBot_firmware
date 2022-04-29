#pragma once

#include "MyServo.h"

class FlapServo: public MyServo {
  protected:
    int _open_angle = 90;
    int _closed_angle = 120;
    int _open_delay = 150;

  public:
    void initialize(int pin);
    double sineInclineAngle(int launcher_pitch, int launcher_pan) const;
    void setOpenDelay(int launcher_pitch, int launcher_pan);
    void releaseBall();
    void closeFlap();
};
