#include "player.h"
#include "assets.h"
#include "input.h"
#include "hud.h"

void drink(void) {
	play("drink.wav");
	spawnPlume(PLUME_DRINK);
}

void takeBreak(void) {
	play("break.wav");
}

void playerGameFrame(void) {
	if (checkCommand(CMD_PLAYER_DRINK)) drink();
	if (checkCommand(CMD_PLAYER_BREAK)) takeBreak();
}
