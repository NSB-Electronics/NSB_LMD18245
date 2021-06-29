/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.

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

void setup() {
  pinMode(TXS_PIN, OUTPUT);
  digitalWrite(TXS_PIN, HIGH);
  
  // Set the speed at 60 rpm:
  myStepper.setSpeed(300); // There is a maximum before motor slips, also faster = less torque
  
  myStepper.setDriveType(ONE_PHASE_ON_FULL_STEP); // Default
  //myStepper.setDriveType(TWO_PHASE_ON_FULL_STEP); // More torque
  //myStepper.setDriveType(HALF_STEP_NO_COMP); // Less torque, more accuracy
  
  //myStepper.setBrake(BRAKE_HIGH); // Sets the brake high to avoid excess current draw
  //myStepper.setBrake(BRAKE_LOW);  // Sets the brake low, draws current but holds motor in place
  
  // Initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // Step one revolution in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(1000);

  // Step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(1000);
}
