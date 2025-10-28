
// #include <Arduino.h>
// #include <Servo.h>
// #include "Enes100.h"
#include <propulsion.h>
#include <cstdlib>

/* Objects */
Servo claw_servo;
HX711 scale;

// State values
double heading;

void setup() {
  // Attach debug serial port
  Serial.begin(9600);
  // Initialize objects
  claw_servo.attach(servo_sg_pin);
  scale.begin(loadcell_dout_pin, loadcell_sck_pin);

  // Initialize state
  heading = 0.;
}

void loop() {
  // put your main code here, to run repeatedly:

  // Find our way past the obstacles to the endzone
  navigate_to_endzone();
}

void navigate_to_endzone() {
  bool checked_left = false;
  bool in_endzone = false;
  // Loop through these instructions until we get there
  while (!in_endzone) {
    if (close_enough(heading, 0, 5)) { // If we're pointing the right way...
      // Either avoid obstacles or move forward
      if (sensor_FL() <= in_front_tolerance || sensor_FR() <= in_front_tolerance) {
        // Move left. If we get to the wall without clearing in front, move right
        if (!checked_left) {
          if (sensor_L() <= in_front_tolerance) checked_left = true;
           if (!checked_left) move_left(in_front_tolerance);
        } else {
          if (sensor_R() <= in_front_tolerance) {
            // If we've reached the right wall, after checking the left wall,
            // there is no way forward! Get angry.
            print("PANIC! No clear path found!");
            break;
          } else move_right(in_front_tolerance);
        }
      } else {
        move_forward(in_front_tolerance);
        checked_left = false;
      }
    } else { // Otherwise, fix our heading and then assume we're oriented the right way
      if (heading > 0) spin_CW(heading); else spin_CCW(heading);
      heading = 0;
    }
  }
}

bool close_enough(float f1, float f2) {
  return (abs(f1 - f2) < 0.1);
}
bool close_enough(float f1, float f2, float tolerance) {
  return (abs(f1 - f2) < tolerance);
}

float sensor_L () {
  return pulseIn(uss_pins[0], HIGH) * 0.034 / 2;
}
float sensor_FL () {
  return pulseIn(uss_pins[1], HIGH) * 0.034 / 2;
}
float sensor_FR () {
  return pulseIn(uss_pins[2], HIGH) * 0.034 / 2;
}
float sensor_R () {
  return pulseIn(uss_pins[3], HIGH) * 0.034 / 2;
}
