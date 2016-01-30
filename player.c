#include <time.h>
#include "player.h"
#include "assets.h"
#include "input.h"
#include "hud.h"
#include "renderer.h"
#include "meter.h"
#include "scene.h"

typedef struct {
	Coord coord;
	Coord target;
	double inc;
} Shot;

#define MAX_SHOTS 100
const int SHOT_DELAY = 15;
double MOVE_INC = 2;
double SHOT_SPEED = 2;

Coord pos = { 50, 50 };
double targetInc = 0;
bool shotDir = false;
Shot shots[255];
int shotInc = 0;
long lastShot;

double hyg_inc = 10;
double hyg_max = 100;

void drink(void) {
	play("drink.wav");
	spawnPlume(PLUME_DRINK);
	bladder++;
	alertness++;
	onDrink = true;
}

void takeBreak(void) {
	play("break.wav");
	spawnPlume(PLUME_BREAK);
	bladder=0;
	work = (work < 1) ? 0 : work--;
	if (hygiene >= hyg_max - hyg_inc) {
		hygiene = hyg_max;
	}
	else {
		hygiene+=hyg_inc;
	}
	onBreak = true;
}

void shoot(void) {
	play("shoot3.wav");

	if(!timer(&lastShot, SHOT_DELAY)) {
		return;
	}

	//Reset shot inc.
	if(shotInc >= MAX_SHOTS) {
		shotInc = 0;
	}

	Shot shot = { makeCoord(pos.x, pos.y+10), makeCoord(1.5, targetInc), shotInc };
	shots[shotInc++] = shot;

	if(shotDir == true && targetInc > 1) {
		shotDir = false;
	}else if(shotDir == false && targetInc < -1) {
		shotDir = true;
	}
	if(shotDir) {
		targetInc += 0.15;
	}else{
		targetInc -= 0.15;
	}
}

void playerRenderFrame(void) {
	if(mode != MODE_COMBAT) return;

	//Draw player.
	Sprite player = makeSimpleSprite("combat-player.png");
	drawSprite(player, pos);

	//Draw shots.
	Sprite shot = makeSimpleSprite("shot.png");
	for(int i=0; i < MAX_SHOTS; i++) {
		if(shots[i].coord.x == 0) continue;
		drawSprite(shot, shots[i].coord);
	}
}

void playerGameFrame(void) {
	if(mode == MODE_COMBAT) {
		if (checkCommand(CMD_PLAYER_LEFT)) pos.x -= MOVE_INC;
		if (checkCommand(CMD_PLAYER_RIGHT)) pos.x += MOVE_INC;
		if (checkCommand(CMD_PLAYER_UP)) pos.y -= MOVE_INC;
		if (checkCommand(CMD_PLAYER_DOWN)) pos.y += MOVE_INC;
		if (checkCommand(CMD_PLAYER_FIRE)) shoot();

		//Move the shots.
		for(int i=0; i < MAX_SHOTS; i++) {
			if(shots[i].coord.x == 0) continue;
			shots[i].coord.x += shots[i].target.x;
			shots[i].coord.y += shots[i].target.y;
		}
	}else{
		if (checkCommand(CMD_PLAYER_DRINK)) drink();
		if (checkCommand(CMD_PLAYER_BREAK)) takeBreak();
	}
}

void initPlayer() {
	lastShot = clock();
}
