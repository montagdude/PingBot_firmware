#pragma once

#include "MyServo.h"

class ESC: public MyServo {
  public:
    // Attach to pin and set pulsewidths for low and full throttle (microseconds)
    void initialize(int pin, int pw_low=1000, int pw_high=2000);

    // Arm by sending the low-throttle signal
    void arm();

    // Calibrate
    void calibrate();
    
    // Change throttle setting subject to optional throttle change speed in percent/sec
    void setThrottle(int new_throttle_percent, unsigned int throttle_change_speed=0);

    // Report throttle
    int throttle() const;
};
