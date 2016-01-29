#include "common.h"
#include "assets.h"
#include "input.h"

//Tying animation.
//Player needs typing animation (head bobbing)
//Player needs drinking animation (grabs coffee, drinks, then puts back).
//Player needs 'nearing break' animation (jiggling body).

//Arcade mode: Fighting off other employees.
//Arcade combat:

double alertness;
double hygiene;
double work;
double bladder;
double timeProgress;
double timeRate = 1;

const double TIME_INC = 0.02;
const double MAX_SPEED = 60;
const double SPEED_INC = 0.1;
const double TIME_END = 480;

void speedup() {
	if(timeRate + SPEED_INC > MAX_SPEED) return;
	timeRate += SPEED_INC;
	play("up.wav");
}

void slowdown() {
	if(timeRate - SPEED_INC < SPEED_INC) return;
	timeRate -= SPEED_INC;
	play("down.wav");
}

void meterGameFrame(void) {
	//Quit game once time is up.
	if(timeProgress >= TIME_END) quit();
	timeProgress += (TIME_INC * timeRate);

	if (checkCommand(CMD_GAME_SLOWDOWN)) slowdown();
	if (checkCommand(CMD_GAME_SPEEDUP)) speedup();
}
