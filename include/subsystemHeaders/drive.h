#pragma once
#include "main.h"

// HELPER FUNCTIONS
void setDrive(int left, int right);

void resetDriveEncoders();

double avgDriveEncodervalue();

//DRIVER CONTROL FUNCTIONS
void setDriveMotors();

//AUTONOMOUS FUNCtIONS
void translate(int units, int direction);
// voltage can go higher
// moving a set of points a set of distance from where it was

// two main functions are translate and turn

void resetGyro();

void rotate(double degrees);