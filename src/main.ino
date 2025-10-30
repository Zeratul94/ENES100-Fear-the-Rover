
// #include <Arduino.h>
// #include <Servo.h>
// #include "Enes100.h"
// #include <HX711.h>
#include <propulsion.h>
#include <cstdlib>

/* Objects */
Servo claw_servo;
HX711 scale;

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
  Enes100.isConnected();
  Enes100.begin("Fear The Rover", MATERIAL, aruco_ID, 1116, tx_pin, rx_pin);
  Enes100.println("Connected...");
  Enes100.print("Hello World!");
  delay(100000);

  // Initialize objects
  claw_servo.attach(servo_sg_pin);
  scale.begin(loadcell_dout_pin, loadcell_sck_pin);

  // Initialize state
  mission_state = GO_TO_CUBE;
  heading = 0.;
  position[0] = 0; position[1] = 0;
  loopctr = 0;
}

void loop() {
  heading = Enes100.getTheta();
  position[0] = Enes100.getX(); position[1] = Enes100.getY();
  switch (mission_state) {
    case GO_TO_CUBE:
      navigate_to_mission();
      break;
    case ADJUST_POSITION:
      adjust_position();
      break;
    case GRAB_CUBE:
      complete_mission();
      break;
    case DROP_CUBE:
      drop_cube();
      break;
    case NAVIGATE_ENDZONE:
      navigate_to_endzone();
      break;
    default:
      Enes100.println("PANIC! Undefined mission state!");
      break;
  }

  loopctr++;
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

void navigate_to_endzone() { // OLD
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
            Enes100.println("PANIC! No clear path found!");
            break;
          } else move_right(in_front_tolerance);
        }
      } else {
        move_forward(in_front_tolerance);
        checked_left = false;
      }
    } else { // Otherwise, fix our heading and then assume we're oriented the right way
      spin(heading);
      heading = 0;
    }
  }
}

bool close_enough(float f1, float f2) {
  return (abs(f1 - f2) < 0.1);
}
bool close_enough(float f1, float f2, float epsilon) {
  return (abs(f1 - f2) < epsilon);
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
