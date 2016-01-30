#include "common.h"
#include "assets.h"
#include "input.h"

//Tying animation.
//Player needs typing animation (head bobbing)
//Player needs drinking animation (grabs coffee, drinks, then puts back).
//Player needs 'nearing break' animation (jiggling body).

//Arcade mode: Fighting off other employees.
//Arcade combat:

double alertness = 50;
double hygiene = 100;
double work = 0;
double bladder = 0;
double timeProgress;
double timeRate = 1;

const double TIME_INC = 0.02;
const double MAX_SPEED = 60;
const double SPEED_INC = 0.1;
const double TIME_END = 480;
const double SLOW_INC = 0.5;

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
	if(mode == MODE_OFFICE) {
		//Quit game once time is up.
		if(timeProgress >= TIME_END) quit();
		//Increment meters
		timeProgress += (TIME_INC * timeRate);
		bladder += (TIME_INC * timeRate * SLOW_INC);
		alertness -= (TIME_INC * timeRate * SLOW_INC);
		work += (alertness/100 * TIME_INC * timeRate * SLOW_INC);

		// Perform actions triggered by meters
		if (work >= 100){
			quit();//"YOU WIN!!!"
		}
		if (bladder >= 100){
			//play("sigh.wav");
			hygiene -= 40;
			bladder = 0;
		}
		if (hygiene <= 0){
			changeMode(MODE_COMBAT);
			hygiene = 40;
		}
		if (alertness <= 0) quit(); //"YOU LOSE"

		if (checkCommand(CMD_GAME_SLOWDOWN)) slowdown();
		if (checkCommand(CMD_GAME_SPEEDUP)) speedup();
	}
}
