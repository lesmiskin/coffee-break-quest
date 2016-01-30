#include "enemy.h"
#include "player.h"
#include "renderer.h"
#include "assets.h"
#include "time.h"

//TODO: Throw/heil animation.
//TODO: Cups vanish upon hit.

//TODO: Projectiles push enemies back.

//TODO: Cups impact player health.
//TODO: Projectiles reverse direction of cups?


typedef struct {
	Coord coord;
	Coord target;
	double angle;
} Cup;

typedef enum {
	STATE_CALMSTEP,
	STATE_THROW,
	STATE_ALARMED
} EnemyState;	//of the...

typedef struct {
	Coord coord;
	EnemyState state;
	long stateTime;
	long lastThrow;
	int nextThrow;
	int animInc;
} Enemy;

#define MAX_ENEMY 12
#define MAX_CUPS 50
#define WALK_ANIMS 4	//We play each frame twice.
Enemy enemies[MAX_ENEMY];
Cup cups[MAX_CUPS];
int cupInc = 0;
int throwMin = 1500;
int throwMax = 3500;
int throwTime = 250;

const double CUP_SPEED = 1.3;
const double ENEMY_SPEED = 0.3;

void enemyGameFrame(void) {
	if(mode != MODE_COMBAT) return;

	for(int i=0; i < MAX_ENEMY; i++) {
		if(enemies[i].coord.x == 0) continue;

		//TODO: Enemy wiggling.

		//Move the enemy towards the player.
		Coord homeStep = getStep(pos, enemies[i].coord, ENEMY_SPEED, true);
		Coord cupStep = getStep(pos, enemies[i].coord, CUP_SPEED, true);
		enemies[i].coord.x += homeStep.x;
		enemies[i].coord.y += homeStep.y;

		//Throw cup.
		if(timer(&enemies[i].lastThrow, enemies[i].nextThrow)) {
			cupInc = (cupInc == MAX_CUPS ? 0 : cupInc + 1);

			Cup cup = { enemies[i].coord, cupStep, random(0, 360) };
			cups[cupInc] = cup;
			enemies[i].lastThrow = clock();
			enemies[i].state = STATE_THROW;
		}
		//Reset throwing state.
		else if(enemies[i].state == STATE_THROW && timer(&enemies[i].lastThrow, throwTime)) {
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

void enemyRenderFrame(void){
	if(mode != MODE_COMBAT) return;

	//Draw the enemies with the right animation frame.
	for(int i=0; i < MAX_ENEMY; i++) {
		if(enemies[i].coord.x == 0) continue;

		Sprite sprite;

		//Choose right animation to show.
		if(enemies[i].state == STATE_THROW) {
			sprite = makeFlippedSprite("steve-aggro.png", shouldFace(enemies[i].coord));
		}else{
			if(enemies[i].animInc >= 2) {
				sprite = makeFlippedSprite("steve-calmstep.png", shouldFace(enemies[i].coord));
			}else{
				sprite = makeFlippedSprite("steve-calm.png", shouldFace(enemies[i].coord));
			}
		}

		drawSprite(sprite, enemies[i].coord);
	}

	//Rotate the cups.
	for(int i=0; i < MAX_CUPS; i++) {
		if (cups[i].coord.x == 0) continue;
		cups[i].angle = cups[i].angle >= 360 ? 0 : cups[i].angle + 2;
		Sprite sprite = makeSimpleSprite("coffee.png");
		drawSpriteFull(sprite, cups[i].coord, 0.75, cups[i].angle);
	}
}

void initEnemy(void) {
	//Make the enemies
	for(int i=0; i < 6; i++) {
		Enemy enemy = {
			makeCoord(
				randomMq(1, 320),
				randomMq(1, 240)
			),
			STATE_CALMSTEP,
			clock(),
			clock(),
			chance(50) ? throwMin : throwMax,
			random(0, WALK_ANIMS-1)
		};

		enemies[i] = enemy;
	}

	//Clear cups.
	memset(cups, 0, sizeof(cups));
}
