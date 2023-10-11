#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.h"
#include "pros/imu.hpp"
#include "pros/rtos.hpp"

//pros::ADIGyro gyro('B', 0.91); // 0.91 change later

// HELPER FUNCTION
void setDrive(int left, int right) {
    driveLeftBack = left;
    driveLeftMid = left;
    driveLeftFront = left;
    driveRightBack = right;
    driveRightMid = right;
    driveRightFront = right;
}

void resetDriveEncoders() {
    driveLeftBack.tare_position();
    driveLeftMid.tare_position();
    driveLeftFront.tare_position();
    driveRightBack.tare_position();
    driveRightMid.tare_position();
    driveRightFront.tare_position();
}

double avgDriveEncodervalue() {
    return (fabs(driveLeftFront.get_position()) +
        fabs(driveLeftMid.get_position()) +
        fabs(driveLeftBack.get_position()) +
        fabs(driveRightFront.get_position()) + 
        fabs(driveRightMid.get_position()) + 
        fabs(driveRightBack.get_position())) / 6;
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

    setDrive(leftJoystick, rightJoystick);

    
}

// AUTONOMOUS FUNCTIONS
void translate(int units, int voltage) {
    // define a direction based on units provided
    int direction = abs(units) / units; // either 1 or -1

    // reset motor encoders
    resetDriveEncoders();
    // drive forward until untits are reached
    // fabs covers doubles
    while(avgDriveEncodervalue() < abs(units)) {
        setDrive(voltage * direction, voltage * direction);
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