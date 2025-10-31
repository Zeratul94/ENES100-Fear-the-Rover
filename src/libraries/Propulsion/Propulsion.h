#include "Servo.h"

#ifndef propulsion_h // Include guards to prevent multiple inclusions
#define propulsion_h

/* Pins (FILLER VALUES RIGHT NOW)*/
inline constexpr char fr_forward_pin = 0;     // PWM digital
inline constexpr char fr_backward_pin = 1;   // PWM digital

inline constexpr char fl_forward_pin = 2;     // PWM digital
inline constexpr char fl_backward_pin = 3;   // PWM digital

inline constexpr char br_forward_pin = 4;     // PWM digital
inline constexpr char br_backward_pin = 5;   // PWM digital

inline constexpr char bl_forward_pin = 6;     // PWM digital
inline constexpr char bl_backward_pin = 7;   // PWM digital 

inline constexpr char claw_forward_pin = 8;   // PWM digital
inline constexpr char claw_backward_pin = 9; // PWM digital

inline constexpr char servo_sg_pin = 10;     // PWM digital
inline constexpr char servo_feedback = 11;   // analog

inline constexpr char tx_pin = 12; // Wi-Fi Module
inline constexpr char rx_pin = 13; // Wi-Fi Module
inline constexpr char aruco_ID = 97;

inline constexpr char loadcell_dout_pin = 14;
inline constexpr char loadcell_sck_pin = 15;

inline constexpr char uss_pins[4] = {16, 17, 18, 19}; // Ultrasonic Sensors

/* Other constants (FILLER VALUES RIGHT NOW) */
enum MissionState{
  GO_TO_CUBE,
  ADJUST_POSITION,
  GRAB_CUBE,
  DROP_CUBE,
  NAVIGATE_ENDZONE
};

inline constexpr double fr_nav_input = .15;
inline constexpr double fl_nav_input = .15;
inline constexpr double br_nav_input = .15;
inline constexpr double bl_nav_input = .15;

inline constexpr double nav_speed = 8; // Get through testing Units: cm/sec
inline constexpr double strafe_speed = 8; // Get through testing Units: cm.sec

inline constexpr double fr_rot_input = .15;
inline constexpr double fl_rot_input = .15;
inline constexpr double br_rot_input = .15;
inline constexpr double bl_rot_input = .15;

inline constexpr double grab_distance = 10.0; // cm, ultrasonic sensor distance to cube to grab it with the claw
inline constexpr double in_front_tolerance = 15.0; // cm, closest we are willing to get to an obstacle before avoiding it
inline constexpr double heading_epsilon = 5.0; // degrees, acceptable error in heading when navigating
inline constexpr double centering_epsilon = 1.0; // cm, acceptable error between front sensors when centering at cube
inline constexpr double grab_lineal_epsilon = 2.0; // cm, acceptable error in distance to cube when grabbing

inline constexpr double rot_speed = 90.; // degrees, Get through testing Units:rot/sec

//distance in cm
void move_forward(double distance);

//distance in cm
void move_backward(double distance);

//distance in cm
void move_right(double distance);

//distance in cm
void move_left(double distance);

void spin(double degs);

void set_servo(double angle);

double read_servo(Servo servo, double angle);

#endif