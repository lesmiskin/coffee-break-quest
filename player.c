#include "player.h"
#include "assets.h"
#include "input.h"

void drink(void) {
	play("drink.wav");
}

void speedup(void) {
	play("up.wav");
}

void slowdown(void) {
	play("down.wav");
}

void takeBreak(void) {
	play("break.wav");
}

void playerGameFrame(void) {
	if (checkCommand(CMD_PLAYER_DRINK)) drink();
	if (checkCommand(CMD_PLAYER_BREAK)) takeBreak();
	if (checkCommand(CMD_GAME_SLOWDOWN)) slowdown();
	if (checkCommand(CMD_GAME_SPEEDUP)) speedup();
}
