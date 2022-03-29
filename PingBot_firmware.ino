#include "PanServo.h"
#include "FlapServo.h"
#include "HopperServo.h"
#include "ESC.h"

// Firmware version
const char * fver = "0.1";

// Top motor ESC parameters
ESC top_ESC;
unsigned int top_ESC_pin = 9;
int top_ESC_pw_low = 1000;
int top_ESC_pw_full = 2000;
unsigned int top_throttle_pin = A0;

// Bottom motor ESC parameters
ESC bot_ESC;
unsigned int bot_ESC_pin = 6;
int bot_ESC_pw_low = 1000;
int bot_ESC_pw_full = 2000;
unsigned int bot_throttle_pin = A1;

// Common ESC parameters
unsigned int throttle_change_speed = 100;
unsigned int startup_throttle_change_speed = 25;

// Flap servo parameters
FlapServo flap_servo;
unsigned int flap_pin = 11;

// Pan servo parameters
PanServo pan_servo;
unsigned int pan_pin = 10;
int pan_neutral_angle = 93;
unsigned int pan_max_rotation = 60;
unsigned int pan_rotation_speed = 45;
unsigned int oscillation_steps = 8;
unsigned int oscillation_angle_pin = A2;

// Hopper servo parameters
int hopper_pin = 5;
int hopper_neutral_angle = 90;
int hopper_min_angle = 0;
int hopper_max_angle = 140;
unsigned int hopper_rotation_speed = 45;
HopperServo hopper_servo(hopper_min_angle, hopper_max_angle,
                         hopper_rotation_speed);

// Cycle parameters
unsigned int period_max = 6000;
unsigned int period_min = 1500;
unsigned int period;
unsigned long time_cycle, time_last;
unsigned long time_now = 0;
unsigned int ball_count;
unsigned int cycle_period_pin = A3;

// Switches
unsigned int start_stop_pin = 8;
unsigned int start_stop = 0;
unsigned int prev_start_stop = 0;
unsigned int oscillation_mode_pin = 7;
unsigned int oscillation_mode = 0;
unsigned int random_oscillation_pin = 4;
unsigned int random_oscillation = 0;

// Other parameters
unsigned int startup_delay = 5000;


/** Setup **/
void setup() {
  // Flap servo setup
  pinMode(flap_pin, OUTPUT);
  flap_servo.initialize(flap_pin);

  // Pan servo setup
  pinMode(pan_pin, OUTPUT);
  pan_servo.initialize(pan_pin, pan_neutral_angle);

  // Hopper servo setup
  pinMode(hopper_pin, OUTPUT);
  hopper_servo.initialize(hopper_pin, hopper_neutral_angle);

  // Top ESC setup
  pinMode(top_ESC_pin, OUTPUT);
  top_ESC.initialize(top_ESC_pin, top_ESC_pw_low, top_ESC_pw_full);
  top_ESC.arm();

  // Bottom ESC setup
  pinMode(bot_ESC_pin, OUTPUT);
  bot_ESC.initialize(bot_ESC_pin, bot_ESC_pw_low, bot_ESC_pw_full);
  bot_ESC.arm();

  // Switches setup
  pinMode(start_stop_pin, INPUT);
  pinMode(oscillation_mode_pin, INPUT);
  pinMode(random_oscillation_pin, INPUT);

  // Other setup
  Serial.begin(9600);
    
  // Since A5 is disconnected, analog noise will cause randomSeed to generate
  // different seed numbers each time it runs.
  randomSeed(analogRead(A5)); // Random seed 
}


/** Start cycle procedure **/
void start_cycle(int top_throttle, int bot_throttle,
                 unsigned int startup_delay) {
  // Print firmware version to serial output. Do it here rather than in setup(),
  // because it might be hard to catch it in time there unless a significant
  // delay is added.
  char buff[40];
  sprintf(buff, "PingBot firmware version %s", fver);
  Serial.println(buff);
  
  // Slowly increase throttle to set value
  top_ESC.setThrottle(top_throttle, startup_throttle_change_speed);
  bot_ESC.setThrottle(bot_throttle, startup_throttle_change_speed);
  
  // Set servos to proper initial angle
  flap_servo.closeFlap();
  pan_servo.setAngle(pan_neutral_angle, pan_rotation_speed);
  hopper_servo.setAngle(hopper_neutral_angle);

  // Give some time for the player to get ready
  period = startup_delay;
  
  // Set some global variables
  prev_start_stop = 1;
  ball_count = 0;
  time_cycle = millis();
  time_now = time_cycle;
  time_last = time_cycle;
}


/** End cycle procedure **/
void end_cycle() {
  // Slowly decrease throttle to 0
  top_ESC.setThrottle(0, startup_throttle_change_speed);
  bot_ESC.setThrottle(0, startup_throttle_change_speed);

  // Set servos to proper initial angle
  flap_servo.closeFlap();
  pan_servo.setAngle(pan_neutral_angle, pan_rotation_speed);
  hopper_servo.setAngle(hopper_neutral_angle);

  // Set some global variables
  prev_start_stop = 0;

  // Print message to serial output
  Serial.println("Ended cycle.");
}

  
/** Main loop **/
void loop() {
  unsigned int input_period;
  int top_throttle, bot_throttle, pan_angle, pan_range;
  char buff[40];

  // Read switches
  start_stop = digitalRead(start_stop_pin);
  oscillation_mode = digitalRead(oscillation_mode_pin);
  random_oscillation = digitalRead(random_oscillation_pin);

  // Read throttles. Map from 10-1023 -> 0-100 gives a little deadband at the
  // low end to correct small voltage deviations that may keep it from going to
  // 0.
  top_throttle = map(analogRead(top_throttle_pin), 10, 1023, 0, 100);
  bot_throttle = map(analogRead(bot_throttle_pin), 10, 1023, 0, 100);

  // Set oscillation angle or range
  if (oscillation_mode == 0) {
    pan_angle = map(analogRead(oscillation_angle_pin), 0, 1023,
                               pan_neutral_angle+pan_max_rotation/2,
                               pan_neutral_angle-pan_max_rotation/2);
    pan_servo.setAngle(pan_angle, pan_rotation_speed);
  }
  else {
    pan_range = map(analogRead(oscillation_angle_pin), 0, 1023, 0,
                    pan_max_rotation);
  }

  // Read cycle period from analog input, but don't set it ('period' variable)
  // until the end of the next cycle. This allows programmatic changes to it in
  // the meantime (e.g. by start_cycle).
  input_period = map(analogRead(cycle_period_pin), 0, 1023, period_max,
                     period_min);

  // Print debugging info
  //sprintf(buff, "%d, %d, %d, %d, %d, %d, %d", top_throttle, bot_throttle,
  //        pan_servo.angle(), input_period, start_stop, oscillation_mode,
  //        random_oscillation);
  //Serial.println(buff);

  // Procedures at start and end of cycle
  if ( (start_stop == 1) && (prev_start_stop == 0) )
    start_cycle(top_throttle, bot_throttle, startup_delay);
  else if ( (start_stop == 0) && (prev_start_stop == 1) )
    end_cycle();

  // If switch is set to stop, don't go any farther
  if (start_stop == 0)
    return;

  // Set throttle
  top_ESC.setThrottle(top_throttle, throttle_change_speed);
  bot_ESC.setThrottle(bot_throttle, throttle_change_speed);
  
  // Keep track of cycle timing in a non-blocking way using millis()
  time_last = time_now;
  time_now = millis();

  if (start_stop == 1) {
    // Use servo to stir balls in hopper
    hopper_servo.rotate(time_now-time_last);

    // Oscillate if requested and release a ball
    if (time_now >= time_cycle + period) {
      time_cycle = time_now;
      if (oscillation_mode == 1) {
        pan_servo.nextAngle(pan_neutral_angle, pan_range, oscillation_steps,
                            pan_rotation_speed, random_oscillation);
      }
      flap_servo.releaseBall();
      ball_count++;

      // Set period from analog input
      period = input_period;

      // Print status to serial output
      sprintf(buff, "Ball count: %d", ball_count);
      Serial.println(buff);
    }
  }
}
