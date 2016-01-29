#include <stdbool.h>
#include "meter.h"
#include "common.h"
#include "input.h"

#define MAX_COMMANDS 20

static bool commands[MAX_COMMANDS];

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

                //List of key PRESSES go here.
                if(keypress == SDL_SCANCODE_ESCAPE)
                    commands[CMD_QUIT] = true;
            }
        }
    }

    //List of HELD keys go here.
    if(keysHeld[SDL_SCANCODE_D])
        commands[CMD_PLAYER_DRINK] = true;
    if(keysHeld[SDL_SCANCODE_B])
        commands[CMD_PLAYER_BREAK] = true;
    if(keysHeld[SDL_SCANCODE_UP])
        commands[CMD_GAME_SPEEDUP] = true;
    if(keysHeld[SDL_SCANCODE_DOWN])
        commands[CMD_GAME_SLOWDOWN] = true;
}

void processSystemCommands(void) {
    if(checkCommand(CMD_QUIT)) quit();
}
