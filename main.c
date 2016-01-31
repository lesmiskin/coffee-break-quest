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

//{{{{{{{{{{
//If you get enough productivity - you win.
//If you take a break - that's the minigame (no shooting... use the mens?).
//If your hygene becomes low - that's the shooting minigame.
//}}}}}}}}}}

//COMBAT / LES

//*************************************************************

//REQUIRED
//TODO: Screen modes (title, end screen).						LES
//TODO: Health counter during combat (5 hits and dead).
//TODO: Trigger the break_intro mode when doing a break.
//TODO: Trigger the break mode when in break_intro mode.

//TODO: Randomised names using text.							EM
//TODO: Female icon on door a bit larger.						EM
//TODO: Trigger for door entry.									EM

//TODO: Windows builds etc.										LES
//TODO: Check sounds before release.
//TODO: Resolution calculator.
//TODO: Don't let the player use SPACE when non-aggro.



//TODO: Aggro, non-aggro upon breaks.							LES
//TODO: Health bar during combat.								LES
//TODO: Windows builds etc.
//TODO: Speed indicator.

//TODO: Cups disappear when hitting player.						EM

//POLISH
//TODO: Random placement on desk.
//TODO: Desk ornament animation.
//TODO: Door spawning in tighter region inside screen.
//TODO: Bug: Aggro crashes sometimes.
//TODO: Don't spawn enemies immediately next to Tim on combat screen.
//TODO: Don't spawn doors immediately next to Tim on combat screen.
//TODO: Enemies should bounce off each other.
//TODO: Enemies should bounce off tim (and just throw things).


//TODO: More character names.














//TODO: B&W triggers:
	//Titlescreen: COFFEE BREAK QUEST title screen, press SPACE to play.
		//Gain maximum productivity before the workday ends! TIP: Don't forget to take breaks.
		//HINT: There's more than one way to win the game...
	//Ladies door trigger: "Umm, what are you doing, you have a reputation to uphold around here you know..."
	//Combat intro: "HYGENE ALERT" - Your collegues have become disgusted with your lack of hygene and have banded together
		//to destroy you. How will you defend yourself? (HINT: Press spacebar).
	//Goal: Push all your collegues offscreen to win (SPACE).
	//Break message (once only): It's time to use the restroom (Use the arrows).


//TODO: Office chars should change facing direction based on last movement (-'ve of target).
//TODO: Shading for characters?
//TODO: Coffee icon for application.


//LES: BIG
//TODO: Once killed, game loops back to start.
//TODO: LES - Tutorial text.
//TODO: LES - GAME OVER screens.
//TODO: Enemies cause GAME OVER state (b&w background).

//LES: EASY
//TODO: Health bar (icons).
//TODO: Coffee cups cause "hit" sound.
//TODO: Tapping on keyboard sound effect.
//TODO: Detect resolution and size accordingly.
//TODO: Review sounds.
//TODO: Coffee cups cause "hit" graphic on player.
//TODO: Klaxon sound for warning icons.



//POLISH
//TODO: Smoothing out animations.
//TODO: Retail distribution on Linux, Mac, and Windows.

//IF TIME
//TODO: Something should happen when enemy bumps up against the player?
//TODO: Bounds check for player in arcade scene.
//TODO: Cheat code to force dismemberment (Konami code - learning is FUN!).
//TODO: Momentum on Tim.
//TODO: Random coffee cup thrown from unknown assailant.
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
    screenNo = randomMq(0,9);
    itemNo = randomMq(0,9);
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
			playerShadowFrame();
			enemyShadowRenderFrame();
			playerRenderFrame();
            enemyRenderFrame();
			enemyNameRenderFrame();
			hudRenderFrame();
            updateCanvas();
        }
    }

    return 0;
}
