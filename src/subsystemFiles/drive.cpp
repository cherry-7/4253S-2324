#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.h"
#include "pros/imu.hpp"
#include "pros/rtos.hpp"
#include "subsystemHeaders/globals.h"
#include <string>

//pros::ADIGyro gyro('B', 0.91); // 0.91 change later

// HELPER FUNCTION
void setDrive(int left, int right) {
    driveLeftBot = left;
    driveLeftFront = left;
    driveLeftBack = left;
    driveRightBot = right;
    driveRightBack = right;
    driveRightFront = right;
}

void resetDriveEncoders() {
    driveLeftBot.tare_position();
    driveLeftFront.tare_position();
    driveLeftBack.tare_position();
    driveRightBot.tare_position();
    driveRightFront.tare_position();
    driveRightBack.tare_position();
}

double avgDriveEncodervalue() {
    return (fabs(driveLeftBack.get_position()) +
        fabs(driveLeftFront.get_position()) +
        fabs(driveLeftBot.get_position()) +
        fabs(driveRightBack.get_position()) + 
        fabs(driveRightFront.get_position()) + 
        fabs(driveRightBot.get_position())) / 6;
}

void resetGyro() {
    imu_sensor.tare_heading();
}

// DRIVER CONTROL FUNCTIONS
void setDriveMotors() {
    int leftJoystick = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightJoystick = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    // joysticks can range from -127 to +127
    // horizontal = left, vertical = right
    // joystick = get analog 
    // specific button = get digital

    // set differently depending on controller error
    // this is called DEAD ZONE
    if(abs(leftJoystick) < 10)
        leftJoystick = 0;

    if(abs(rightJoystick) < 10) 
        rightJoystick = 0;

    setDrive(leftJoystick * 0.75, rightJoystick * 0.75);
    pros::lcd::set_text(3, "Left: " + std::to_string(leftJoystick) + "Right: " + std::to_string(rightJoystick));
    
}

bool enableDrivePID = true;

double kP = 0;
double kI = 0;
double kD = 0;

// AUTONOMOUS FUNCTIONS
int drivePID(int setpoint) {

	while(enableDrivePID) {
		// constants
		// tune in PDI order
		

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

void translate(int units) {
    // define a direction based on units provided
    int direction = abs(units) / units; // either 1 or -1

    // reset motor encoders
    resetDriveEncoders();
    // drive forward until untits are reached
    // fabs covers doubles
    while(avgDriveEncodervalue() < abs(units)) {
        setDrive(drivePID(units) * direction, drivePID(units) * direction);
        pros::delay(10);
    }
    
    // brief breaks
    setDrive(-10 * direction, -10 * direction);
    pros::delay(50);
    
    // set drive back to neutral
    setDrive(0, 0);
}

void turn (int angle, int voltage) {
    int direction = abs(angle) / angle; // either 1 or -1
    // reset motor encoders
    resetGyro();
    // drive forward until untits are reached
    // fabs covers doubles
    setDrive(-voltage * direction, voltage * direction);
    while(fabs(pros::c::imu_get_heading(9)) < abs(angle * 10) - 50) {
        std::cout << pros::c::imu_get_heading(9) << std::endl;
        pros::delay(10);
    }

    //letting robot lose momentum
    pros::delay(100);
    // correcting overshoot/undershoot
    if(fabs(pros::c::imu_get_heading(9)) > abs(angle * 10) ) {
        setDrive(0.5 * voltage * direction, 0.5 * -voltage * direction);
        while(fabs(pros::c::imu_get_heading(9)) < abs(angle * 10)) {
            pros::delay(10);
        }
    } else if (fabs(pros::c::imu_get_heading(9)) < abs(angle * 10)) {
        setDrive(0.5 * -voltage * direction, 0.5 *voltage * direction);
        while(fabs(pros::c::imu_get_heading(9)) < abs(angle * 10)) {
            pros::delay(10);
        }
    }
    // set drive back to neutral
    setDrive(0, 0);
}