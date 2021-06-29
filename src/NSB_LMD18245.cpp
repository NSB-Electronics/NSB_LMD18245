// The MIT License (MIT)
//
// Copyright (c) 20121 NSB Electronics (Pty) Ltd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "NSB_LMD18245.h"


/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */
LMD_Stepper::LMD_Stepper(int numberOfSteps, uint8_t dirA_Pin, uint8_t dirB_Pin,
                                    uint8_t m4321A_Pin, uint8_t m4321B_Pin,
																		uint8_t brakeA_Pin, uint8_t brakeB_Pin) {
  this->stepNumber = 0;    // which step the motor is on
  this->direction = 0;     // motor direction
  this->lastStepTime = 0;  // timestamp in us of the last step taken
  this->numberOfSteps = numberOfSteps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->dirA_Pin = dirA_Pin;
  this->dirB_Pin = dirB_Pin;
  this->m4321A_Pin = m4321A_Pin;
  this->m4321B_Pin = m4321B_Pin;
	this->brakeA_Pin = brakeA_Pin;
	
	if (brakeB_Pin == 0xFF) {
		this->brakeB_Pin = brakeA_Pin; // Use the same brake pin for A and B
	} else {
		this->brakeB_Pin = brakeB_Pin;
	}
		

  // setup the pins on the microcontroller:
  pinMode(this->dirA_Pin, OUTPUT);
  pinMode(this->dirB_Pin, OUTPUT);
  pinMode(this->m4321A_Pin, OUTPUT);
  pinMode(this->m4321B_Pin, OUTPUT);
	pinMode(this->brakeA_Pin, OUTPUT);
  pinMode(this->brakeB_Pin, OUTPUT);
	
	digitalWrite(this->dirA_Pin, LOW);
	digitalWrite(this->dirB_Pin, LOW);
	digitalWrite(this->m4321A_Pin, LOW);
	digitalWrite(this->m4321B_Pin, LOW);
	digitalWrite(this->brakeA_Pin, HIGH);
	digitalWrite(this->brakeB_Pin, HIGH);
}

/*
 * Sets the drive type
 */
void LMD_Stepper::setDriveType(int driveType) {
  this->driveType = driveType;
}

/*
 * Sets the speed in revs per minute
 */
void LMD_Stepper::setSpeed(long whatSpeed) {
  this->stepDelay = 60L * 1000L * 1000L / this->numberOfSteps / whatSpeed;
}

/*
 * Sets the brake LOW or HIGH
 */
void LMD_Stepper::setBrake(int brake) {
  if (brake == BRAKE_LOW) {
		digitalWrite(this->brakeA_Pin, LOW);
		digitalWrite(this->brakeB_Pin, LOW);
	} else {
		digitalWrite(this->brakeA_Pin, HIGH);
		digitalWrite(this->brakeB_Pin, HIGH);
	}
}

/*
 * Moves the motor stepsToMove steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void LMD_Stepper::step(int stepsToMove) {
  int stepsLeft = abs(stepsToMove);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (stepsToMove > 0) { this->direction = 1; }
  if (stepsToMove < 0) { this->direction = 0; }

	// Set the brakes low so we can turn the motor
	setBrake(BRAKE_LOW);
	delay(1);
	
  // decrement the number of steps, moving one step each time:
  while (stepsLeft > 0) {
    unsigned long now = micros();
    // move only if the appropriate delay has passed:
    if (now - this->lastStepTime >= this->stepDelay) {
      // get the timeStamp of when you stepped:
      this->lastStepTime = now;
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1) {
        this->stepNumber++;
        if (this->stepNumber == this->numberOfSteps) {
          this->stepNumber = 0;
        }
      } else {
        if (this->stepNumber == 0) {
          this->stepNumber = this->numberOfSteps;
        }
        this->stepNumber--;
      }
      // decrement the steps left:
      stepsLeft--;
      // step the motor to step number 0, 1, ..., {3}
			if ((this->driveType == ONE_PHASE_ON_FULL_STEP) || (this->driveType == TWO_PHASE_ON_FULL_STEP)) {
				stepMotor(this->stepNumber % 4);
			} else if (this->driveType == HALF_STEP_NO_COMP) {
				stepMotor(this->stepNumber % 8);
			}
    }
  }
	// Set brake high to avoid excess current draw
	delay(1);
	setBrake(BRAKE_HIGH);
	
}

/*
 * Moves the motor forward or backwards.
 */
void LMD_Stepper::stepMotor(int thisStep) {
	if (this->driveType == ONE_PHASE_ON_FULL_STEP) {
		switch (thisStep) {
			case 0:  // 0101
				digitalWrite(this->dirA_Pin, LOW);
				digitalWrite(this->dirB_Pin, HIGH);
				digitalWrite(this->m4321A_Pin, LOW);
				digitalWrite(this->m4321B_Pin, HIGH);
			break;
			case 1:  // 0010
				digitalWrite(this->dirA_Pin, LOW);
				digitalWrite(this->dirB_Pin, LOW);
				digitalWrite(this->m4321A_Pin, HIGH);
				digitalWrite(this->m4321B_Pin, LOW);
			break;
			case 2:  // 1001
				digitalWrite(this->dirA_Pin, HIGH);
				digitalWrite(this->dirB_Pin, LOW);
				digitalWrite(this->m4321A_Pin, LOW);
				digitalWrite(this->m4321B_Pin, HIGH);
			break;
			case 3:  // 1110
				digitalWrite(this->dirA_Pin, HIGH);
				digitalWrite(this->dirB_Pin, HIGH);
				digitalWrite(this->m4321A_Pin, HIGH);
				digitalWrite(this->m4321B_Pin, LOW);
			break;
		}
	} else if (this->driveType == TWO_PHASE_ON_FULL_STEP) {
		digitalWrite(this->m4321A_Pin, HIGH);
		digitalWrite(this->m4321B_Pin, HIGH);
		switch (thisStep) {
			case 0:  // 01
				digitalWrite(this->dirA_Pin, LOW);
				digitalWrite(this->dirB_Pin, HIGH);
			break;
			case 1:  // 00
				digitalWrite(this->dirA_Pin, LOW);
				digitalWrite(this->dirB_Pin, LOW);
			break;
			case 2:  // 10
				digitalWrite(this->dirA_Pin, HIGH);
				digitalWrite(this->dirB_Pin, LOW);
			break;
			case 3:  // 11
				digitalWrite(this->dirA_Pin, HIGH);
				digitalWrite(this->dirB_Pin, HIGH);
			break;
		}
	} else if (this->driveType == HALF_STEP_NO_COMP) {
		switch (thisStep) {
			case 0:  // 0101
				digitalWrite(this->dirA_Pin, LOW);
				digitalWrite(this->dirB_Pin, HIGH);
				digitalWrite(this->m4321A_Pin, LOW);
				digitalWrite(this->m4321B_Pin, HIGH);
			break;
			case 1:  // 0111
				digitalWrite(this->dirA_Pin, LOW);
				digitalWrite(this->dirB_Pin, HIGH);
				digitalWrite(this->m4321A_Pin, HIGH);
				digitalWrite(this->m4321B_Pin, HIGH);
			break;
			case 2:  // 0010
				digitalWrite(this->dirA_Pin, LOW);
				digitalWrite(this->dirB_Pin, LOW);
				digitalWrite(this->m4321A_Pin, HIGH);
				digitalWrite(this->m4321B_Pin, LOW);
			break;
			case 3:  // 0011
				digitalWrite(this->dirA_Pin, LOW);
				digitalWrite(this->dirB_Pin, LOW);
				digitalWrite(this->m4321A_Pin, HIGH);
				digitalWrite(this->m4321B_Pin, HIGH);
			break;
			case 4:  // 1001
				digitalWrite(this->dirA_Pin, HIGH);
				digitalWrite(this->dirB_Pin, LOW);
				digitalWrite(this->m4321A_Pin, LOW);
				digitalWrite(this->m4321B_Pin, HIGH);
			break;
			case 5:  // 1011
				digitalWrite(this->dirA_Pin, HIGH);
				digitalWrite(this->dirB_Pin, LOW);
				digitalWrite(this->m4321A_Pin, HIGH);
				digitalWrite(this->m4321B_Pin, HIGH);
			break;
			case 6:  // 1110
				digitalWrite(this->dirA_Pin, HIGH);
				digitalWrite(this->dirB_Pin, HIGH);
				digitalWrite(this->m4321A_Pin, HIGH);
				digitalWrite(this->m4321B_Pin, LOW);
			break;
			case 7:  // 1111
				digitalWrite(this->dirA_Pin, HIGH);
				digitalWrite(this->dirB_Pin, HIGH);
				digitalWrite(this->m4321A_Pin, HIGH);
				digitalWrite(this->m4321B_Pin, HIGH);
			break;
		}
	}
}
