#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include "common.h"
#include "input.h"
#include "assets.h"
#include "renderer.h"
#include "scene.h"
#include "mysdl.h"
#include "player.h"
#include "meter.h"
#include "hud.h"

//EMILY WORK
//TODO: Game mechanics and calculations.
    //TODO: Caff reduces over time.
    //TODO: Prod increases proportional to the caffiene.
    //TODO: Hygene decreases when breaks are not taken.
    //TODO: Bladder increases when breaks are not taken.
//TODO: Put props in the Office game.
//TODO: Update sprites in Office game.
//TODO: Hook up proper character sprites in Combat game.

//OFFICE
//TODO: Numbers should flash.
//TODO: Large messages that pop up on the screen: "YOU FAIL IT"
//TODO: Sigh for blad expulsion.
//TODO: Get up / come back "animation"

//COMBAT
//TODO: Introduce enemies.
//TODO: Enemies shoot coffee cups.
//TODO: When hit, pushes them away.
//TODO: Goal is to push them all offscreen.
//TODO: Enemies cause GAME OVER state (b&w background).
//TODO: Once killed, game loops back to start.

//*************************************************************

//IF TIME
//TODO: Tapping on keyboard sound effect.
//TODO: Tapping and bobbing animations speed up when time is sped up.
//TODO: Meters.
//TODO: Particle effect for game-over.
//TODO: Random objects on table.
//TODO: Cheat code to force dismemberment (Konami code - learning is FUN!).
//TODO: Ties streaming behind them?
//TODO: Monochrome effect.
//TODO: Tapping sound effect for keyboard.
//TODO: Hands going up and down.
//TODO: Feet going up and down.
//TODO: Control menu in bottom right of screen.
//TODO: Exploding cat.




static const char *GAME_TITLE = "Coffee Break Quest";
const int ANIMATION_HZ = 1000 / 12;		//12fps
const int RENDER_HZ = 1000 / 60;		//60fps
const int GAME_HZ = 1000 / 60;			//60fps

bool running = true;
SDL_Window *window = NULL;
Coord windowSize = { 1280, 960 };   // 320x240

static void initSDL(void) {
    SDL_Init(/*SDL_INIT_AUDIO | */SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
    if(!IMG_Init(IMG_INIT_PNG)) {
        fatalError("Fatal error", "SDL_Image did not initialise.");
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fatalError("Fatal error", "SDL_Mixer did not initialise.");
    }
    SDL_InitSubSystem(SDL_INIT_VIDEO);
}

static void initWindow(void) {
    window = SDL_CreateWindow(
        GAME_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        (int)windowSize.x,					//dimensions
        (int)windowSize.y,
        SDL_WINDOW_OPENGL
    );
    assert(window != NULL);
}

static void shutdownWindow(void) {
    if(window == NULL) return;			//OK to call if not yet setup (an established subsystem pattern elsewhere)

    SDL_DestroyWindow(window);
    window = NULL;
}

static void shutdownMain(void) {
    shutdownAssets();
    shutdownRenderer();
    shutdownWindow();

    SDL_Quit();
}

int main()  {
    //Seed randomMq number generator
    srand(time(NULL));

    atexit(shutdownMain);

    initSDL();
    initWindow();
    initRenderer();
    initAssets();
	initHud();
    initPlayer();

    long lastRenderFrameTime = clock();
    long lastGameFrameTime = lastRenderFrameTime;
    long lastAnimFrameTime = lastRenderFrameTime;

    //Main game loop (realtime)
    while(running){
        //Game frame
        if(timer(&lastGameFrameTime, GAME_HZ)) {
            pollInput();
            playerGameFrame();
			meterGameFrame();
			hudGameFrame();
            processSystemCommands();
        }

		//Animation frame
		if(timer(&lastAnimFrameTime, ANIMATION_HZ)) {
			sceneAnimateFrame();
		}

        //Renderer frame
        double renderFPS;
        if(timer(&lastRenderFrameTime, RENDER_HZ)) {
            sceneRenderFrame();
			playerRenderFrame();
			hudRenderFrame();
            updateCanvas();
        }
    }

    return 0;
}
