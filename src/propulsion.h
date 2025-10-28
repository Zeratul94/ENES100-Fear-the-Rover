#ifndef MY_FUNCTIONS_H // Include guards to prevent multiple inclusions
#define MY_FUNCTIONS_H

//distance in cm
void move_forward(double distance);

//distance in cm
void move_backward(double distance);

//distance in cm
void move_right(double distance);

//distance in cm
void move_left(double distance);

void spin_CCW(double radians);

void spin_CW(double radians);

void set_servo(double angle);

double read_servo(double angle);

#endif