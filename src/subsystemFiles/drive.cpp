#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.h"
#include "pros/imu.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "subsystemHeaders/globals.h"
#include <algorithm>
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

bool arcDrive = true;

void arcadeDrive(int drive, int rotate) {
   int maximum = fmax(abs(drive), abs(rotate));

   int total = drive + rotate;
   int difference = drive - rotate;

    // left side

   if (drive >= 0) {
        if (rotate >= 0) {
            setDrive(maximum, difference);
        }
        else {
            setDrive(total, maximum);
        }
   } else {
        if(rotate >= 0) {
             setDrive(total, -maximum);
        }
        else {
            driveLeftBot = -maximum;
            driveLeftFront = -maximum;
            driveLeftBack = -maximum;

            driveRightBot = -difference;
            driveRightBack = -difference;
            driveRightFront = -difference;
        }
   }

}


// DRIVER CONTROL FUNCTIONS
void setDriveMotors() {
    static int toggleDrive = 0;
    // PRESS B
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

    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        if(toggleDrive == 0) {
            toggleDrive = 1;
            arcDrive = false;
        } else if(toggleDrive == 1) {
            toggleDrive = 0;
            arcDrive = true;
        }
    }
    
    if(arcDrive == true) 
        arcadeDrive(rightJoystick, 0.8 * leftJoystick);
    else 
        setDrive(leftJoystick, rightJoystick);
    pros::lcd::set_text(3, "Left: " + std::to_string(leftJoystick) + "Right: " + std::to_string(rightJoystick));
    
}



// AUTONOMOUS FUNCTIONS
int drivePID(int setpoint) {
    bool enableDrivePID = true;

    // PID constants
    double drivekP = 0.2345;
    double drivekI = 0.3;
    double drivekD = 0.374;

	while(enableDrivePID) {
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
		
		double power = error*drivekP + integral*drivekI + derivative*drivekD;

		pros::delay(15); // dT
		return (int)(power);
	}

	return 0;
}

void translate(int units, int direction) {
    // reset motor encoders
    resetDriveEncoders();

    // drive forward until untits are reached
    // fabs covers doubles
    while(avgDriveEncodervalue() < abs(units)) {
        setDrive(drivePID(units) * direction,
         drivePID(units) * direction);
        pros::delay(10);
    }
    
    // brief breaks
    setDrive(-10 * direction, -10 * direction);
    pros::delay(50);
    
    // set drive back to neutral
    setDrive(0, 0);
    //pros::delay(100);
}


void rotate(double degrees) {

    resetGyro();
    turnPID->reset();
    turnPID->setTarget(0);

    do {
        double power = turnPID->step(degrees - imu_sensor.get_yaw());
        setDrive(-power * 127, power * 127);
        pros::delay(10);
    } while(!turnPID->isSettled());

    setDrive(0, 0);
}

// OLD TURN FUNCTIONS
/*

bool enableRotatePID = true;

double rotatekP = 1;
double rotatekI = 0;
double rotatekD = 0;

int rotatePID(double setpoint) {

	while(enableRotatePID) {
		// constants
		// tune in PDI order
		

		// Proportional (small current errors)
		double error = setpoint - pros::c::imu_get_heading(9);
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
		
		double power = error*rotatekP + integral*rotatekI + derivative*rotatekD;

		pros::delay(15); // dT
		return (int)(power);
	}

	return 0;
}

void rotate(double degrees, int direction) {
    
    // degrees = degrees*2;
    // reset motor encoders
    
    while(fabs(imu_sensor.get_heading()) < fabs(degrees)) {
        pros::lcd::set_text(1, std::to_string(fabs(pros::c::imu_get_heading(9))));
        setDrive(-rotatePID(degrees) * direction, rotatePID(degrees) * direction);
        pros::delay(10);
    }
    // turn until units are reached
    pros::delay(100);

    // set drive back to neutral
    setDrive(0, 0);
    //pros::delay(100);
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
}*/

/*

    // degrees = degrees*2;
    // reset motor encoders
    resetGyro();
    while(fabs(pros::c::imu_get_heading(9)) < fabs(degrees)) {
        pros::lcd::set_text(1, std::to_string(fabs(pros::c::imu_get_heading(9))));
        setDrive(-rotatePID(degrees) * direction, rotatePID(degrees) * direction);
        pros::delay(10);
    }

    // turn until units are reached
    pros::delay(100);

    // set drive back to neutral
    setDrive(0, 0);
    //pros::delay(100);*/