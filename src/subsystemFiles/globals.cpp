#include "main.h"
#include "pros/adi.hpp"

// MOTORS
pros::Motor shooter(1, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor intake(2, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor driveLeftBack(3, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor driveLeftBot(4, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor driveLeftFront(11, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor driveRightBack(6, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor driveRightBot(7, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor driveRightFront(8, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Imu imu_sensor(9);
pros::ADIDigitalOut pnu('A');
pros::ADIDigitalOut pnu2('B');

// positive voltage = forward/false
// backwards = true

// CONTROLLER
pros::Controller controller(pros::E_CONTROLLER_MASTER);

std::shared_ptr<okapi::IterativePosPIDController> turnPID = std::make_shared<okapi::IterativePosPIDController>(0.1, 0.0, 0.0, 0.0, okapi::TimeUtilFactory::withSettledUtilParams(2, 2, okapi::millisecond*100));