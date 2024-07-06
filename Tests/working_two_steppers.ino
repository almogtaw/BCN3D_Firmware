// MultiStepper.pde
// -*- mode: C++ -*-
// Use MultiStepper class to manage multiple steppers and make them all move to 
// the same position at the same time for linear 2d (or 3d) motion.

#include <AccelStepper.h>
#include <MultiStepper.h>

// Joint 1
#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38

// Joint 2 
#define E0_STEP_PIN 26
#define E0_DIR_PIN 28
#define E0_ENABLE_PIN 24

AccelStepper joint1(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper joint2(AccelStepper::DRIVER, E0_STEP_PIN, E0_DIR_PIN);

// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper steppers;

//test with uint8 converted to long
unsigned int x = 1000;

void setup() {
  Serial.begin(250000);

  joint1.setEnablePin(X_ENABLE_PIN);
  joint2.setEnablePin(E0_ENABLE_PIN);
  joint1.setPinsInverted(false, false, true);
  joint2.setPinsInverted(false, false, true);
  joint1.enableOutputs();
  joint2.enableOutputs();

  // Configure each stepper
  joint1.setMaxSpeed(10000);
  joint2.setMaxSpeed(10000);

  joint1.setAcceleration(100);

  // Then give them to MultiStepper to manage
  steppers.addStepper(joint1);
  steppers.addStepper(joint2);
}

void loop() {
  long positions[2]; // Array of desired stepper positions

  // Back of the envelope calculation for microsteps/revolution, where positions[i] is the number of steps (or microsteps).
  positions[0] = 0; //4100 microsteps is 1/8 revolutions ----> 32800 microsteps/rev
  positions[1] = 0; //2000 is 40/360 revolutions ---> 18000 microsteps/rev
  
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1);
  
  // Move to a different coordinate
  positions[0] = 10000;
  positions[1] = 10000;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1);
}
