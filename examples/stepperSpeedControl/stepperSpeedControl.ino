/*
 Stepper Motor Control - speed control

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.
 A potentiometer is connected to analog input 0.

 The motor will rotate in a clockwise direction. The higher the potentiometer value,
 the faster the motor speed. Because setSpeed() sets the delay between steps,
 you may notice the motor is less responsive to changes in the sensor value at
 low speeds.

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

int stepCount = 0;  // number of steps the motor has taken

void setup() {
  pinMode(TXS_PIN, OUTPUT);
  digitalWrite(TXS_PIN, HIGH);
}

void loop() {
  // read the sensor value:
  int sensorReading = analogRead(A0);
  // map it to a range from 0 to 100:
  int motorSpeed = map(sensorReading, 0, 1023, 0, 100);
	motorSpeed = 50;
  // set the motor speed:
  if (motorSpeed > 0) {
    myStepper.setSpeed(motorSpeed);
    // step 1/100 of a revolution:
    myStepper.step(stepsPerRevolution / 100);
  }
}
