#include <stdbool.h>
#include "meter.h"
#include "common.h"
#include "input.h"
#include "enemy.h"

#define MAX_COMMANDS 20

static bool commands[MAX_COMMANDS];
bool firstOffice = true;
bool firstCombat = true;

bool checkCommand(int commandFlag) {
    return commands[commandFlag];
}

void pollInput(void) {
    SDL_PumpEvents();
    const Uint8 *keysHeld = SDL_GetKeyboardState(NULL);

    //We're on a new frame, so clear all previous checkCommand (not key) states (i.e. set to false)
    memset(commands, 0, sizeof(commands));

    //Respond to SDL events, or key presses (not holds)
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0) {
        switch(event.type) {
            case SDL_QUIT:
                commands[CMD_QUIT] = true;
                break;
                //Presses
            case SDL_KEYDOWN: {
                //Ignore held keys.
                if(event.key.repeat) break;

                SDL_Keycode keypress = event.key.keysym.scancode;

				switch(mode) {
					case MODE_TITLE:
						if(keypress == SDL_SCANCODE_SPACE)
							changeMode(MODE_OFFICE_INTRO);
						if(keypress == SDL_SCANCODE_ESCAPE)
							commands[CMD_QUIT] = true;

						break;
					case MODE_OFFICE_WON:
					case MODE_OFFICE_LOST:
					case MODE_COMBAT_WON:
					case MODE_COMBAT_LOST:
						if(keypress == SDL_SCANCODE_SPACE)
							changeMode(MODE_TITLE);
						break;
					case MODE_BREAK_OOPS:
						if(keypress == SDL_SCANCODE_SPACE)
							changeMode(MODE_COMBAT);
						break;
					case MODE_OFFICE_INTRO:
						if(keypress == SDL_SCANCODE_SPACE)
							changeMode(MODE_OFFICE);
						break;
					case MODE_BREAK_INTRO:
					case MODE_COMBAT_FIRSTTIME:
						if(keypress == SDL_SCANCODE_SPACE)
							changeMode(MODE_COMBAT);
						break;
				}

				//Exit to title.
				if(mode != MODE_TITLE) {
					if(keypress == SDL_SCANCODE_ESCAPE)
					changeMode(MODE_TITLE);
				}

//                //List of key PRESSES go here.
                if(keypress == SDL_SCANCODE_X) {
					aggro = true;
					changeMode(MODE_COMBAT);
				}
            }
        }
    }

	//Combat keys
	if(mode == MODE_COMBAT) {
		if(keysHeld[SDL_SCANCODE_LEFT])
			commands[CMD_PLAYER_LEFT] = true;
		else if(keysHeld[SDL_SCANCODE_RIGHT])
			commands[CMD_PLAYER_RIGHT] = true;

		if(keysHeld[SDL_SCANCODE_UP])
			commands[CMD_PLAYER_UP] = true;
		else if(keysHeld[SDL_SCANCODE_DOWN])
			commands[CMD_PLAYER_DOWN] = true;

		if(aggro) {
			if (keysHeld[SDL_SCANCODE_SPACE])
				commands[CMD_PLAYER_FIRE] = true;
		}

	//Office keys
	} else {
		if(mode == MODE_OFFICE) {
			if(keysHeld[SDL_SCANCODE_D])
				commands[CMD_PLAYER_DRINK] = true;
			if(keysHeld[SDL_SCANCODE_B])
				commands[CMD_PLAYER_BREAK] = true;
			if(keysHeld[SDL_SCANCODE_UP])
				commands[CMD_GAME_SPEEDUP] = true;
			if(keysHeld[SDL_SCANCODE_DOWN])
				commands[CMD_GAME_SLOWDOWN] = true;
		}
	}
}

void processSystemCommands(void) {
    if(checkCommand(CMD_QUIT)) quit();
}
