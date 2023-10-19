#include "main.h"
#include "pros/misc.h"
#include "pros/rtos.hpp"

// HELPER FUNCTION
void setShooter(int power) {
    shooter = power;
}

// DRIVER CONTROL FUNCTIONS
void setShooterMotors() {
    // digital = buttons
    // bottom trigger intakes, top trigger outtakes
   /*static bool flywheelState = false;
   if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
    flywheelState = !flywheelState;
   }

    // move absolute
   shooter.move_absolute(-350, 600);
       pros::delay(250);
       shooter.move_absolute(350, 600);
    */
   //int indexPower = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1) * 45;
   //setShooter(flywheelState * 127);
    static bool shooterState = false;
   if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
    shooterState = !shooterState;
   }
   //setShooter(flywheelState * 127);
   setShooter(500 * shooterState);
}