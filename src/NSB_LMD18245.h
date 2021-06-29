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

#ifndef NSB_LMD18245_h
#define NSB_LMD18245_h

#include "Arduino.h"

#define BRAKE_LOW		LOW
#define BRAKE_HIGH	HIGH

#define ONE_PHASE_ON_FULL_STEP	0
#define TWO_PHASE_ON_FULL_STEP	1
#define HALF_STEP_NO_COMP				2
// Other drive types require independent control of the M4,M3,M2,M1 pins


// library interface description
class LMD_Stepper {
  public:
    // constructors:
    LMD_Stepper(int numberOfSteps, uint8_t dirA_Pin, uint8_t dirB_Pin,
                               uint8_t m4321A_Pin, uint8_t m4321B_Pin,
															 uint8_t brakeA_Pin, uint8_t brakeB_Pin = 0xFF);
		
		void setDriveType(int driveType = ONE_PHASE_ON_FULL_STEP);
		
    // speed setter method:
    void setSpeed(long whatSpeed);
		
		void setBrake(int brake = BRAKE_HIGH);

    // mover method:
    void step(int numberOfSteps);

  private:
    void stepMotor(int this_Step);

    int direction;            // Direction of rotation
    unsigned long stepDelay;  // delay between steps, in ms, based on speed
    int numberOfSteps;        // total number of steps this motor can take
    int stepNumber;           // which step the motor is on
		int driveType;						// 

    // motor pin numbers:
    int dirA_Pin;
    int dirB_Pin;
    int m4321A_Pin;
    int m4321B_Pin;
		int brakeA_Pin;
		int brakeB_Pin;

    unsigned long lastStepTime; // timestamp in us of when the last step was taken
};

#endif

