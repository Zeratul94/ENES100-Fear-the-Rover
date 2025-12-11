#include <Arduino.h>
#include <Servo.h>
#include <Enes100.h>
#include <HX711.h>
//#include <cstdlib>
//#include <string>

/* Objects */
Servo claw_servo;
HX711 scale;

enum MissionState{
  GO_TO_CUBE,
  // ADJUST_POSITION,
  IDENTIFY_CUBE,
  GRAB_CUBE,
  NAVIGATE_ENDZONE
};

/* SYSTEM PARAMETERS */

// Pins
int fr_forward_pin = 4;     // PWM digital
int fr_backward_pin = 5;   // PWM digital
int fl_forward_pin = 11;     // PWM digital
int fl_backward_pin = 10;   // PWM digital
int br_forward_pin = 3;     // PWM digital
int br_backward_pin = 2;   // PWM digital
int bl_forward_pin = 9;     // PWM digital
int bl_backward_pin = 8;   // PWM digital

int deploy_pin = 13;   // PWM digital
int retract_pin = 12; // PWM digital

int servo_sg_pin = 6;     // PWM digital
int servo_feedback = 0;   // analog
int tx_pin = 14; // Wi-Fi Module
int rx_pin = 15; // Wi-Fi Module
int aruco_ID = 67;
int loadcell_dout_pin = 38;
int loadcell_sck_pin = 39;
int uss_echo_pins[4] = {30, 32, 34, 36}; // Ultrasonic Sensor receive
int uss_trig_pins[4] = {31, 33, 35, 37}; // Ultrasonic Sensor pulse

// Movement parameters
double fr_nav_input = .7 * 255;
double fl_nav_input = .7 * 255;
double br_nav_input = .8 * 255;
double bl_nav_input = .8 * 255;
double nav_speed    = .7 * 20.5; // Arbitrary units
double strafe_speed = .7 * 20.5; // Arbitrary units
double fr_rot_input = .7 * 255;
double fl_rot_input = .7 * 255;
double br_rot_input = .7 * 255;
double bl_rot_input = .7 * 255;


// Navigation parameters
double grab_distance = 5.0; // cm, ultrasonic sensor distance to cube to grab it with the claw
double in_front_tolerance = 7.5; // cm, closest we are willing to get to an obstacle before avoiding it
double otv_width = 25.;
double heading_epsilon = 4.0; // degrees, acceptable error in heading when navigating
double centering_epsilon = 1.0; // cm, acceptable error between front sensors when centering at cube
double grab_lineal_epsilon = 2.0; // cm, acceptable error in distance to cube when grabbing
double rot_speed = 32.; // degrees, Get through testing Units:rot/sec

// State values
MissionState mission_state = GO_TO_CUBE;
double heading;
bool cube_material; // true = squishy foam, false = hard plastic
int cube_weight; // 0 = lightest weight class, then 1, then 2 is heaviest
double position[2];
bool checked_left = false;
bool in_endzone = false;
bool wasJustStrafing = false;

short loopctr;
double startpos[2];

void setup() {
  // Initializes ENES100 transmit/receive
  Serial.begin(9600);
  Enes100.isConnected();
  Enes100.begin("Fear The Rover", MATERIAL, aruco_ID, 1201, tx_pin, rx_pin);
  Enes100.println("Fear the Rover connected!");
  delay(1000);


  // Initialize special pins
  for (int i=0; i<4; i++) {
    pinMode(uss_trig_pins[i], OUTPUT);
    digitalWrite(uss_trig_pins[i], LOW); // Default to LOW
    pinMode(uss_echo_pins[i], INPUT);
  }
  pinMode(deploy_limit, INPUT_PULLUP);
  pinMode(retract_limit, INPUT_PULLUP);

  // Initialize objects
  scale.begin(loadcell_dout_pin, loadcell_sck_pin);
  claw_servo.attach(servo_sg_pin);

  // Initialize state
  mission_state = GO_TO_CUBE;
  heading = 0.;
  position[0] = 0; position[1] = 0;
  loopctr = 0;

  analogWrite(deploy_pin, 0);
  analogWrite(retract_pin, 255);
  delay(500);
  analogWrite(retract_pin, 0);
}

void loop() {
  navigate_to_mission();
  heading = Enes100.getTheta();
  position[0] = Enes100.getX(); position[1] = Enes100.getY();

  switch (mission_state) {
    case GO_TO_CUBE:
      navigate_to_mission();
      break;
    case ADJUST_POSITION:
      adjust_position();
      break;
    case IDENTIFY_CUBE:
      identify_material();
      break;
    case GRAB_CUBE:
      grab_and_weigh();
      break;
    case NAVIGATE_ENDZONE:
      navigate_to_endzone();
      break;
    case 5:
        Enes100.println("PANIC! No clear path found!");
        break;
    default:
      Enes100.println("PANIC! Undefined mission state!");
      break;
  }

  loopctr++;
}

void navigate_to_mission() {
  spin(startheading); //point towards direction 0

  if (startheading > 0){ 
    spin(PI/2);
  } else {
    spin(-PI/2);
  }

  move_forward(78); //cm
}

void identify_material() {
  int material_reads_foam = 0;
  int material_reads_plastic = 0;
  material_test:
  for (int i=0; i<5; i++) {
    switch (detect_material()) {
      case 1:
        material_reads_foam += 1;
        break;
      case 2:
        material_reads_plastic += 1;
        break;
      default:
        break;
    }
  }

  if (material_reads_plastic > (material_reads_foam + 1)) {
    material = "Plastic";
    Serial.println("The cube is plastic!");
    mission_state += 1;
    return;
  } else if (material_reads_foam > (material_reads_plastic + 1)) {
    material = "Foam";
    Serial.println("The cube is foam!");
    mission_state += 1;
    return;
  } else {
    Serial.println("Test inconclusive; checking again...");
    goto material_test;
  }
}

int detect_material() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor
  float cm = sensor_FR();
  float cm2 = sensor_FL();
  
  delay(500);

  if (cm == 805 || inches == 316 || cm2 == 805 || inches2 == 316) {
    Serial.println("Read - ");
    Serial.print(cm);
    return 1;
  }
  else if (cm <= 30 || inches <= 10 || cm2 <= 30 || inches2 <= 10) {
    Serial.println("Read - ");
    Serial.print(cm);
    return 2;
  }
  return -1;
}

void grab_and_weigh() {
  open_claw();
  delay(500);
  deploy_claw(claw_motor_no_load_input);
  delay(500);
  close_claw();
  delay(1000);
  retract_claw(claw_motor_load_input);
  delay(500);
  open_claw();
  delay(5000);
  weight = abs(scale.get_units(10));

  if (abs(light-weight) < abs(medium-weight))
    Enes100.println("The cube is in weight class Light.");
  else if (abs(medium-weight) < abs(heavy-weight))
    Enes100.println("The cube is in weight class Medium.");
  else 
    Enes100.println("The cube is in weight class Heavy.");

  close_claw();
  delay(100);
  deploy_claw(claw_motor_load_input);
  open_claw();
  delay(250);
  retract_claw(claw_motor_no_load_input);

  mission_state += 1;
  loopctr = 0;
  return;
}

void navigate_to_endzone() {
  if loopctr == 0 {
    spin(-heading);
  }

  // Loop through these instructions until we get there
  if (!in_endzone) {
    if (close_enough(heading, 0, 0.0872664626)) { // If we're pointing the right way...
      // Either avoid obstacles or move forward
      if (sensorBeyond(sensor_FL(), in_front_tolerance) && sensorBeyond(sensor_FR(), in_front_tolerance)) {
        if (wasJustStrafing) {
          if (checked_left) move_right(otv_width);
          else move_left(otv_width);
          move_forward(in_front_tolerance * 2);
          wasJustStrafing = false;
        }
        else {
          move_forward(in_front_tolerance);
        }
        checked_left = false;
        return;
      }
      // Otherwise:
      // Move left. If we get to the wall without clearing in front, move right
      if (!checked_left) {
        if (!sensorBeyond(sensor_L(), in_front_tolerance*1.75)) { checked_left = true; }
        if (!checked_left) { move_left(in_front_tolerance); wasJustStrafing = true; }
      } else {
        if (!sensorBeyond(sensor_R(), in_front_tolerance*1.75)) {
          // If we've reached the right wall, after checking the left wall,
          // there is no way forward! Get angry.
          mission_state = 5; // Panic state
        } else { move_right(in_front_tolerance*2); wasJustStrafing = true; }
      }
      return;
    }
    // Otherwise, fix our heading and then assume we're oriented the right way
    spin(heading);
    heading = 0;
    return;
  }
}

bool close_enough(float f1, float f2) {
  return (abs(f1 - f2) < 0.1);
}
bool close_enough(float f1, float f2, float epsilon) {
  return (abs(f1 - f2) < epsilon);
}

bool sensorBeyond(float sensorVal, float threshold) {
  return (sensorVal > threshold || sensorVal == 0);
}

float usensor(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor
  pinMode(echoPin, INPUT);
  unsigned long duration = pulseIn(echoPin, HIGH, 1000000); // Timeout for 1000ms
  return duration * 0.034 * 0.6 / 2;
}

float sensor_L () {
  return usensor(uss_trig_pins[0], uss_echo_pins[0]);
}
float sensor_FL () {
  return usensor(uss_trig_pins[1], uss_echo_pins[1]);
}
float sensor_FR () {
  return usensor(uss_trig_pins[2], uss_echo_pins[2]);
}
float sensor_R () {
  return usensor(uss_trig_pins[3], uss_echo_pins[3]);
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
    degs = -degs;
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

void deploy_claw(double input) {
  
  while (digitalRead(deploy_limit) == HIGH)
    analogWrite(deploy_pin, input);

  analogWrite(deploy_pin, 0);  
  
}

void retract_claw(double input) {
  while (digitalRead(retract_limit) == HIGH)
    analogWrite(retract_pin, input);

  analogWrite(retract_pin, 0); 
}

void set_servo(double angle) {
  claw_servo.write(angle);
}

double read_servo(){
  return analogRead(servo_feedback);
}

void open_claw() {
  claw_servo.write(90);
}

double close_claw(){
  claw_servo.write(130);
}