// #include <Arduino.h>
// #include <Servo.h>
// #include "Enes100.h"
#include <propulsion.h>
#include <cstdlib>

/* Constants */
const float in_front_tolerance = 20.f; // No clue what scale of number this should be
double heading = 0.; // Degrees pls?

/* Pins (FILLER VALUES RIGHT NOW)*/
const char fr_foward_pin = 0;     //PWM digital
const char fr_backward_pin = 1;   //PWM digital

const char fl_foward_pin = 2;     //PWM digital
const char fl_backward_pin = 3;   //PWM digital

const char br_foward_pin = 4;     //PWM digital
const char br_backward_pin = 5;   //PWM digital

const char bl_foward_pin = 6;     //PWM digital
const char bl_backward_pin = 7;   //PWM digital

const char claw_foward_pin = 8;   //PWM digital
const char claw_backward_pin = 9; //PWM digital

const char servo_sg_pin = 10;     //PWM digital
const char servo_feedback = 11;   //analog

const char uss_pins[4] = {1, 2, 3, 4}; // four pins, one per sensor

/* Other constants (FILLER VALUES RIGHT NOW) */
const double fr_nav_input = .15;
const double fl_nav_input = .15;
const double br_nav_input = .15;
const double bl_nav_input = .15;

const double nav_speed = 8; // Get through testing Units: cm/sec   
const double strafe_speed = 8; //get through testing Units: cm.sec

const double fr_rot_input = .15;
const double fl_rot_input = .15;
const double br_rot_input = .15;
const double bl_rot_input = .15;

const double rot_speed = 3.14/2; // Get through testing Units:rot/sec



/* Objects */
Servo claw_servo;

void setup() {
  // put your setup code here, to run once:
  claw_servo.attach(servo_sg_pin);
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
      if (heading > 0) spin_CW(heading) else spin_CCW(heading);
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