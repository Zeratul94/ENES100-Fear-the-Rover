
/*Libraries */
// #include <Arduino.h>
// #include <Servo.h>
// #include "Enes100.h"
#include <propulsion.h>
#include <cstdlib>

//distance in cm
void move_foward(double distance) {
  analogWrite(fl_forward_pin, fl_nav_input);
  analogWrite(fr_forward_pin, fr_nav_input);
  analogWrite(bl_forward_pin, bl_nav_input);
  analogWrite(br_forward_pin, br_nav_input);
  delay((distance/nav_speed)*1000);
  analogWrite(fl_forward_pin, 0);
  analogWrite(fr_forward_pin, 0);
  analogWrite(bl_forward_pin, 0);
  analogWrite(br_forward_pin, 0);
}

//distance in cm
void move_backward(double distance) {
  analogWrite(fl_backward_pin, fl_nav_input);
  analogWrite(fr_backward_pin, fr_nav_input);
  analogWrite(bl_backward_pin, bl_nav_input);
  analogWrite(br_backward_pin, br_nav_input);
  delay((distance/nav_speed)*1000);
  analogWrite(fl_backward_pin, 0);
  analogWrite(fr_backward_pin, 0);
  analogWrite(bl_backward_pin, 0);
  analogWrite(br_backward_pin, 0);
}

//distance in cm
void move_right(double distance) {
  analogWrite(fl_forward_pin, fl_nav_input);
  analogWrite(fr_backward_pin, fr_nav_input);
  analogWrite(bl_backward_pin, bl_nav_input);
  analogWrite(br_foward_pin, br_nav_input);
  delay((distance/strafe_speed)*1000);
  analogWrite(fl_forward_pin, 0);
  analogWrite(fr_backward_pin, 0);
  analogWrite(bl_backward_pin, 0);
  analogWrite(br_foward_pin, 0);
}

//distance in cm
void move_left(double distance) {
  analogWrite(fl_backward_pin, fl_nav_input);
  analogWrite(fr_forward_pin, fr_nav_input);
  analogWrite(bl_foward_pin, bl_nav_input);
  analogWrite(br_backward_pin, br_nav_input);
  delay((distance/strafe_speed)*1000);
  analogWrite(fl_backward_pin, 0);
  analogWrite(fr_forward_pin, 0);
  analogWrite(bl_foward_pin, 0);
  analogWrite(br_backward_pin, 0);
}

// degrees > 0 means CW, < 0 means CCW
void spin(double degs) {
  if (degs >= 0) {
    analogWrite(fl_forward_pin, fl_nav_input);
    analogWrite(fr_backward_pin, fr_nav_input);
    analogWrite(bl_foward_pin, bl_nav_input);
    analogWrite(br_backward_pin, br_nav_input);
    delay((degs/rot_speed)*1000);
    analogWrite(fl_forward_pin, 0);
    analogWrite(fr_backward_pin, 0);
    analogWrite(bl_foward_pin, 0);
    analogWrite(br_backward_pin, 0);
  } else {
    analogWrite(fl_backward_pin, fl_nav_input);
    analogWrite(fr_forward_pin, fr_nav_input);
    analogWrite(bl_backward_pin, bl_nav_input);
    analogWrite(br_foward_pin, br_nav_input);
    delay((degs/rot_speed)*1000);
    analogWrite(fl_backward_pin, 0);
    analogWrite(fr_forward_pin, 0);
    analogWrite(bl_backward_pin, 0);
    analogWrite(br_foward_pin, 0);
  }
}

void set_servo(Servo servo, double angle) {
  servo.write(angle);
}

double read_servo(double angle){
  return analogRead(servo_feedback);
}