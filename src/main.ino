#include <Arduino.h>
#include <Servo.h>
#include "Enes100.h"
#include <HX711.h>
//#include <cstdlib>
//#include <string>

/* Objects */
Servo claw_servo;
HX711 scale;

enum MissionState{
  GO_TO_CUBE,
  ADJUST_POSITION,
  GRAB_CUBE,
  DROP_CUBE,
  NAVIGATE_ENDZONE
};

/* SYSTEM PARAMETERS */

// Pins
char fr_forward_pin = 0;     // PWM digital
char fr_backward_pin = 1;   // PWM digital
char fl_forward_pin = 2;     // PWM digital
char fl_backward_pin = 3;   // PWM digital
char br_forward_pin = 4;     // PWM digital
char br_backward_pin = 5;   // PWM digital
char bl_forward_pin = 6;     // PWM digital
char bl_backward_pin = 7;   // PWM digital 
char claw_forward_pin = 8;   // PWM digital
char claw_backward_pin = 9; // PWM digital
char servo_sg_pin = 10;     // PWM digital
char servo_feedback = 11;   // analog
char tx_pin = 12; // Wi-Fi Module
char rx_pin = 13; // Wi-Fi Module
char aruco_ID = 97;
char loadcell_dout_pin = 14;
char loadcell_sck_pin = 15;
char uss_echo_pins[4] = {30, 32, 34, 36}; // Ultrasonic Sensor receive
char uss_trig_pins[4] = {31, 33, 35, 37}; // Ultrasonic Sensor pulse (should always be HIGH)

// Movement parameters
double fr_nav_input = .15;
double fl_nav_input = .15;
double br_nav_input = .15;
double bl_nav_input = .15;
double nav_speed = 8; // Get through testing Units: cm/sec
double strafe_speed = 8; // Get through testing Units: cm.sec
double fr_rot_input = .15;
double fl_rot_input = .15;
double br_rot_input = .15;
double bl_rot_input = .15;


// Navigation parameters
double grab_distance = 5.0; // cm, ultrasonic sensor distance to cube to grab it with the claw
double in_front_tolerance = 10.0; // cm, closest we are willing to get to an obstacle before avoiding it
double heading_epsilon = 5.0; // degrees, acceptable error in heading when navigating
double centering_epsilon = 1.0; // cm, acceptable error between front sensors when centering at cube
double grab_lineal_epsilon = 2.0; // cm, acceptable error in distance to cube when grabbing
double rot_speed = 90.; // degrees, Get through testing Units:rot/sec

// State values
MissionState mission_state = GO_TO_CUBE;
double heading;
bool cube_material; // true = squishy foam, false = hard plastic
char cube_weight; // 0 = lightest weight class, then 1, then 2 is heaviest
double position[2];

short loopctr;
double startpos[2];

void setup() {
  // Initializes ENES100 transmit/receive
  Serial.begin(9600);
  Enes100.isConnected();
  Enes100.begin("Fear The Rover", MATERIAL, aruco_ID, 1116, tx_pin, rx_pin);
  Enes100.println("Connected...");
  Enes100.print("Hello World!");
  delay(1000);

  // Initialize objects
  claw_servo.attach(servo_sg_pin);
  scale.begin(loadcell_dout_pin, loadcell_sck_pin);
  for (int i=0; i<4; i++) {
    digitalWrite(uss_trig_pins[i], HIGH);
  }

  // Initialize state
  mission_state = GO_TO_CUBE;
  heading = 0.;
  position[0] = 0; position[1] = 0;
  loopctr = 0;
}

void loop() {
  Serial.println("Starting frame");
  // heading = Enes100.getTheta();
  // position[0] = Enes100.getX(); position[1] = Enes100.getY();
  // switch (mission_state) {
  //   case GO_TO_CUBE:
  //     navigate_to_mission();
  //     break;
  //   case ADJUST_POSITION:
  //     adjust_position();
  //     break;
  //   default:
  //     Enes100.println("PANIC! Undefined mission state!");
  //     break;
  // }
  Serial.println("L: ");
  Serial.print(sensor_L());
  Serial.println("FL: ");
  Serial.print(sensor_FL());
  Serial.println("FR: ");
  Serial.print(sensor_FR());
  Serial.println("R: ");
  Serial.print(sensor_R());

  //loopctr++;
}

void navigate_to_mission() {
  // On the first loop, try to face toward the mission
  if (loopctr == 0) {
    Enes100.println("Navigating to mission...");
    spin((90 ? heading < 0 : -90) - heading); // Face toward the mission location
    return;
  }

  // Subsequent loops: if we're facing the mission, move up if it's clear!
  if (close_enough(heading, 90, heading_epsilon) || close_enough(heading, -90, heading_epsilon)) {
    if (sensor_FL() >= in_front_tolerance && sensor_FR() >= in_front_tolerance) {
      move_forward(in_front_tolerance);
    // If it's not clear, we must have gotten to the cube
    } else {
      Enes100.println("Cube detected!");
      mission_state = ADJUST_POSITION;
      loopctr = 0;
    }
  // If we're facing the wrong way, adjust
  } else {
    spin((90 - heading) ? heading > 0 : (-90 - heading)); // Face toward the mission location
  }
}

// Place the OTV so that both sensors pick up the cube
// GEDALYA ASSUMES that this will mean the cube is centered and at
// grabbing distance
void adjust_position() {
  Enes100.println("Adjusting position...");
  bool left_detect = sensor_FL() <= grab_distance;
  bool right_detect = sensor_FR() <= grab_distance;
  bool centered = close_enough(sensor_FL(), sensor_FR(), centering_epsilon);
  
  if (left_detect && right_detect && centered) {
    if (close_enough((sensor_FL() + sensor_FR())/2, grab_distance, grab_lineal_epsilon)) {
      Enes100.println("Positioned and ready!");
      mission_state = GRAB_CUBE;
      loopctr = 0;
      return;
    } else {
      move_backward(grab_lineal_epsilon/2);
      return;
    }
  }

  if (left_detect) {
    move_left(centering_epsilon/2);
  } else if (right_detect) {
    move_right(centering_epsilon/2);
  }

  if (!left_detect && !right_detect) {
    move_forward(grab_lineal_epsilon/2);
    return;
  }
}

bool close_enough(float f1, float f2) {
  return (abs(f1 - f2) < 0.1);
}
bool close_enough(float f1, float f2, float epsilon) {
  return (abs(f1 - f2) < epsilon);
}

float sensor_L () {
  return pulseIn(uss_echo_pins[0], HIGH) * 0.034 / 2;
}
float sensor_FL () {
  return pulseIn(uss_echo_pins[1], HIGH) * 0.034 / 2;
}
float sensor_FR () {
  return pulseIn(uss_echo_pins[2], HIGH) * 0.034 / 2;
}
float sensor_R () {
  return pulseIn(uss_echo_pins[3], HIGH) * 0.034 / 2;
}

//distance in cm
void move_forward(double distance) {
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
  analogWrite(br_forward_pin, br_nav_input);
  delay((distance/strafe_speed)*1000);
  analogWrite(fl_forward_pin, 0);
  analogWrite(fr_backward_pin, 0);
  analogWrite(bl_backward_pin, 0);
  analogWrite(br_forward_pin, 0);
}

//distance in cm
void move_left(double distance) {
  analogWrite(fl_backward_pin, fl_nav_input);
  analogWrite(fr_forward_pin, fr_nav_input);
  analogWrite(bl_forward_pin, bl_nav_input);
  analogWrite(br_backward_pin, br_nav_input);
  delay((distance/strafe_speed)*1000);
  analogWrite(fl_backward_pin, 0);
  analogWrite(fr_forward_pin, 0);
  analogWrite(bl_forward_pin, 0);
  analogWrite(br_backward_pin, 0);
}

// degrees > 0 means CW, < 0 means CCW
void spin(double degs) {
  if (degs >= 0) {
    analogWrite(fl_forward_pin, fl_nav_input);
    analogWrite(fr_backward_pin, fr_nav_input);
    analogWrite(bl_forward_pin, bl_nav_input);
    analogWrite(br_backward_pin, br_nav_input);
    delay((degs/rot_speed)*1000);
    analogWrite(fl_forward_pin, 0);
    analogWrite(fr_backward_pin, 0);
    analogWrite(bl_forward_pin, 0);
    analogWrite(br_backward_pin, 0);
  } else {
    analogWrite(fl_backward_pin, fl_nav_input);
    analogWrite(fr_forward_pin, fr_nav_input);
    analogWrite(bl_backward_pin, bl_nav_input);
    analogWrite(br_forward_pin, br_nav_input);
    delay((degs/rot_speed)*1000);
    analogWrite(fl_backward_pin, 0);
    analogWrite(fr_forward_pin, 0);
    analogWrite(bl_backward_pin, 0);
    analogWrite(br_forward_pin, 0);
  }
}

void set_servo(Servo servo, double angle) {
  servo.write(angle);
}

double read_servo(double angle){
  return analogRead(servo_feedback);
}
