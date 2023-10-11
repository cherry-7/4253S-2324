#include "main.h"
#include "pros/misc.h"
#include "pros/rtos.hpp"

// HELPER FUNCTION
void setWall(int power) {
    shooter = power;
}

// DRIVER CONTROL FUNCTIONS
void setWallMotors() {
    // digital = buttons
    // bottom trigger intakes, top trigger outtakes
   /*static bool flywheelState = false;
   if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
    flywheelState = !flywheelState;
   }
    */
   //int indexPower = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1) * 45;
   //setShooter(flywheelState * 127);

   if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
       shooter.move_absolute(350, 600);
       pros::delay(250);
       shooter.move_absolute(-350, 600);
   }
}