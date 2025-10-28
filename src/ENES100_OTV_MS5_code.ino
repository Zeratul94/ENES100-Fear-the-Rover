
/*Libraries */
#include "Enes100.h"


/* Objects */


/* Pins (FILLER VALUES RIGHT NOW)*/
const int fr_foward_pin = 0;     //PWM digital
const int fr_backward_pin = 1;   //PWM digital

const int fl_foward_pin = 2;     //PWM digital
const int fl_backward_pin = 3;   //PWM digital

const int br_foward_pin = 4;     //PWM digital
const int br_backward_pin = 5;   //PWM digital

const int bl_foward_pin = 6;     //PWM digital
const int bl_backward_pin = 7;   //PWM digital

const int TX_PIN = 10;
const int RX_PIN = 11;

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

const double rot_speed = 3.14/2; // Get through testing Units:rads/sec

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


void setup() {

  //subtask 4 Wireless receive
  Enes100.isConnected();
    Enes100.begin("Fear The Rover", MATERIAL, AcUro_ID, 1116, TX_PIN, RX_PIN);
    // At this point we know we are connected.
    Enes100.println("Connected...");
    Enes100.print("Hello World!");
    delay(100000);

}

void loop() {
  
  float x, y, t; bool v; // Declare variables to hold the data

  //subtask 1 have a safe robot
  
  //subtask 2
  move_foward(350); //move foward 3.5 m

  //subtask 3
  rot_cw(PI()/2);
  delay(1000);
  rot_cw(PI()/2);
  delay(1000);
  rot_cw(PI()/2);



  //subtask 5 Wireless transmit
  x = Enes100.getX(); 
    y = Enes100.getY(); 
    t = Enes100.getTheta(); 
    v = Enes100.isVisible(); 

    if (v) // If the ArUco marker is visible
    {
        Enes100.print(x); 
        Enes100.print(",");
        Enes100.print(y);
        Enes100.print(",");
        Enes100.println(t);
    }
    else { 
        Enes100.println("Not visible"); 
    }

  //subtask 6 Navigation code - no obstacles 


  //subtask 7 Navigation code - obstacle avoidance 


  //subtask 8 Obstacle sensing (demonstarte ultrasonic sensors have been calibrated)



}


