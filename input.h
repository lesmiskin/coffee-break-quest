#ifndef INPUT_H
#define INPUT_H

#include "common.h"

extern void pollInput(void);
extern void processSystemCommands(void);
extern bool checkCommand(int commandFlag);

typedef enum {
    CMD_QUIT = 0,
    CMD_PLAYER_DRINK = 1,
    CMD_PLAYER_BREAK = 2,
	CMD_GAME_SPEEDUP = 3,
	CMD_GAME_SLOWDOWN = 4
} Command;

#endif