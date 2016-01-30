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
#include "enemy.h"

//COMBAT / LES

//*************************************************************

//LES
//TODO: Randomise the people sprites.
//TODO: Introduce people captions.
//TODO: LES - Tutorial text.
//TODO: LES - GAME OVER screens.
//TODO: When hit, pushes them away.
//TODO: Goal is to push them all offscreen.
//TODO: Enemies cause GAME OVER state (b&w background).
//TODO: Health bar (icons).
//TODO: Review sounds.
//TODO: Tapping on keyboard sound effect.
//TODO: Coffee cups cause "hit" sound.
//TODO: Coffee cups cause "hit" graphic on player.
//TODO: Random coffee cup thrown from unknown assailant.
//TODO: Once killed, game loops back to start.
//TODO: Detect resolution and size accordingly.

//POLISH
//TODO: Smoothing out animations.
//TODO: 'x' press bug: Are we clearing the arrays of out-of-range shots etc?
//TODO: Cheat code to force dismemberment (Konami code - learning is FUN!).
//TODO: Momentum on Tim.
//TODO: Retail distribution on Linux, Mac, and Windows.

//IF TIME
//TODO: Music...
//TODO: Hands going up and down, using arm sprite.
//TODO: Tapping and bobbing animations speed up when time is sped up.
//TODO: Ties streaming behind them?
//TODO: Monochrome effect.
//TODO: Tapping sound effect for keyboard.
//TODO: Forced re-entry upon game exit (Y/Y etc.)


static const char *GAME_TITLE = "Coffee Break Quest";
const int ANIMATION_HZ = 1000 / 12;		//12fps
const int RENDER_HZ = 1000 / 60;		//60fps
const int GAME_HZ = 1000 / 60;			//60fps

bool running = true;
SDL_Window *window = NULL;

#ifdef _WIN32
#elif __APPLE__
    Coord windowSize = { 640, 480};
#elif __linux__
    Coord windowSize = { 1280, 960 };   // 320x240
#endif

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
    randomNo = randomMq(1,100);
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
            enemyGameFrame();
			meterGameFrame();
			hudGameFrame();
            processSystemCommands();
        }

		//Animation frame
		if(timer(&lastAnimFrameTime, ANIMATION_HZ)) {
            enemyAnimateFrame();
			sceneAnimateFrame();
            playerAnimateFrame();
		}

        //Renderer frame
        double renderFPS;
        if(timer(&lastRenderFrameTime, RENDER_HZ)) {
            sceneRenderFrame();
			playerRenderFrame();
            enemyRenderFrame();
			hudRenderFrame();
            updateCanvas();
        }
    }

    return 0;
}
