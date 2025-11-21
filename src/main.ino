/*Libraries */
#include "Enes100.h"
#include "HX711.h"
#include <Servo.h>


/* Objects */
HX711 scale;
Servo claw_servo;

/* DRIVE PINS */
const int fr_foward_pin = 5;     //PWM digital
const int fr_backward_pin = 4;   //PWM digital

const int fl_foward_pin = 10;     //PWM digital
const int fl_backward_pin = 11;   //PWM digital

const int br_foward_pin = 3;     //PWM digital
const int br_backward_pin = 2;   //PWM digital

const int bl_foward_pin = 8;     //PWM digital
const int bl_backward_pin = 9;   //PWM digital

/* SERVO PINS */
const int servo_sg_pin = 6;     //PWM digital
const int servo_feedback = 0;   //analog

/* CLAW MOTOR PINS */
const int retract_pin = 12;     //PWM digital
const int deploy_pin = 13;      //PWM digital

/* LOADCELL PINS */
const int loadcell_dout_pin = 6;  //digital
const int loadcell_sck_pin = 5;   //digital

/* COMM PINS */
const int TX_PIN = 10;  //COMM
const int RX_PIN = 11;  //COMM

/* LIMIT SIWTCH PINS (FILLER)*/
const int deploy_limit = 23;  //digital
const int retract_limit = 48; //digital

/* Other constants (FILLER VALUES RIGHT NOW) */
const double fr_nav_input = 100;
const double fl_nav_input = 100;
const double br_nav_input = 100;
const double bl_nav_input = 130;

const double fr_rot_input = 100;
const double fl_rot_input = 100;
const double br_rot_input = 100;
const double bl_rot_input = 100;

const double claw_motor_input = 200;

const double nav_speed = 8; // Get through testing Units: cm/sec   
const double strafe_speed = 8; //get through testing Units: cm.sec
const double rot_speed = 3.14/2; // Get through testing Units:rads/sec

const double light = 3301379; //NEED TO CHANGE
const double medium = 3366831; //NEED TO CHANGE
const double heavy = 3527879; //NEED TO CHANGE

double foam_pos = 150; //FILLER 
double plastic_pos = 180; //FILLER

String material;
String weight_class;

int AcUro_ID = 97;

//distance in cm
void move_foward(double distance) {
  analogWrite(fl_foward_pin, fl_nav_input);
  analogWrite(fr_foward_pin, fr_nav_input);
  analogWrite(bl_foward_pin, bl_nav_input);
  analogWrite(br_foward_pin, br_nav_input);
  delay((distance/nav_speed)*pow(10,3));
  analogWrite(fl_foward_pin, 0);
  analogWrite(fr_foward_pin, 0);
  analogWrite(bl_foward_pin, 0);
  analogWrite(br_foward_pin, 0);
}

void move_foward() {
  analogWrite(fl_foward_pin, fl_nav_input);
  analogWrite(fr_foward_pin, fr_nav_input);
  analogWrite(bl_foward_pin, bl_nav_input);
  analogWrite(br_foward_pin, br_nav_input);
}

//distance in cm
void move_backward(double distance) {
  analogWrite(fl_backward_pin, fl_nav_input);
  analogWrite(fr_backward_pin, fr_nav_input);
  analogWrite(bl_backward_pin, bl_nav_input);
  analogWrite(br_backward_pin, br_nav_input);
  delay((distance/nav_speed)*pow(10,3));
  analogWrite(fl_backward_pin, 0);
  analogWrite(fr_backward_pin, 0);
  analogWrite(bl_backward_pin, 0);
  analogWrite(br_backward_pin, 0);
}

void move_backward() {
  analogWrite(fl_backward_pin, fl_nav_input);
  analogWrite(fr_backward_pin, fr_nav_input);
  analogWrite(bl_backward_pin, bl_nav_input);
  analogWrite(br_backward_pin, br_nav_input);
}

//distance in cm
void move_right(double distance) {
  analogWrite(fl_foward_pin, fl_nav_input);
  analogWrite(fr_backward_pin, fr_nav_input);
  analogWrite(bl_backward_pin, bl_nav_input);
  analogWrite(br_foward_pin, br_nav_input);
  delay((distance/strafe_speed)*pow(10,3));
  analogWrite(fl_foward_pin, 0);
  analogWrite(fr_backward_pin, 0);
  analogWrite(bl_backward_pin, 0);
  analogWrite(br_foward_pin, 0);
}

void move_right() {
  analogWrite(fl_foward_pin, fl_nav_input);
  analogWrite(fr_backward_pin, fr_nav_input);
  analogWrite(bl_backward_pin, bl_nav_input);
  analogWrite(br_foward_pin, br_nav_input);
}

//distance in cm
void move_left(double distance) {
  analogWrite(fl_backward_pin, fl_nav_input);
  analogWrite(fr_foward_pin, fr_nav_input);
  analogWrite(bl_foward_pin, bl_nav_input);
  analogWrite(br_backward_pin, br_nav_input);
  delay((distance/strafe_speed)*pow(10,3));
  analogWrite(fl_backward_pin, 0);
  analogWrite(fr_foward_pin, 0);
  analogWrite(bl_foward_pin, 0);
  analogWrite(br_backward_pin, 0);
}

void move_left() {
  analogWrite(fl_backward_pin, fl_nav_input);
  analogWrite(fr_foward_pin, fr_nav_input);
  analogWrite(bl_foward_pin, bl_nav_input);
  analogWrite(br_backward_pin, br_nav_input);
}

void spin_CCW(double radians) {
  analogWrite(fl_backward_pin, fl_nav_input);
  analogWrite(fr_foward_pin, fr_nav_input);
  analogWrite(bl_backward_pin, bl_nav_input);
  analogWrite(br_foward_pin, br_nav_input);
  delay((radians/rot_speed)*pow(10,3));
  analogWrite(fl_backward_pin, 0);
  analogWrite(fr_foward_pin, 0);
  analogWrite(bl_backward_pin, 0);
  analogWrite(br_foward_pin, 0);
}

void spin_CCW() {
  analogWrite(fl_backward_pin, fl_nav_input);
  analogWrite(fr_foward_pin, fr_nav_input);
  analogWrite(bl_backward_pin, bl_nav_input);
  analogWrite(br_foward_pin, br_nav_input);
}

void spin_CW(double radians) {
  analogWrite(fl_foward_pin, fl_nav_input);
  analogWrite(fr_backward_pin, fr_nav_input);
  analogWrite(bl_foward_pin, bl_nav_input);
  analogWrite(br_backward_pin, br_nav_input);
  delay((radians/rot_speed)*pow(10,3));
  analogWrite(fl_foward_pin, 0);
  analogWrite(fr_backward_pin, 0);
  analogWrite(bl_foward_pin, 0);
  analogWrite(br_backward_pin, 0);
}

void spin_CW() {
  analogWrite(fl_foward_pin, fl_nav_input);
  analogWrite(fr_backward_pin, fr_nav_input);
  analogWrite(bl_foward_pin, bl_nav_input);
  analogWrite(br_backward_pin, br_nav_input);
}


void stop_motor() {
  analogWrite(fl_foward_pin, 0);
  analogWrite(fl_backward_pin, 0);
  analogWrite(fr_foward_pin, 0);
  analogWrite(fr_backward_pin, 0);
  analogWrite(bl_foward_pin, 0);
  analogWrite(bl_backward_pin, 0);
  analogWrite(br_foward_pin, 0);
  analogWrite(br_backward_pin, 0);
}

void deploy_claw() {
  
  while (digitalRead(deploy_limit) == HIGH)
    analogWrite(deploy_pin, claw_motor_input);

  analogWrite(deploy_pin, 0);  
  
}

void retract_claw() {
  while (digitalRead(retract_limit) == HIGH)
    analogWrite(retract_pin, claw_motor_input);

  analogWrite(retract_pin, 0); 
}

void set_servo(double angle) {
  claw_servo.write(angle);
}

double read_servo(){
  return analogRead(servo_feedback);
}

void open_claw() {
  claw_servo.write(100);
}

double close_claw(){
  claw_servo.write(130);
}


void setup() {
  move_foward();
  delay(5000);
  stop_motor();
  move_backward();
  delay(5000);
  stop_motor();
  move_left();
  delay(5000);
  stop_motor();
  move_right();
  delay(5000);
  stop_motor();
  spin_CW();
  delay(5000);
  stop_motor();
  spin_CCW();
  delay(5000);
  stop_motor();
  

}

void loop() {
  

}
