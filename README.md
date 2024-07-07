# BCN3D_Firmware
Arduino Firmware for BCN3D Moveo robotic arm


## How to use
Upload moveo_arduino.ino to arduino
send serial commands using:
1. Arduino IDE Serial Monitor
2. Ubuntu CLI:
    open two windows:
    In one window run: 
    stty -F /dev/ttyACM0 57600
    In the second window:
    cat /dev/ttyACM0  - this will echo the arduino commands received

    In the first window run commands in the following format:
    GRIP {angle} - angle is the target angle for the gripper
    move joint_{joint #} {position} {velocity}

    examples:
    echo "move joint_3 200 100" > /dev/ttyACM0
    echo "GRIP 90" > /dev/ttyACM0
## Dependencies
AccelStepper library