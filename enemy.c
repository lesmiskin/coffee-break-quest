#include "enemy.h"
#include "player.h"
#include "renderer.h"
#include "assets.h"
#include "time.h"
#include "hud.h"

//TODO: Cups vanish upon hit.
//TODO: Cups impact player health.
//TODO: Projectiles reverse direction of cups?



typedef enum {
	STATE_CALMSTEP,
	STATE_THROW,
	STATE_ALARMED
} EnemyState;	//of the...

typedef enum {
	NAME_DWIGHT,
	NAME_DILBERT,
	NAME_STEVE,
	NAME_BILL,
	NAME_OWEN,
	NAME_JAMES,
	NAME_JOHN,
	NAME_ROB,
	NAME_MIKE,
	NAME_WILL,
	NAME_DAVID,
	NAME_RICHARD,
	NAME_CHARLES,
	NAME_JOE,
	NAME_CHRIS,
	NAME_DANIEL,
	NAME_PAUL,
	NAME_MARK,
	NAME_DONALD,
	NAME_GEORGE,
	NAME_KEN,
	NAME_EDWARD,
	NAME_BRIAN,
} EnemyName;

typedef enum {
	TYPE_DWIGHT,
	TYPE_DILBERT,
	TYPE_STEVE,
} EnemyType;

typedef struct {
	Coord coord;
	EnemyState state;
	long stateTime;
	long lastThrow;
	int nextThrow;
	int animInc;
	EnemyType type;
	EnemyName name;
	Coord idleTarget;
	long lastDir;
	int nextDir;
} Enemy;

#define MAX_ENEMY 12
#define WALK_ANIMS 4	//We play each frame twice.
Enemy enemies[MAX_ENEMY];
Cup cups[MAX_CUPS];
int cupInc = 0;
int throwMin = 1500;
int throwMax = 3500;
int throwTime = 250;
int xOffset = -7;
bool aggro = true;

const double CUP_SPEED = 1.3;
const double ENEMY_SPEED = 0.3;

bool onScreen(Coord coord, double threshold) {
	return inBounds(coord, makeRect(
			0 + threshold,
			0 + threshold,
			screenBounds.x - (threshold),
			screenBounds.y - (threshold)
	));
}

bool hasWon(void) {
	//If any enemies are in the screen bounds - we've still got work to do!
	for(int i=0; i < MAX_ENEMY; i++) {
		if(enemies[i].coord.x == 0 && enemies[i].coord.y == 0) continue;

		if(onScreen(enemies[i].coord, 0)) {
			return false;
		}
	}

	//Everyone out? OK, you won.
	return true;
}

Coord randomDir(Coord coord) {
	return deriveCoord(coord, randomMq(-20, 20), randomMq(-20, 20));
}

void enemyGameFrame(void) {
	if(mode != MODE_COMBAT) return;

	//Everyone put offscreen? Then win.
	if(hasWon()) {
		changeMode(MODE_COMBAT_WON);
	}

	//Did the player go offscreen? Switch back to our desk, then.
	if(!onScreen(pos, 0)) {
//		changeMode();
	}

	for(int i=0; i < MAX_ENEMY; i++) {
		if(enemies[i].coord.x == 0) continue;

		//Shot collision detection.
		for(int j=0; j < MAX_SHOTS; j++) {
			if(inBounds(shots[j].coord, makeSquareBounds(enemies[i].coord, 20))) {
				Coord step = getStep(pos, enemies[i].coord, 1, true);
				enemies[i].coord.x -= step.x;
				enemies[i].coord.y -= step.y;
				enemies[i].state = STATE_ALARMED;
				//aggro = true; // Messes with door collision in non-combat mode
			}
		}

		if(!aggro) {
			Coord homeStep;

			//Change idle target if it's time to, or if he's near the edge of the screen.
			if (timer(&enemies[i].lastDir, enemies[i].nextDir)) {
				enemies[i].idleTarget = randomDir(enemies[i].coord);
			//Retarget enemies to the middle of the screen if they're going offscreen of their own volition.
			}else if(!onScreen(enemies[i].coord, 10)) {
				if(enemies[i].coord.x > screenBounds.x/2) {
					enemies[i].idleTarget = deriveCoord(enemies[i].coord, 20, 0);
				}else{
					enemies[i].idleTarget = deriveCoord(enemies[i].coord, -20, 0);
				}
				if(enemies[i].coord.y > screenBounds.y/2) {
					enemies[i].idleTarget = deriveCoord(enemies[i].coord, 0, 20);
				}else{
					enemies[i].idleTarget = deriveCoord(enemies[i].coord, 0, -20);
				}
			}

			homeStep = getStep(enemies[i].coord, enemies[i].idleTarget, ENEMY_SPEED, true);

			//Move around.
			enemies[i].coord.x += homeStep.x;
			enemies[i].coord.y += homeStep.y;

		} else {
			//Move the enemy towards the player.
			Coord homeStep = getStep(pos, enemies[i].coord, ENEMY_SPEED, true);
			Coord cupStep = getStep(pos, enemies[i].coord, CUP_SPEED, true);
			enemies[i].coord.x += homeStep.x;
			enemies[i].coord.y += homeStep.y;

			//Throw cup.
			if (onScreen(enemies[i].coord, 10) && timer(&enemies[i].lastThrow, enemies[i].nextThrow)) {
				cupInc = (cupInc == MAX_CUPS ? 0 : cupInc + 1);

				Cup cup = {enemies[i].coord, cupStep, randomMq(0, 360)};
				cups[cupInc] = cup;
				enemies[i].lastThrow = clock();
				enemies[i].state = STATE_THROW;
			}
				//Reset throwing state.
			else if (enemies[i].state == STATE_THROW && timer(&enemies[i].lastThrow, throwTime)) {
				enemies[i].state = STATE_CALMSTEP;
			}
		}
		if (enemies[i].state == STATE_ALARMED && enemies[i].animInc == 3) {
			enemies[i].state = STATE_CALMSTEP;
		}
	}

	//Move the cups towards the player.
	for(int i=0; i < MAX_CUPS; i++) {
		if (cups[i].coord.x == 0) continue;
		cups[i].coord.x += cups[i].target.x;
		cups[i].coord.y += cups[i].target.y;
	}
}

void enemyAnimateFrame(void) {
	//Animate the enemies
	for(int i=0; i < MAX_ENEMY; i++) {
		if(enemies[i].coord.x == 0) continue;

		//Increment animations.
		if(enemies[i].animInc < 4) {
			enemies[i].animInc++;
		}else{
			enemies[i].animInc = 0;
		}
	}
}

SDL_RendererFlip shouldFace(Coord npc) {
	return pos.x < npc.x ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
}

void enemyNameRenderFrame(void) {
	if(mode != MODE_COMBAT) return;
	for(int i=0; i < MAX_ENEMY; i++) {
		if (enemies[i].coord.x == 0) continue;
		Sprite nameSprite;

		switch(enemies[i].name) {
			case NAME_BILL:
				nameSprite = makeSimpleSprite("name-bill.png");
				drawSprite(nameSprite, deriveCoord(enemies[i].coord, 0, -31));
				break;
			case NAME_DILBERT:
				nameSprite = makeSimpleSprite("name-dilbert.png");
				drawSprite(nameSprite, deriveCoord(enemies[i].coord, 0, -31));
				break;
			case NAME_OWEN:
				nameSprite = makeSimpleSprite("name-owen.png");
				drawSprite(nameSprite, deriveCoord(enemies[i].coord, 0, -31));
				break;
			case NAME_STEVE:
				nameSprite = makeSimpleSprite("name-steve.png");
				drawSprite(nameSprite, deriveCoord(enemies[i].coord, 0, -31));
				break;
			case NAME_DWIGHT:
				nameSprite = makeSimpleSprite("name-dwight.png");
				drawSprite(nameSprite, deriveCoord(enemies[i].coord, 0, -31));
				break;
			case NAME_JAMES:
				writeFont("james", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_BRIAN:
				writeFont("brian", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_EDWARD:
				writeFont("edward", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_KEN:
				writeFont("ken", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_GEORGE:
				writeFont("george", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_DONALD:
				writeFont("donald", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_MARK:
				writeFont("mark", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_JOHN:
				writeFont("john", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_ROB:
				writeFont("rob", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_MIKE:
				writeFont("mike", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_WILL:
				writeFont("will", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_DAVID:
				writeFont("david", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_RICHARD:
				writeFont("richard", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_CHARLES:
				writeFont("charles", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_JOE:
				writeFont("joe", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_CHRIS:
				writeFont("chris", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_DANIEL:
				writeFont("daniel", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
			case NAME_PAUL:
				writeFont("paul", deriveCoord(enemies[i].coord, xOffset, -31));
				break;
		}
	}
}

void enemyShadowRenderFrame(void) {
	if (mode != MODE_COMBAT) return;

	//Draw the enemies with the right animation frame.
	for (int i = 0; i < MAX_ENEMY; i++) {
		if (enemies[i].coord.x == 0) continue;
		Sprite shadow = makeSimpleSprite("shadow.png");
		drawSprite(shadow, deriveCoord(enemies[i].coord, 0, 21));
	}
}

void enemyRenderFrame(void){
	if(mode != MODE_COMBAT) return;

	//Draw the enemies with the right animation frame.
	for(int i=0; i < MAX_ENEMY; i++) {
		if(enemies[i].coord.x == 0) continue;

		Sprite sprite;

		//Ugh... Time constraints!
		switch(enemies[i].type) {
			case TYPE_DILBERT:
				if(enemies[i].state == STATE_THROW)
					sprite = makeFlippedSprite("dilbertaggro.png", shouldFace(enemies[i].coord));
				else
				if(enemies[i].state == STATE_ALARMED)
					sprite = makeFlippedSprite("dilbertalarmed.png", shouldFace(enemies[i].coord));
				else
				if(enemies[i].animInc >= 2)
					sprite = makeFlippedSprite("dilbertcalmstep.png", shouldFace(enemies[i].coord));
				else
					sprite = makeFlippedSprite("dilbertcalm.png", shouldFace(enemies[i].coord));
				break;
			case TYPE_DWIGHT:
				if(enemies[i].state == STATE_THROW)
					sprite = makeFlippedSprite("dwightaggro.png", shouldFace(enemies[i].coord));
				else
				if(enemies[i].state == STATE_ALARMED)
					sprite = makeFlippedSprite("dwightalarmed.png", shouldFace(enemies[i].coord));
				else
				if(enemies[i].animInc >= 2)
					sprite = makeFlippedSprite("dwightcalmstep.png", shouldFace(enemies[i].coord));
				else
					sprite = makeFlippedSprite("dwightcalm.png", shouldFace(enemies[i].coord));
				break;
			case TYPE_STEVE:
				if(enemies[i].state == STATE_THROW)
					sprite = makeFlippedSprite("steveaggro.png", shouldFace(enemies[i].coord));
				else
				if(enemies[i].state == STATE_ALARMED)
					sprite = makeFlippedSprite("stevealarmed.png", shouldFace(enemies[i].coord));
				else
				if(enemies[i].animInc >= 2)
					sprite = makeFlippedSprite("stevecalmstep.png", shouldFace(enemies[i].coord));
				else
					sprite = makeFlippedSprite("stevecalm.png", shouldFace(enemies[i].coord));
				break;
		}

		drawSprite(sprite, enemies[i].coord);
	}

	//Rotate the cups.
	for(int i=0; i < MAX_CUPS; i++) {
		if (cups[i].coord.x == 0) continue;
		cups[i].angle = cups[i].angle >= 360 ? 0 : cups[i].angle + 2;
		Sprite sprite = makeSimpleSprite("coffee-small.png");
		drawSpriteFull(sprite, cups[i].coord, 1, cups[i].angle);
	}
}

void initEnemy(void) {
	//Make the enemies
	for(int i=0; i < 6; i++) {
		Coord c = makeCoord(
				randomMq(75, 320),
				randomMq(75, 240)
		);

		Enemy enemy = {
			c,
			STATE_CALMSTEP,
			clock(),
			clock(),
			chance(50) ? throwMin : throwMax,
			randomMq(0, WALK_ANIMS-1),
			(EnemyType)randomMq(0, 2),
			(EnemyName)randomMq(0, 22),
			randomDir(c),
			clock(),
			1000
		};

		enemies[i] = enemy;
	}

	//Clear cups.
	memset(cups, 0, sizeof(cups));
}
