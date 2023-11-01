#include "main.h"
#include <string>

// HELPER FUNCTION
void setIntake(int power) {
    intake = power;
}

int toggle = 0;
int multiplier = 0;
// DRIVER CONTROL FUNCTIONS
void setIntakeMotors() {
    // digital = buttons
    // bottom trigger intakes, top trigger outtakes
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
        if(toggle == 0) {
            toggle = 1;
            multiplier = 1;
        } else if(toggle == 1) {
            toggle = 2;
            multiplier = -1;
        }
        else if(toggle == 2) {
            toggle = 0;
            multiplier = 0;
        }
    }
    
   int intakePower = 500 * (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) - controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2));
   setIntake(intakePower);

    //pros::lcd::set_text(2, "Toggle: " + std::to_string(toggle));
    //pros::lcd::set_text(3, "Multi: " + std::to_string(multiplier));
}