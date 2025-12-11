/*Libraries */
#include "Enes100.h"
#include "HX711.h"
#include <Servo.h>


/* Objects */
HX711 scale;
Servo claw_servo;

/* DRIVE PINS */
const int fr_foward_pin = 5;     //PWM digitalf
const int fr_backward_pin = 4;   //PWM digital

const int fl_foward_pin = 10;     //PWM digital
const int fl_backward_pin = 11;   //PWM digital

const int br_foward_pin = 2;     //PWM digital
const int br_backward_pin = 3;   //PWM digital

const int bl_foward_pin = 8;     //PWM digital
const int bl_backward_pin = 9;   //PWM digital

/* SERVO PINS */
const int servo_sg_pin = 6;     //PWM digital
const int servo_feedback = 0;   //analog

/* CLAW MOTOR PINS */
const int retract_pin = 13;     //PWM digital
const int deploy_pin = 12;      //PWM digital

/* LOADCELL PINS */
const int loadcell_dout_pin = 38;  //digital
const int loadcell_sck_pin = 39;   //digital

int uss_echo_pins[4] = {30, 32, 34, 36}; // Ultrasonic Sensor receive
int uss_trig_pins[4] = {31, 33, 35, 37}; // Ultrasonic Sensor pulse

/* COMM PINS */
const int TX_PIN = 14;  //COMM
const int RX_PIN = 15;  //COMM

/* LIMIT SIWTCH PINS (FILLER)*/
const int deploy_limit = 47;  //digital
const int retract_limit = 46; //digital

/* Ultrasonic Sensor Pins */
int trigPin = 33;    // Trigger
int echoPin = 32;
int trigPin2 = 35;
int echoPin2 = 34;    // Echo
long duration;

/* Other constants (FILLER VALUES RIGHT NOW) */
const double fr_nav_input = 100;
const double fl_nav_input = 100;
const double br_nav_input = 100;
const double bl_nav_input = 100;

const double fr_rot_input = 100;
const double fl_rot_input = 100;
const double br_rot_input = 100;
const double bl_rot_input = 100;

const double claw_motor_no_load_input = 100;
const double claw_motor_load_input = 255;

const double nav_speed = 8; // Get through testing Units: cm/sec   
const double strafe_speed = 8; //get through testing Units: cm.sec
const double rot_speed = 3.14/2; // Get through testing Units:rads/sec

const double light = 337628; //NEED TO CHANGE
const double medium = 286715; //NEED TO CHANGE
const double heavy = 229685; //NEED TO CHANGE

double foam_pos = 150; //FILLER 
double plastic_pos = 180; //FILLER

String material;
String weight_class;

int AcUro_ID = 257;

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


void setup() {

  
  //Enes100.isConnected();
  //Enes100.begin("Fear The Rover", MATERIAL, AcUro_ID, 1116, TX_PIN, RX_PIN);
  
  int count = 0;
  double sum = 0;
  double pos;
  double weight;

  pinMode(deploy_limit, INPUT_PULLUP);
  pinMode(retract_limit, INPUT_PULLUP);

  for (int i=0; i<4; i++) {
    pinMode(uss_trig_pins[i], OUTPUT);
    digitalWrite(uss_trig_pins[i], LOW); // Default to LOW
    pinMode(uss_echo_pins[i], INPUT);
  }
  
  /* Initalize objects */
  scale.begin(loadcell_dout_pin, loadcell_sck_pin);
  claw_servo.attach(servo_sg_pin);
  Serial.begin(9600);

  //navigate to the mission
  spin(startheading); //point towards direction 0

  if (startheading > 0){ 
    spin(PI/2);
  } else {
    spin(-PI/2);
  }

  move_forward(78); //cm

  mission_state = ADJUST_POSITION;
  /* MATERIAL DETECTION - -1 is none detected, 1 is foam, 2 is plastic */
  identify_cube();
  
  /* PICK UP AND WEIGH DETECTION */
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
    weight_class = "light";
  else if (abs(medium-weight) < abs(heavy-weight))
    weight_class = "medium";
  else 
    weight_class = "heavy";

  close_claw();
  delay(100);
  deploy_claw(claw_motor_load_input);
  open_claw();
  delay(250);
  retract_claw(claw_motor_no_load_input);

  Serial.println(weight_class);

  /* SEND TO WIFI weight_class and material */
  startheading = Enes100.getTheta();
  spin(startheading); //point towards direction 0
}

void loop() {

}

void identify_cube() {
  int material_reads_foam = 0;
  int material_reads_plastic = 0;
  material_test:
  for (int i=0; i<5; i++) {
    switch (check_mat()) {
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
    return;
  } if (material_reads_foam > (material_reads_plastic + 1)) {
    material = "Foam";
    Serial.println("The cube is foam!");
    return;
  }
  Serial.println("Test inconclusive; checking again...");
  goto material_test;
}

int check_mat() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor
  float cm = sensor_FR();
  float cm2 = sensor_FL();
  
  delay(500);

  if (sensorBeyond(cm, 100) || sensorBeyond(cm2, 100)) {
    Serial.print("Read - ");
    Serial.println(cm);
    return 1;
  }
  else if (!sensorBeyond(cm, 30.) || !sensorBeyond(cm2, 30.)) {
    Serial.print("Read - ");
    Serial.println(cm);
    return 2;
  }
  return -1;
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
