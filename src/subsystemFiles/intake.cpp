#include "main.h"

// HELPER FUNCTION
void setIntake(int power) {
    intake = power;
}

// DRIVER CONTROL FUNCTIONS
void setIntakeMotors() {
    // digital = buttons
    // bottom trigger intakes, top trigger outtakes

   int intakePower = 350 * (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) - controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2));
   setIntake(intakePower);
}