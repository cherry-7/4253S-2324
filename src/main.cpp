#include "main.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "Graphy/Grapher.hpp"
#include <string>

/**	
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pnu.set_value(true);
	pnu2.set_value(false);
	pros::lcd::initialize();
	pros::lcd::set_text(0, "Bot Initializing");

	driveLeftBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	driveLeftFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	driveRightBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	driveRightFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

	shooter.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	pros::lcd::set_text(1, "4253S Bot Started! :D");
	pros::delay(2000);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

// backwards = negative units
// speed is positive bc no direction

void moveIntake(int delay, int direction) {
	setIntake(direction * 127);
	pros::delay(delay);
	setIntake(0);
	pros::delay(250);
}

void moveShooter(int delay, int direction) {
	setShooter(direction * 500);
	pros::delay(delay);
	setIntake(0);
	pros::delay(250);
}

void sameColorGoal() {
	// release intake
	moveShooter(150, -1);

	// turn & score matchload
	rotate(45);
	translate(1600, 1);

	moveIntake(800, -1);
	translate(1600, -1);
	pros::delay(250);

	// grab netural triball
	rotate(-100);
	pros::delay(250);
	translate(1800, 1);
	moveIntake(1000, 1);
	pros::delay(250);

	// score triball
	translate(1600, -1); 
	pros::delay(100);
	rotate(45);
	translate(1600, 1);
	moveIntake(800, -1);
	translate(1700, -1);

	rotate(100);
	translate(1800, -1);

} 

void diffColorGoal() {
	// release intake
	moveShooter(150, -1);

	// AWP 
	pnu.set_value(false);
	
	// turn & score matchload
	translate(200, 1);
	rotate(45);
	pros::delay(250);
	translate(1600, 1);
	moveIntake(800, -1);
	pnu.set_value(true);
	translate(1600, -1); // change to 1200 
	pros::delay(250);

	// hang 
	rotate(-50);
	translate(2400, -1);
}

void diffColorMid() {
	moveShooter(150, -1);
	// AWP 
	pnu2.set_value(true);
	// turn & score matchload
	rotate(45);
	translate(1600, 1);
	pnu2.set_value(false);
	moveIntake(800, -1);
	translate(1600, -1);
	pros::delay(250);
	rotate(50);
	translate(1800, -1);
}

void skills() {
	// shoots matchloads
	moveShooter(44000, 1);

	// get neutral triball
	rotate(90);
	translate(600, 1);
	rotate(-45);
	pnu.set_value(false);
	pnu2.set_value(true);
	translate(1200, 1);

	// score neutral triball & matchload
	translate(1200, 1);
	rotate(-50);
	translate(1600, 1);
	pnu.set_value(true);
	pnu2.set_value(false);

	// score random matchloads 
	for(int i = 0; i < 2; i++) {
		translate (1500, -1);
		rotate(-45);
		pnu.set_value(false);
		pnu2.set_value(true);
		translate(600, 1);
		rotate(-45);
		translate(600, 1);
		rotate(45);
		translate(600, 1);
		rotate(90);
		translate(1600, 1);
		pnu.set_value(true);
		pnu2.set_value(false);
	}

	// last remaining random matchloads
	rotate(-90);
	pnu.set_value(false);
	pnu2.set_value(true);
	translate(600, 1);
	rotate(90);
	translate(1200, 1);
	rotate(45);
	translate(900, 1);
	translate(900, -1);
	rotate(45);
	translate(900, 1);
	translate(900, -1);
	
	// close wings
	pnu.set_value(false);
	pnu2.set_value(true);

	// hang
	rotate(-45);
	translate(1200, -1);
	rotate(-45);
	translate(1200, -1);
}


void autonomous() {
	//sameColorGoal();
	diffColorGoal();
	//skills();
}

void setPnu() {
	
	if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
		pros::lcd::set_text(1, "PNU activated");
		pnu.set_value(false);
		pnu2.set_value(true);
		//pros::delay(10);
		//pnu.set_value(false);
	}
	if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
		pros::lcd::set_text(2, "PNU deactivated");
		pnu.set_value(true);
		pnu2.set_value(false);
		//pros::delay(10);
		//pnu.set_value(false);
	}

	//shooter = 127;
	pros::delay(10);
}

void opcontrol() {
	bool state = true;
	while(state) {
		
		std::cout << "robot heading: " << imu_sensor.get_heading() << std::endl;

		pros::lcd::set_text(0, "Op Control Started");
		//std::cout << "HELLO" << std::endl;

		

		// some code to control drive
		setDriveMotors();
		pros::lcd::set_text(1, "Drive Motors Set");
		//setDrive(0, 127);
		
		// control intake
		setIntakeMotors();
		pros::lcd::set_text(1, "Intake Motors Set");
		// control rollers
		setShooterMotors();
		pros::lcd::set_text(1, "Shooter Motors Set");

		// control wall
		setPnu();
		

		pros::delay(10);
	}
}
