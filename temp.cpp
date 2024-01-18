#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START IQ MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END IQ MACROS


// Robot configuration code.
motor MotorGroup1MotorA = motor(PORT1, false);
motor MotorGroup1MotorB = motor(PORT12, true);
motor_group MotorGroup1 = motor_group(MotorGroup1MotorA, MotorGroup1MotorB);

motor MotorGroup7MotorA = motor(PORT7, true);
motor MotorGroup7MotorB = motor(PORT6, false);
motor_group MotorGroup7 = motor_group(MotorGroup7MotorA, MotorGroup7MotorB);

#pragma endregion VEXcode Generated Robot Configuration



/* ---------------- ROBOT MOVEMENT CONTROL ---------------- */
double cmPerSecond = 0;
double RPM = 0;
double accel = 0;
double accelDistance = 10.0;
double accelTime = 1.0;

double quarterTurn = 16;

void robotTurn(double times) {
  double spinTime = quarterTurn * times / cmPerSecond;
  wait(spinTime * 1000, msec);
  MotorGroup1.stop();
  MotorGroup7.stop();
}

void slowDown() {
  MotorGroup7.spin(forward);
  // double tempVelocity = RPM;
  for (double tempVelocity = 50; tempVelocity > 0; tempVelocity -= accel) {
    MotorGroup1MotorA.setVelocity(tempVelocity, rpm);
    MotorGroup1MotorB.setVelocity(tempVelocity, rpm);
    MotorGroup7MotorA.setVelocity(tempVelocity, rpm);
    MotorGroup7MotorB.setVelocity(tempVelocity, rpm);
    wait(20, msec);
  }
}

/* ---------------- MOVEMENT TRACKING ---------------- */
struct Movement {
  int movement;
  double units;
};

// saved variables
Movement movements[100] = {};
int currentIndex = 0;
double totalDistance = 0;

void addMovement(int movement, double units, double distance) {
  movements[currentIndex] = {movement, units};
  currentIndex++;
  totalDistance += distance;
}

void goForward(double spaces) { addMovement(0, spaces, 50.0 * spaces); }
void goBack(double spaces) { addMovement(1, spaces, 50.0 * spaces); }
void goRight(double spaces) { addMovement(2, spaces, 50.0 * spaces); }
void goLeft(double spaces) { addMovement(3, spaces, 50.0 * spaces); }
void turnRight(double times) { addMovement(4, times, quarterTurn * times); }
void turnLeft(double times) { addMovement(5, times, quarterTurn * times); }

void run(double targetTime) {
  double cmPerSecond = totalDistance / targetTime;
  double rotations = totalDistance / 20.0;
  double RPS = rotations / targetTime;
  RPM = RPS * 60.0;

  // calculate acceleration
  double accelCmPerSecond2 = (accelDistance - cmPerSecond * accelTime) * 2 / (accelTime * accelTime);
  accel = accelCmPerSecond2 / 20.0;
  

  Brain.Screen.clearScreen();
  Brain.Screen.print("total distance: %f", totalDistance);
  Brain.Screen.newLine();
  Brain.Screen.print("cm per second: %f", cmPerSecond);
  Brain.Screen.newLine();
  Brain.Screen.print("RPM: %f", RPM);
  Brain.Screen.newLine();
  Brain.Screen.print("accel: %f", accel);
  
  MotorGroup1MotorA.setVelocity(RPM, rpm);
  MotorGroup1MotorB.setVelocity(RPM, rpm);
  MotorGroup7MotorA.setVelocity(RPM, rpm);
  MotorGroup7MotorB.setVelocity(RPM, rpm);

  for (auto &m : movements) {
    switch(m.movement) {
      case 0:
        MotorGroup7.spinFor(forward, m.units * 2.5, turns);
        break;
      case 1:
        MotorGroup7.spinFor(reverse, m.units * 2.5, turns);
        break;  
      case 2:
        MotorGroup1.spinFor(forward, m.units * 2.5, turns);
        break;  
      case 3:
        MotorGroup1.spinFor(reverse, m.units * 2.5, turns);
        break;  
      case 4:
        MotorGroup1MotorA.spin(forward);
        MotorGroup1MotorB.spin(reverse);
        MotorGroup7MotorA.spin(reverse); 
        MotorGroup7MotorB.spin(forward);
        robotTurn(m.units);
        break;  
      case 5:
        MotorGroup1MotorA.spin(reverse);
        MotorGroup1MotorB.spin(forward);
        MotorGroup7MotorA.spin(forward);
        MotorGroup7MotorB.spin(reverse);
        robotTurn(m.units);
        break;  
      default:
        Brain.Screen.clearScreen();
        Brain.Screen.print("error");
        break;
    }
  }
}

int main() {
  // ADD TARGET TIME!!!
  double TARGET_TIME = 2;

  // ROBOT COMMANDS!!!
  // goForward(1);
  // wait(2, seconds);
  MotorGroup7.setVelocity(50, rpm);
  MotorGroup7.spin(forward);
  wait(1, seconds);
  slowDown();
  /* ---------------- DO NOT EDIT ANYTHING BELOW! NO! ---------------- */
  run(TARGET_TIME);
}
