
/*Libraries */
#include <Servo.h>
#include "Enes100.h"
#include "HX711.h"

/* Objects */
Servo claw_servo;
HX711 scale;

/* Pins (FILLER VALUES RIGHT NOW)*/
const int fr_foward_pin = 0;     //PWM digital
const int fr_backward_pin = 1;   //PWM digital

const int fl_foward_pin = 2;     //PWM digital
const int fl_backward_pin = 3;   //PWM digital

const int br_foward_pin = 4;     //PWM digital
const int br_backward_pin = 5;   //PWM digital

const int bl_foward_pin = 6;     //PWM digital
const int bl_backward_pin = 7;   //PWM digital

const int claw_foward_pin = 8;   //PWM digital
const int claw_backward_pin = 9; //PWM digital

const int servo_sg_pin = 10;     //PWM digital
const int servo_feedback = 11;   //analog

const int loadcell_dout_pin = 12;
const int loadcell_sck_pin = 13;

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

void set_servo(double angle) {
  claw_servo.write(angle);
}

double read_servo(double angle){
  return analogRead(servo_feedback);
}

void setup() {
  //instantiate objects
  //claw_servo.attach(servo_sg_pin);

  Serial.println("HX711 Demo");
  Serial.println("Initializing the scale");

  scale.begin(loadcell_dout_pin, loadcell_sck_pin);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());      // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)
            
  scale.set_scale(-459.542);
  //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  Serial.println("Readings:");

}

void loop() {
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 5);

  delay(5000);
}

