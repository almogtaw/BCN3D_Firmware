//accelstepper constant speed example has been altered and is being used
// not using default 4 wires setup, but instead using step, direction, and enable pins
// using TB6600 4A 9-42V stepper driver at 6400 pulses/rev (32 microsteps)

// Example taken from:
// https://forum.arduino.cc/t/using-accelstepper-library-w-stepper-driver-step-direction-and-enable-pins/639451/6

#include <AccelStepper.h>

// defines pins numbers
const int stepPin = 54;
const int directionPin = 55;
const int enablePin = 38;

// Define a stepper and the pins it will use
// 1 or AccelStepper::DRIVER means a stepper driver (with Step and Direction pins)
AccelStepper stepper(AccelStepper::DRIVER, stepPin, directionPin);

void setup()
{
  Serial.begin(57600);

  stepper.setEnablePin(enablePin);
  stepper.setPinsInverted(false, false, true);
  stepper.enableOutputs();
  stepper.setAcceleration(5000);
  stepper.setMaxSpeed(100000);
  stepper.setSpeed(10000);

}

void loop()
{
  stepper.runToNewPosition(20000);

    // Print the current speed to the serial monitor
  // Serial.print("Current Speed: ");
  // Serial.println(stepper.speed());

  // // Add a small delay to avoid flooding the serial monitor with messages
  // delay(100);
}