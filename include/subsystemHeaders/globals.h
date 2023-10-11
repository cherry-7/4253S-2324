#pragma once
#include "main.h"
#include "pros/adi.hpp"

// MOTORS
extern pros::Motor shooter;
extern pros::Motor intake;
extern pros::Motor wall;

extern pros::Motor driveLeftBack;
extern pros::Motor driveLeftMid;
extern pros::Motor driveLeftFront;

extern pros::Motor driveRightBack;
extern pros::Motor driveRightMid;
extern pros::Motor driveRightFront;

extern pros::Imu imu_sensor;
extern pros::ADIDigitalOut pnu;

extern pros::Controller controller;
