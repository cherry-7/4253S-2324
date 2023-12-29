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
void on_center_button()
{
	static bool pressed = false;
	pressed = !pressed;
	if (pressed)
	{
		pros::lcd::set_text(2, "I was pressed!");
	}
	else
	{
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	endgame.set_value(false);
	// pnu2 left? pnu right
	pnu.set_value(true);
	pnu2.set_value(false);
	pros::lcd::initialize();
	pros::lcd::set_text(0, "Bot Initializing");

	driveLeftBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	driveLeftFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	driveRightBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	driveRightFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	linesensor.calibrate();

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

/**q1qq	`a1qa							a
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

void moveIntake(int delay, int direction)
{
	setIntake(direction * 127);
	pros::delay(delay);
	setIntake(0);
	pros::delay(250);
}

void moveShooter(int delay, int direction)
{
	setShooter(direction * 500);
	pros::delay(delay);
	setShooter(0);
	pros::delay(250);
}

void sameColorGoal()
{
	// release intake
	setShooter(-500);

	// turn & score matchload
	translate(750, 1);
	rotate(-45);

	moveIntake(500, -1);
	// moveIntake(1000, -1);
	translate(1200, 1);
	// rotate(180);
	translate(600, -1);
	translate(700, 1);
	// moveIntake(500, -1);
	translate(700, -1);
	setShooter(0);
	pros::delay(100);

	/* HANG
		translate(800, -1);
	pros::delay(100);
	rotate(25);
	translate(1500, -1);
	rotate(40);
	translate(1700, -1);
	*/
}

void moveForward()
{
	moveShooter(150, -1);

	setDrive(88, 88);
	while (linesensor.get_value() > 0)
	{
		if (linesensor.get_value() < 2910)
		{
			break;
		}
	}
	setDrive(88, 88);
}

void moveFor2()
{
	moveShooter(150, -1);

	setDrive(50, 50);
	pros::delay(800);
	setDrive(0, 0);

	moveIntake(200, -1);
}

void diffColorGoal()
{
	// release intake
	moveShooter(150, -1);

	// AWP
	pnu.set_value(false);
	translate(650, 1);
	pros::delay(100);

	// turn & score matchload
	rotate(45);
	pnu.set_value(true);
	translate(1200, 1);
	translate(600, -1);
	translate(700, 1);
	// moveIntake(500, -1);
	pros::delay(250);
	translate(700, -1);
	// middle triball
	/*
	rotate(110);
	translate(1200, 1);
	translate(600, -1);
	translate(800, 1);
	rotate(-90);
	pros::delay(100);
	translate(700, 1);
	moveIntake(800, 1);
	rotate(-90);
	translate(1200, 1);*/

	// hang
	/*
	rotate(-25);
	translate(800, -1); // change to 1200
	pros::delay(250);
	rotate(-20);
	translate(1800, -1); */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ///////////////////////////////////////////;
}

void diffColorMid()
{
	moveShooter(150, -1);
	// AWP
	pnu2.set_value(true);
	// turn & score matchload
	translate(600, 1);
	rotate(50);
	// moveIntake(1000, -1);
	translate(1200, 1);
	pros::delay(100);
	pnu2.set_value(false);

	rotate(-50); //
	translate(1700, -1);
	pros::delay(250);
	rotate(50);
	translate(1800, -1);
}

void skills()
{
	// shoots matchloads
	setShooter(500);
	/*
	setShooter(500);
	for(int i = 0; i < 44; i ++) {
		while(distanceSensor.get() > 0) {
			if (distanceSensor.get() < 50) {
				break;
			}
			pros::delay(100);
		}
		pros::delay(25);
	}
	setShooter(0);*/
	// moveShooter(42000, 1);

	/*
		rotate(45);
	translate(600, 1);
	rotate(-45);
	translate(1200, 1);
	pnu.set_value(false);
	pnu2.set_value(true);

	// score neutral triball & matchload
	translate(2400, 1);
	rotate(-50);
	translate(600, 1);
	rotate(-45);
	translate(1100, 1);
	translate(600, -1);
	rotate(90);

	// score random matchloads
	for(int i = 0; i < 2; i++) {
		translate (1800, -1);
		rotate(-90);
		pnu.set_value(false);
		pnu2.set_value(true);
		while (linesensor.get_value() < 2000) {
			setDrive(88, 88); // 70% of original speed
			pros::delay(100);
		}
		setDrive(0, 0);

		rotate(90);
		translate(1800, 1);
		translate(-600, 1);
		translate(700, 1);


		// NOT TESTED

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
	*/
}

void sameColor()
{
	moveShooter(150, -1);

	moveIntake(500, -1);
	rotate(-145);

	// middle
	translate(1200, 1);
	setIntake(127);
	translate(300, 1);
	translate(1400, -1);
	setIntake(0);

	rotate(35);

	// descore
	pnu.set_value(false);
	pnu2.set_value(true);
	translate(1100, 1);

	// score
	rotate(-40);
	setIntake(-127);
	/*setDrive(127, 127);
	pros::delay(1500);
	setDrive(0, 0);*/
	translate(1200, 1);
	setIntake(0);
	pnu2.set_value(false);
	pnu.set_value(true);

	// middle triballs
	translate(900, -1);
	rotate(-115);
	setIntake(127);
	translate(2500, 1);
	rotate(20);
	pnu.set_value(false);
	pnu2.set_value(true);
	translate(1200, 1);
	rotate(60);
	setIntake(-127);
	translate(1800, 1);
}

void diffColor()
{
	moveIntake(1000, -1);
	translate(200, 1);
	translate(1000, -1);
	rotate(135);
	translate(950, 1);
	rotate(-45);
	translate(1200, 1);
}

void autonomous()
{
	//sameColor();
	diffColor();
	// diffColorGoal();
	// sameColorGoal();
	// skills();
	// moveForward();
	// moveFor2();
}

void setPnu()
{

	if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))
	{
		pros::lcd::set_text(1, "PNU activated");
		pnu.set_value(false);
		pnu2.set_value(true);
		// pros::delay(10);
		// pnu.set_value(false);
	}
	if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
	{
		pros::lcd::set_text(2, "PNU deactivated");
		pnu.set_value(true);
		pnu2.set_value(false);
		// pros::delay(10);
		// pnu.set_value(false);
	}

	// shooter = 127;
	pros::delay(10);
}

void setOpening()
{

	if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
	{
		pros::lcd::set_text(1, "Shooter activated");
		moveShooter(150, -1);
	}

	// shooter = 127;
	pros::delay(10);
}

void setEndgame()
{

	if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
	{
		pros::lcd::set_text(1, "Endgame activated");
		endgame.set_value(true);
		// pros::delay(10);
		// pnu.set_value(false);
	}

	// shooter = 127;
	pros::delay(10);
}

void opcontrol()
{
	bool state = true;
	while (state)
	{

		std::cout << "line sensor: " << linesensor.get_value() << std::endl;

		pros::lcd::set_text(0, "Op Control Started");
		// std::cout << "HELLO" << std::endl;

		// some code to control drive
		setDriveMotors();
		pros::lcd::set_text(1, "Drive Motors Set");
		// setDrive(0, 127);

		// control intake
		setIntakeMotors();
		pros::lcd::set_text(1, "Intake Motors Set");
		// control rollers
		setShooterMotors();
		pros::lcd::set_text(1, "Shooter Motors Set");

		// control wall
		setPnu();

		// control endgame
		setEndgame();

		// open intake
		setOpening();

		pros::delay(10);
	}
}
