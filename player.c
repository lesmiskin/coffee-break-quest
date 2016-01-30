#include <time.h>
#include "player.h"
#include "assets.h"
#include "input.h"
#include "hud.h"
#include "renderer.h"
#include "meter.h"
#include "scene.h"

const int SHOT_DELAY = 15;
double MOVE_INC = 1.4;
double SHOT_SPEED = 2;

Coord pos = { 50, 50 };
double targetInc = 0;
bool shotDir = false;
Shot shots[255];
int shotInc = 0;
long lastShot;
int walkInc = 0;
bool playerDir = false;
double health = 100;

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

	Coord target = makeCoord(
		playerDir ? 2 : -2,
		targetInc
	);

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

	double xPos = playerDir ? 7 : -7;
	Shot shot = { makeCoord(pos.x + xPos, pos.y+5), target, shotInc };
	shots[shotInc++] = shot;
}

bool isWalking(void) {
	return checkCommand(CMD_PLAYER_LEFT) ||
		checkCommand(CMD_PLAYER_RIGHT) ||
		checkCommand(CMD_PLAYER_DOWN) ||
		checkCommand(CMD_PLAYER_UP);
}

void playerAnimateFrame(void) {
	if(isWalking()) {
		walkInc = (walkInc == 0) ? 1 : 0;
	}
}

void playerShadowFrame(void) {
	if(mode != MODE_COMBAT) return;

	Sprite shadow = makeSimpleSprite("shadow.png");
	drawSprite(shadow, deriveCoord(pos, 0, 21));
}

void playerRenderFrame(void) {
	if(mode != MODE_COMBAT) return;

	//Draw player.
	Sprite player;
	SDL_RendererFlip flip = playerDir ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	if(isWalking()) {
		if(walkInc == 1) {
			player = makeFlippedSprite("timcalmstep.png", flip);
		}else{
			player = makeFlippedSprite("timcalm.png", flip);
		}
	}else{
		player = makeFlippedSprite("timcalm.png", flip);
	}

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
		if(health <= 0) {
			//TODO: Game over.
			switchMode();
		}

		if (checkCommand(CMD_PLAYER_LEFT)) {
			pos.x -= MOVE_INC;
			playerDir = false;
		}

		if (checkCommand(CMD_PLAYER_RIGHT)) {
			pos.x += MOVE_INC;
			playerDir = true;
		}

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
	memset(shots, 0, sizeof(shots));
}
