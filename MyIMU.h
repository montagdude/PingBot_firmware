/** This class calculates static pitch and roll from the LSM9DS1 IMU and filters
    moving average. This is meant to be used in conjuction with the IMU object
    from the existing LSM9DS1 class, not to replace it. **/
#pragma once

class MyIMU {
  private:
    const static int _navg = 100;
    int _count;
    float _pitches[_navg];
    float _rolls[_navg];

    // Shift data in vectors
    void shift(float & newpitch, float & newroll);

  public:
    // Constructor
    MyIMU();

    // Update averages
    void update();

    // Get values
    float pitch() const;
    float roll() const;
};
