#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

// Joint 1
#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

// Joint 2
#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

// Joint 3
#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

// Joint 4
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38

// Joint 5 
#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

// Max velocities - empirical
#define joint_1_max_vel    0
#define joint_2_max_vel    0
#define joint_3_max_vel    4000
#define joint_4_max_vel    0   
#define joint_5_max_vel    5000

// Max accelerations - empirical
#define joint_1_max_accel    0
#define joint_2_max_accel    0
#define joint_3_max_accel    10000
#define joint_4_max_accel    0   
#define joint_5_max_accel    5000

struct joint_parameters
{
  int min_travel;       // [steps]
  int max_travel;       // [steps]
  float min_vel;        // [steps/sec]
  float max_vel;        // [steps/sec]
  float default_vel;    // [steps/sec]
  float min_accel;      // [steps/sec^2]
  float max_accel;      // [steps/sec^2]
  float default_accel;  // [steps/sec^2]
  float steps_per_deg;  // [steps/deg]

  // Constructor to initialize the values
  joint_parameters(int mt = 0, int mxt = 0, float miv = 0.0, float mav = 0.0, float dv = 0.0, float mia = 0.0, float maa = 0.0, float da = 0.0, float spd = 0.0)
    : min_travel(mt), max_travel(mxt), min_vel(miv), max_vel(mav), default_vel(dv), min_accel(mia), max_accel(maa), default_accel(da), steps_per_deg(spd) {}
};

// Initialize each joint with tested parameters:
joint_parameters joint1_param(0, 1000, 0.0, 1000.0, 500.0, 0.0, 1000.0, 500.0, 1.0);
joint_parameters joint2_param(0, 1000, 0.0, 1000.0, 500.0, 0.0, 1000.0, 500.0, 1.0);
joint_parameters joint3_param(0, 1000, 0.0, 1000.0, 500.0, 0.0, 1000.0, 500.0, 1.0);
joint_parameters joint4_param;  // inactive joint - all values initialize to zero in the constructor
joint_parameters joint5_param(0, 1000, 0.0, 1000.0, 500.0, 0.0, 1000.0, 500.0, 1.0);

// Create an array of pointers to the instances
joint_parameters* joints_params[] = {&joint1_param, &joint2_param, &joint3_param, &joint4_param, &joint5_param};


AccelStepper joint1(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper joint2(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper joint3(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper joint4(AccelStepper::DRIVER, E1_STEP_PIN, E1_DIR_PIN);
AccelStepper joint5(AccelStepper::DRIVER, E0_STEP_PIN, E0_DIR_PIN);

Servo gripper;
MultiStepper steppers;

AccelStepper* joints[] = {&joint1, &joint2, &joint3, &joint4, &joint5};

void setup() {
  Serial.begin(57600); // Start serial communication
  pinMode(13, OUTPUT);

  // Configure each stepper enable pin
  joint1.setEnablePin(X_ENABLE_PIN);
  joint2.setEnablePin(Z_ENABLE_PIN);
  joint3.setEnablePin(Y_ENABLE_PIN);
  joint4.setEnablePin(E1_ENABLE_PIN);
  joint5.setEnablePin(E0_ENABLE_PIN);

  // Configure each stepper enable pin inverted
  joint1.setPinsInverted(false, false, true);
  joint2.setPinsInverted(false, false, true);
  joint3.setPinsInverted(false, false, true);
  joint4.setPinsInverted(false, false, true);
  joint5.setPinsInverted(false, false, true);

  // enable outputs
  joint1.enableOutputs();
  joint2.enableOutputs();
  joint3.enableOutputs();
  joint4.enableOutputs();
  joint5.enableOutputs();

  // Configure each stepper velocity
  joint1.setMaxSpeed(1500);
  joint2.setMaxSpeed(750);
  joint3.setMaxSpeed(2000);
  joint4.setMaxSpeed(0);
  joint5.setMaxSpeed(1000);

  // Configure each stepper acceleration
  joint1.setAcceleration(10000.0);
  joint2.setAcceleration(1000.0);
  joint3.setAcceleration(10000.0);
  joint4.setAcceleration(0.0);
  joint5.setAcceleration(5000.0);

  // Then give them to MultiStepper to manage
  // steppers.addStepper(joint1);
  // steppers.addStepper(joint2);
  // steppers.addStepper(joint3);
  // steppers.addStepper(joint4);
  // steppers.addStepper(joint5);

  // Configure gripper servo
  gripper.attach(11);
  
  digitalWrite(13, 1); // Toggle LED to indicate setup is complete
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Read the command from the serial port

    // Print the received command
    Serial.print("Received command: ");
    Serial.println(command);

    // Parse the command
    int joint_num;
    long position;
    long velocity;
    int parsed_items = sscanf(command.c_str(), "move joint_%d %ld %ld", &joint_num, &position, &velocity);



    if (parsed_items >= 2 && joint_num >= 1 && joint_num <= 5) 
    {
      if (parsed_items == 3) {
        joints[joint_num - 1]->setMaxSpeed(velocity);
      }
      joints[joint_num - 1]->runToNewPosition(position);
    } else if (command.startsWith("GRIP ")) 
    {
      int angle = 0;
      sscanf(command.c_str(), "GRIP %d", &angle);
      gripper.write(angle); // Set servo angle
      digitalWrite(13, HIGH - digitalRead(13)); // Toggle LED to indicate gripper command received
    }

    // steppers.runSpeedToPosition(); // Blocks until all are in position
  }
  digitalWrite(13, HIGH - digitalRead(13)); // Toggle LED to indicate loop activity

  //delay(1);
}
