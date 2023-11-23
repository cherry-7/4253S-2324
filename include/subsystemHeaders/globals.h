#pragma once
#include "main.h"
#include "pros/adi.hpp"

// MOTORS
extern pros::Motor shooter;
extern pros::Motor intake;

extern pros::Motor driveLeftBack;
extern pros::Motor driveLeftBot;
extern pros::Motor driveLeftFront;

extern pros::Motor driveRightBack;
extern pros::Motor driveRightBot;
extern pros::Motor driveRightFront;

extern pros::Imu imu_sensor;
extern pros::ADIDigitalOut pnu;
extern pros::ADIDigitalOut pnu2;

extern pros::Controller controller;

extern std::shared_ptr<okapi::IterativePosPIDController> turnPID; 