/*
 Stepper Motor Control - one step at a time

 This program drives a unipolar or bipolar stepper motor.

 The motor will step one step at a time, very slowly.  You can use this to
 test that you've got the four wires of your stepper wired to the correct
 pins. If wired correctly, all steps should be in the same direction.

 Use this also to count the number of steps per revolution of your motor,
 if you don't know it.  Then plug that number into the oneRevolution
 example to see if you got it right.
 
 */

#include <NSB_LMD18245.h>

#define DIRA_PIN    10
#define DIRB_PIN    12
#define M4321A_PIN  6
#define M4321B_PIN  7
#define BRAKEA_PIN  13
#define BRAKEB_PIN  13

#define TXS_PIN     2

const int stepsPerRevolution = 200;  // Change this to fit the number of steps per revolution for your motor

// Initialize the Motor library
LMD_Stepper myStepper(stepsPerRevolution, DIRA_PIN, DIRB_PIN, M4321A_PIN, M4321B_PIN, BRAKEA_PIN);
//LMD_Stepper myStepper(stepsPerRevolution, DIRA_PIN, DIRB_PIN, M4321A_PIN, M4321B_PIN, BRAKEA_PIN, BRAKEB_PIN);

int stepCount = 0;         // number of steps the motor has taken

void setup() {
  pinMode(TXS_PIN, OUTPUT);
  digitalWrite(TXS_PIN, HIGH);
  
  myStepper.setDriveType(ONE_PHASE_ON_FULL_STEP); // Default
  //myStepper.setDriveType(TWO_PHASE_ON_FULL_STEP); // More torque
  //myStepper.setDriveType(HALF_STEP_NO_COMP); // Less torque, more accuracy
  
  //myStepper.setBrake(BRAKE_HIGH); // Sets the brake high to avoid excess current draw
  //myStepper.setBrake(BRAKE_LOW);  // Sets the brake low, draws current but holds motor in place
  
  // Initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one step:
  myStepper.step(1);
  Serial.print("steps:");
  Serial.println(stepCount);
  stepCount++;
  delay(1);
}
