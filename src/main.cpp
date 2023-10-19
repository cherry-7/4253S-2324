#include "main.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"

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
	pnu.set_value(false);
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

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

// backwards = negative units
// speed is positive bc no direction

void moveIndexer(int loops, int delay) {
	int i { loops };

	for(i; i> 0; i--) {
		shooter.move_absolute(350, 600);
		pros::delay(250);
		shooter.move_absolute(-350, 600);

		pros::delay(delay);
	}
}

void moveIntake(int delay) {
	setIntake(127);
	pros::delay(delay);
	setIntake(-127);
	pros::delay(250);
}

bool enableDrivePID = true;

int drivePID(int setpoint) {

	while(enableDrivePID) {
		// constants
		// tune in PDI order
		double kP = 0;
		double kI = 0;
		double kD = 0;

		// Proportional (small current errors)
		double error = setpoint - avgDriveEncodervalue();
		//pros::lcd::set_text(1, "avg Encode:" + avgDriveEncodervalue());
		// Integral (past errors)
		// kI << kP
		double integral = integral + error;
		if ((error == 0) || (error > setpoint))
			integral = 0;
		
		if (error > 50)
			integral = 0;
		
		// Deritative (future errors, rate of change)
		// direction opposite to current direction of travel
		double prevError = error;
		double derivative = error - prevError;
		
		double power = error*kP + integral*kI + derivative*kD;

		pros::delay(15); // dT
		return (int)(power);
	}

	return 0;
}

void autonomous() {
	//pros::Task test(drivePID(457));
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void setPnu() {
	
	if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
		pros::lcd::set_text(2, "PNU activated");
		pnu.set_value(true);
		//pros::delay(10);
		//pnu.set_value(false);
	}
	if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
		pros::lcd::set_text(3, "PNU deactivated");
		pnu.set_value(false);
		//pros::delay(10);
		//pnu.set_value(false);
	}

	//shooter = 127;
	pros::delay(10);
}

void opcontrol() {
	pros::lcd::set_text(2, "Op Control Started");
	//std::cout << "HELLO" << std::endl;

	// some code to control drive
	setDriveMotors();
	// control intake
	setIntakeMotors();
	// control rollers
	setShooterMotors();

	// control wall
	setPnu();

	pros::delay(10);
}
