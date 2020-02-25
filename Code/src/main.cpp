         #include "main.h"
#include "okapi/api.hpp"

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
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
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

void cube_close(okapi::Motor intakeMotor)
{
  //Autonmous cube the intake closes
	intakeMotor.moveRelative(-195, 300);
	intakeMotor.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
}

void cube_open(okapi::Motor intakeMotor)
{
  // Autonmous cube intake open
	intakeMotor.moveRelative(195, 300);
	intakeMotor.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
}
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

 void arm_move(okapi::ChassisController * chassis, int distance, double velocity)
{
  // This moves the arm (DR4B) up and down depending on sign of distance
	chassis->setMaxVelocity(velocity);
	chassis->moveDistance(distance);
	chassis->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  pros::delay(100);
}

// forward declaration in order to make the program recognizae that autorun is a function
void autorun();
void autorunNight();
void test()
{ // Because we changed the chassisWidth to 12 this is to see what the turn angles will be
  using namespace okapi;
  using namespace okapi::literals;

  int frontLMotor = -5, frontRMotor = 6, backLMotor = 12, backRMotor = -13;
  auto wheelDiameter = 4_in, chassisWidth = 12.5_in; // Changed this to 12
  auto chassis = ChassisControllerFactory::create({frontLMotor, backLMotor},
   {frontRMotor, backRMotor}, okapi::AbstractMotor::gearset::green, {wheelDiameter, chassisWidth});

   chassis.setMaxVelocity(30);
   chassis.turnAngle(90_deg);

}

void autonomous()
{
	autorunNight();
}

void autorunNight()
/* HOW TO POSITION THE ROBOT: THE FRONT RIGHT WHEEL NEEDS TO BE ON THE SECOND
 * TILE TRAPEZOID EXTRUSION FROM THE VIEW OF FACING THE GOAL.
 */
{  //___int_least16_t_defined Named this night because we are doing it the night before a competition
    /* Setting which alliance side we are on
    v*  1 = red side (border to left of robot)
     * -1 = blue side (border to right of robot)
     */
    int allianceSide = 1;

   using namespace okapi;
   using namespace okapi::literals;

   Motor intakeMotor(7);
   intakeMotor.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);

   int frontLMotor = -5, frontRMotor = 6, backLMotor = 12, backRMotor = -13;
   auto wheelDiameter = 4_in, chassisWidth = 12_in; // Changed this to 12
   auto chassis = ChassisControllerFactory::create({frontLMotor, backLMotor},
    {frontRMotor, backRMotor}, okapi::AbstractMotor::gearset::green, {wheelDiameter, chassisWidth});

    int topRight= -20, topLeft = 11, bottomRight = 18, bottomLeft = -19;
    auto armChassis = ChassisControllerFactory::create({topRight, bottomRight},
      {topLeft, bottomLeft}, okapi::AbstractMotor::gearset::green);

    // Flips claw out of robot
    chassis.moveDistance(5_in);

    chassis.setMaxVelocity(50); // This keeps the robot from going crazy fast
    arm_move(&armChassis, 65, 30); // We used the same values as old auton


    // ~NEW AUTONMOUS~
    // We had to do this because it conflicted with what the bigRobot was doing.

    // Moves to goal
    chassis.moveDistance(10.5_in);

    // Opens cube over goal
    cube_open(intakeMotor);
    pros::delay(300);

    // Turning to face the front of the four cube tower
    if (allianceSide == 1)
    {   // Red
      chassis.moveDistance(-22_in);
    }
    else if (allianceSide == -1)
    {   // Blue
      chassis.moveDistance(-25_in);
    }
    cube_close(intakeMotor);
    pros::delay(300);
    if (allianceSide == 1)
    {   // Red
      chassis.turnAngle(75_deg);
    }
    else if (allianceSide == -1)
    {   // Blue
      chassis.turnAngle(-70_deg);
    }

    // Knocks cube out of the way
    chassis.moveDistance(12_in);
    if (allianceSide == 1)
    {   // Red
      chassis.turnAngle(90_deg);
    }
    else if (allianceSide == -1)
    {   // Blue
      chassis.turnAngle(-90_deg);
    }

    if (allianceSide == 1)
    {   // Red
      chassis.turnAngle(-60_deg);
    }
    else if (allianceSide == -1)
    {   // Blue
      chassis.turnAngle(58_deg);
    }
    cube_open(intakeMotor);

    // Moves towards four cube tower
    chassis.moveDistance(5_in);

    // cube_close(intakeMotor);
    // pros::delay(300);
    //
    // // Moving back with whatever four tower cubes we grabbed to face goal
    // chassis.moveDistance(-32_in);
    // chassis.turnAngle(-90_deg);
    //
    // // Moves towards goal raising arm to put cubes grabbed from tower
    // // on top of the first cube placed in the goal
    // arm_move(&armChassis, 150, 70);
    // chassis.moveDistance(17_in);
    // cube_open(intakeMotor);


}

 // void autorun()
 // {
 //
	//  // setting which alliance side we are on
	//  // 1 = blue side (border to left of robot)
	//  // -1 = red side (border to right of robot)
	//  int allianceSide = 1;
 //
 // 	using namespace okapi;
	// using namespace okapi::literals;
 //
 //  Motor intakeMotor(7);
 //  intakeMotor.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
 //
 // 	int frontLMotor = -5, frontRMotor = 6, backLMotor = 12, backRMotor = -13;
 // 	auto wheelDiameter = 4_in, chassisWidth = 12.5_in;
 // 	auto chassis = ChassisControllerFactory::create({frontLMotor, backLMotor},
 // 	 {frontRMotor, backRMotor}, okapi::AbstractMotor::gearset::green, {wheelDiameter, chassisWidth});
 //
	//  int topRight= -20, topLeft = 11, bottomRight = 18, bottomLeft = -19;
 // 	 auto armChassis = ChassisControllerFactory::create({topRight, bottomRight},
 //   	 {topLeft, bottomLeft}, okapi::AbstractMotor::gearset::green);
 //
	// //  // Starting on right edge of the field)
 //  chassis.setMaxVelocity(30);
	// chassis.turnAngle(allianceSide * 20_deg);
 //  chassis.setMaxVelocity(50);
	// chassis.moveDistance(12_in); // move toward tower
 //  chassis.moveDistance(-5_in);
 //  arm_move(&armChassis, 390, 70);
 //  chassis.setMaxVelocity(60);
 //  chassis.turnAngle(allianceSide * 20_deg);
 //  chassis.setMaxVelocity(50);
 //  chassis.moveDistance(5_in);
 //  cube_open(intakeMotor);//place block in tower
 //  pros::delay(700);
	// //Move away from tower
	// chassis.moveDistance(-6_in);
 //  cube_close(intakeMotor);
 //  pros::delay(100);
 //  chassis.setMaxVelocity(30);
 //  chassis.turnAngle(allianceSide * -47_deg);
 //  chassis.setMaxVelocity(50);
	// arm_move(&armChassis, -390, 30);
 //
 //  // Moving the robot in position to grab block
	// chassis.moveDistance(-32_in); // Moves backwards
 //  chassis.setMaxVelocity(30);
	// chassis.turnAngle(allianceSide * 90_deg);
 //  chassis.setMaxVelocity(50);
	// chassis.moveDistance(16_in);
 //  chassis.setMaxVelocity(30);
	// chassis.turnAngle(allianceSide * -73_deg);
 //  chassis.setMaxVelocity(50);
	// chassis.moveDistance(-7_in); // possible area that might be effected by side
 //  chassis.setMaxVelocity(30);
	// chassis.turnAngle(allianceSide * -13_deg);
 //  chassis.setMaxVelocity(50);
 //  arm_move(&armChassis, 55, 30);
 //  pros::delay(200);
 //  cube_open(intakeMotor);
	// chassis.moveDistance(16_in);
 //  cube_close(intakeMotor);//grab block
 //  pros::delay(500);
 //  arm_move(&armChassis, 60, 50);
 //  chassis.moveDistance(-6_in);
 //  chassis.turnAngle(allianceSide * -160_deg);
 //
 //
 //  // chassis.setMaxVelocity(30);
	// // chassis.turnAngle(allianceSide * 40_deg);
 //  // chassis.setMaxVelocity(50);
	// // chassis.moveDistance(25_in);
 //  // chassis.setMaxVelocity(30);
 //  // if (allianceSide == 1) {
 //  //   chassis.turnAngle(-40_deg);
 //  // }
 //  // else if (allianceSide == -1) {
 //  //   chassis.turnAngle(46_deg);
 //  // }
 //  // chassis.setMaxVelocity(50);
	// // // chassis.moveDistance(30_in);
 //  // // chassis.setMaxVelocity(30);
 //  // // chassis.turnAngle(allianceSide * -40_deg);
 //  // // chassis.setMaxVelocity(50);
 //  // // chassis.moveDistance(10_in);
 //  // // chassis.setMaxVelocity(30);
 //  // // chassis.turnAngle(allianceSide * 38_deg);
 //  // // chassis.setMaxVelocity(50);
 //  // // chassis.moveDistance(36_in);
 //  // if (allianceSide == 1) {
 //  //   chassis.moveDistance(68.5_in);
 //  // }
 //  // else if (allianceSide == -1) {
 //  //   chassis.moveDistance(69_in);
 //  // }
 //  // cube_open(intakeMotor);
 //  // pros::delay(300);
 //  // chassis.moveDistance(-6_in);
 // }

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
void opcontrol()
{
	using namespace pros;
	Controller master(E_CONTROLLER_MASTER);
  okapi::Motor leftFrontMotor(5)/*inverted*/, rightFrontMotor(6), leftBackMotor(12), rightBackMotor(13)/*inverted*/;
	okapi::Motor intakeMotor(7);
  okapi::Motor topRight(20)/*inverted*/, topLeft(11), bottomRight(18), bottomLeft(19)/*inverted*/;
	leftFrontMotor.setReversed(true);
	rightBackMotor.setReversed(true);
	topRight.setReversed(true);
	bottomLeft.setReversed(true);

	// for PID
	okapi::IntegratedEncoder topRightEncoder(topRight); topRightEncoder.reset();
	okapi::IntegratedEncoder topLeftEncoder(topLeft); topLeftEncoder.reset();
	okapi::IntegratedEncoder bottomRightEncoder(bottomRight); bottomRightEncoder.reset();
	okapi::IntegratedEncoder bottomLeftEncoder(bottomLeft); bottomLeftEncoder.reset();

	int pidThreshold = 8;

	// for Arm
	int armUpLimit = 70;

  // controlling driver sensitivity
  int lowerSensitivity = 20;

	//autorunNight(); // make sure to get rid of this before competition
  // test();

	while (true)
		{
			 //Getting controller stick input
			int x = (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X));
			int y = (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));
      // getting digital buttons for the cube intake
			bool r1 = master.get_digital(E_CONTROLLER_DIGITAL_R1);
			bool r2 = master.get_digital(E_CONTROLLER_DIGITAL_R2);
			int threshold = 15;
      /** Makes sure that controller doesn't read every single movement and
      * won't be too sensitive
      */

      // make sure small movement in analog stick doesn't effect too much
      if (x < threshold && x > -threshold) {
        x = 0;
      }
      if (y < threshold && y > -threshold) {
        y = 0;
      }

      // creating both left and right velocities based on control inputs
			int leftVelocity = y + x, rightVelocity = y - x;

      if (leftVelocity < lowerSensitivity && leftVelocity > -lowerSensitivity) {
        leftVelocity /= 2;
      }

      if (rightVelocity < lowerSensitivity && rightVelocity > -lowerSensitivity) {
        rightVelocity /= 2;
      }

			//Controlling what the left wheels do
			if(leftVelocity != 0)
			{
				leftFrontMotor.moveVelocity(leftVelocity);
				leftBackMotor.moveVelocity(leftVelocity);
			}
			else
			{
				leftFrontMotor.moveVelocity(0);
				leftBackMotor.moveVelocity(0);
			}

			//Controlling what the right wheels do
			if(rightVelocity != 0)
			{
				rightFrontMotor.moveVelocity(rightVelocity);
				rightBackMotor.moveVelocity(rightVelocity);
			}
			else
			{
				rightFrontMotor.moveVelocity(0);
				rightBackMotor.moveVelocity(0);
			}

			// Moves intake based upon button inputs
			if (r1)
			{
        // Opens intake
				intakeMotor.moveVelocity(300);
			}
			else if (r2)
			{
        // Closes intake
				intakeMotor.moveVelocity(-300);
			}
			else
			{
        // Doesn't move if no buttons pushed
				intakeMotor.moveVelocity(0);
				intakeMotor.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
			}


			//ARM MOVEMENT
			int arm = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
			bool armTurboBtn = master.get_digital(E_CONTROLLER_DIGITAL_B);
			int thresholdUP = 15;
			int thresholdDOWN = -15;

			// if((topRightEncoder.get() + topLeftEncoder.get() + bottomRightEncoder.get() + bottomLeftEncoder.get())/ 4 <= 0 && arm < 0)
      // {
      //   // The limit so that the arm does not go below what is possible for the robot
			// 	arm = 0;
			// }

			// if ((topRightEncoder.get() + topLeftEncoder.get() + bottomRightEncoder.get() + bottomLeftEncoder.get())/4 >= 370 && arm > 0) {
      //  The limit so that the arm does not go above what is possible for the robot
			// 	arm = 0;
			// }

      //trying to help synchronization
      double pidErrorTopLeft = topRightEncoder.get() - topLeftEncoder.get();
      double pidErrorBottomRight = topRightEncoder.get() - bottomRightEncoder.get();
      double pidErrorBottomLeft = topRightEncoder.get() - bottomLeftEncoder.get();

			if(pidErrorTopLeft >= pidThreshold || pidErrorTopLeft <= -1 * pidThreshold){
				topLeft.moveRelative(pidErrorTopLeft, armUpLimit);
			}

			if(pidErrorBottomRight >= pidThreshold || pidErrorBottomRight <= -1 * pidThreshold){
				bottomRight.moveRelative(pidErrorBottomRight, armUpLimit);
			}

		 	if (pidErrorBottomLeft >= pidThreshold || pidErrorBottomLeft <= -1 * pidThreshold) {
				bottomLeft.moveRelative(pidErrorBottomLeft, armUpLimit);
			}

			if(arm > thresholdUP || arm < thresholdDOWN) //up and down
			{
				if (arm < armUpLimit && arm > -10)
        {
          // joystick values within limits
					topRight.moveVelocity(arm);
					topLeft.moveVelocity(arm);
					bottomRight.moveVelocity(arm);
					bottomLeft.moveVelocity(arm);
				}
				else if (arm >= armUpLimit)
        {
          // upper edge case
					topRight.moveVelocity(armUpLimit);
					topLeft.moveVelocity(armUpLimit);
					bottomRight.moveVelocity(armUpLimit);
					bottomLeft.moveVelocity(armUpLimit);
				}
				else if (arm <= -10)
        {
          // lower edge case
					topRight.moveVelocity(-30);
					topLeft.moveVelocity(-30);
					bottomRight.moveVelocity(-30);
					bottomLeft.moveVelocity(-30);
				}
        // Setting the brake mode to hold
				topRight.set_brake_mode(MOTOR_BRAKE_HOLD);
				bottomRight.set_brake_mode(MOTOR_BRAKE_HOLD);
				topLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
				bottomLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
			}
			else
			{
        //hold position
				topRight.moveVelocity(0);
				topLeft.moveVelocity(0);
				bottomRight.moveVelocity(0);
				bottomLeft.moveVelocity(0);
			}

      // Here so we can check temperature of motors for four bar
			double topR = topRight.get_temperature();
			double topL = topLeft.get_temperature();
			double bottomR = bottomRight.get_temperature();
			double bottomL = bottomLeft.get_temperature();

			// Here so we can check temperature of drive base
			double frontL = leftFrontMotor.getTemperature();
			double frontR = rightFrontMotor.getTemperature();
			double backL = leftBackMotor.getTemperature();
			double backR = rightBackMotor.getTemperature();

			//PRINT TEMPERATURE!
			lcd::print(0, "Tempeture topRight: %f", topR);
			lcd::print(1, "Tempeture topLeft: %f", topL);
			lcd::print(2, "Tempeture bottomRight: %f", bottomR);
			lcd::print(3, "Tempeture bottomLeft: %f", bottomL);

			lcd::print(4, "Temperature front left: %f", frontL);
			lcd::print(5, "Tempeture front right: %f", frontR);
			lcd::print(6, "Tempeture back left: %f", backL);
			lcd::print(7, "Tempeture back right: %f", backR);

      // lcd::print(4, "Difference Top: %d", pidErrorTop);
      // lcd::print(5, "Difference Bottom: %d", pidErrorBottom);
      // lcd::print(6, "Difference Left: %d", pidErrorLeft);
      // lcd::print(7, "Difference Right: %d", pidErrorRight);

			// lcd::print(4, "Top right encoder: %f", topRightEncoder.get());
			// lcd::print(5, "Top left encoder: %f", topLeftEncoder.get());
			// lcd::print(6, "Bottom right encoder: %f", bottomRightEncoder.get());
			// lcd::print(7, "Bottom left encoder: %f", bottomLeftEncoder.get());

			delay(20);
		}
}
