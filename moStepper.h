/*
 * moStepper.h
 *
 *  Created on: Dec 9, 2015
 *      Author: dukte_000
 */

#ifndef MOSTEPPER_H_
#define MOSTEPPER_H_


struct moStepper{
	//numSteps - number of total steps
	char numSteps;
	//div
	char div;
	signed char curStep;
	unsigned char* amp;
	//Pin offsets: The way to use these is as follows
	//if A = 9, check if A > 7. If so, the offsets are for port 2, otherwise, it is for port1
	//if for port 2, subtract 8 and then shift by the difference. In this case, 9 - 8 = 1 so A is located at 2.1
	unsigned char A,B,A_bar,B_bar;
	//Values along amp
	unsigned char *a,*b,*a_bar,*b_bar;

	void (*stepForward)(struct moStepper*);
	void (*stepBack)(struct moStepper*);
	void (*setMotor)(struct moStepper*);
};

void moStepperInit(struct moStepper*, unsigned char, unsigned char, unsigned char, unsigned char,unsigned char);
void stepF(struct moStepper*);
void stepB(struct moStepper*);
void setM(struct moStepper*);



#endif /* MOSTEPPER_H_ */
