#include <MSP430.h>
#include <stdlib.h>
#include "moStepper.h"
/*
 * moStepper.c
 *
 *  Created on: Dec 9, 2015
 *      Author: dukte_000
 */

void moStepperInit(struct moStepper* thisStepper,unsigned char Ass,unsigned char Bss,unsigned char A_barss,unsigned char B_barss,unsigned char maxSteps){

	//Initialize pointer functions to header functions
	thisStepper->stepForward = &stepF;
	thisStepper->stepBack = &stepB;
	thisStepper->setMotor = &setM;

	//Initialize the amp
	thisStepper->amp = malloc(thisStepper->numSteps*sizeof(unsigned char));
	//placeholder intiialization for 4
	thisStepper->amp[0] = 1;
	thisStepper->amp[1] = 0;
	thisStepper->amp[2] = 0;
	thisStepper->amp[3] = 0;

	thisStepper->numSteps = maxSteps;

	thisStepper->div = thisStepper->numSteps/4;

	thisStepper->curStep = 0;

	thisStepper->a = thisStepper->amp;
	thisStepper->b_bar = thisStepper->amp + thisStepper->div;
	thisStepper->a_bar = thisStepper->amp + 2*thisStepper->div;
	thisStepper->b = thisStepper->amp + 3*thisStepper->div;

	thisStepper->A = Ass;
	thisStepper->B = Bss;
	thisStepper->A_bar = A_barss;
	thisStepper->B_bar = B_barss;

	//Set appropriate pins to outputs
	if(thisStepper->A > 7){
		P2DIR |= (1<<(thisStepper->A-8));
	}
	else{
		P1DIR |= (1<<thisStepper->A);
	}

	if(thisStepper->A_bar >7){
		P2DIR |= (1<<(thisStepper->A_bar - 8));
	}
	else{
		P1DIR |= (1<<(thisStepper->A_bar));
	}

	if(thisStepper->B >7){
		P2DIR |= (1<<(thisStepper->B - 8));
	}
	else{
		P1DIR |= (1<<(thisStepper->B));
	}

	if(thisStepper->B_bar >7){
		P2DIR |= (1<<(thisStepper->B_bar - 8));
	}
	else{
		P1DIR |= (1<<(thisStepper->B_bar));
	}

	thisStepper->setMotor(&thisStepper);

}


//setM is a DIGITAL function right now; it depends on a pointing to a 0/1 to be shifted.
//setM should be upgraded for analog values.

void setM(struct moStepper* thisStepper){
//----------------
	if(thisStepper->A > 7){
		if(*(thisStepper->a) == 1){
			P2OUT |= (1 << (int)(thisStepper->A - 8));
		}
		else{
			P2OUT &= ~(1 << (int)(thisStepper->A - 8));
		}
	}
	else{
		if(*(thisStepper->a) == 1){
			P1OUT |= (1 << (int)thisStepper->A);
		}
		else{
			P1OUT &= ~(1 << (int)thisStepper->A);
		}
	}

//----------------
	if(thisStepper->B > 7){
		if(*(thisStepper->b) == 1){
			P2OUT |= (1 << (int)(thisStepper->B - 8));
		}
		else{
			P2OUT &= ~(1 << (int)(thisStepper->B - 8));
		}
	}
	else{
		if(*(thisStepper->b) == 1){
			P1OUT |= (1 << (int)thisStepper->B);
		}
		else{
			P1OUT &= ~(1 << (int)thisStepper->B);
		}
	}
//--------------
	if(thisStepper->A_bar > 7){
		if(*(thisStepper->a_bar) == 1){
			P2OUT |= (1 << (int)(thisStepper->A_bar - 8));
		}
		else{
			P2OUT &= ~(1 << (int)(thisStepper->A_bar - 8));
		}
	}
	else{
		if(*(thisStepper->a_bar) == 1){
			P1OUT |= (1 << (int)thisStepper->A_bar);
		}
		else{
			P1OUT &= ~(1 << (int)thisStepper->A_bar);
		}
	}

//--------------
	if(thisStepper->B_bar > 7){
		if(*(thisStepper->b_bar) == 1){
			P2OUT |= (1 << (int)(thisStepper->B_bar - 8));
		}
		else{
			P2OUT &= ~(1 << (int)(thisStepper->B_bar - 8));
		}
	}
	else{
		if(*(thisStepper->b_bar) == 1){
			P1OUT |= (1 << (int)thisStepper->B_bar);
		}
		else{
			P1OUT &= ~(1 << (int)thisStepper->B_bar);
		}
	}
}

void stepF(struct moStepper* thisStepper){
	thisStepper->curStep++;
	thisStepper->a++;
	thisStepper->b++;
	thisStepper->a_bar++;
	thisStepper->b_bar++;

	if(thisStepper->curStep == thisStepper->div){
		thisStepper->b = thisStepper->amp;
	}
	else if(thisStepper->curStep == (2*thisStepper->div)){
		thisStepper->a_bar = thisStepper->amp;
	}
	else if(thisStepper->curStep == (3*thisStepper->div)){
		thisStepper->b_bar = thisStepper->amp;
	}
	else if(thisStepper->curStep == (4*thisStepper->div)){
		thisStepper->a = thisStepper->amp;
		thisStepper->curStep = 0;
	}

	thisStepper->setMotor(thisStepper);
}

void stepB(struct moStepper* thisStepper){
	thisStepper->curStep--;
	thisStepper->a--;
	thisStepper->b--;
	thisStepper->a_bar--;
	thisStepper->b_bar--;


	if(thisStepper->curStep == (2*thisStepper->div)){
		thisStepper->b_bar = &thisStepper->amp[thisStepper->numSteps-1];
	}
	else if(thisStepper->curStep == (thisStepper->div)){
		thisStepper->a_bar = &thisStepper->amp[thisStepper->numSteps-1];
	}
	else if(thisStepper->curStep == 0){
		thisStepper->b = &thisStepper->amp[thisStepper->numSteps-1];
	}
	else if(thisStepper->curStep == -1){
		thisStepper->a = &thisStepper->amp[thisStepper->numSteps-1];
		thisStepper->curStep = thisStepper->numSteps-1;
	}

	thisStepper->setMotor(thisStepper);
}
