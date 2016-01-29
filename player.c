#include "player.h"
#include "assets.h"
#include "input.h"

void drink(void) {
	play("drink.wav");
}

void takeBreak(void) {
	play("break.wav");
}

void playerGameFrame(void) {
	if (checkCommand(CMD_PLAYER_DRINK)) drink();
	if (checkCommand(CMD_PLAYER_BREAK)) takeBreak();
}
