#include "meter.h"
#include "hud.h"
#include "renderer.h"
#include "time.h"

static Sprite letters[10];
static const int LETTER_WIDTH = 4;

typedef struct {
	PlumeType type;
	Coord origin;
	long spawnTime;
} Plume;

Plume plume;

void spawnPlume(PlumeType type) {
	Plume thisPlume = {
		type,
		makeCoord(100, 100),
		clock()
	};

	plume = thisPlume;
}

void initHud(void) {
	//Pre-load font sprites.
	for(int i=0; i < 10; i++) {
		char textureName[50];
		sprintf(textureName, "font-%02d.png", i);
		letters[i] = makeSimpleSprite(textureName);
	}
}

void writeText(int amount, Coord pos) {
	if(amount == 0) {
		drawSprite(letters[0], pos);
	}else{
		while(amount != 0) {
			drawSprite(letters[amount % 10], pos);
			amount /= 10;
			pos.x -= LETTER_WIDTH;
		}
	}
}

void hudGameFrame(void) {
	if(plume.origin.x > 0) {
		plume.origin.y -= 2;
	}
}

void hudRenderFrame(void) {
	writeText(alertness, makeCoord(10, 10));
	writeText(hygiene, makeCoord(10, 20));
	writeText(alertness, makeCoord(10, 30));
	writeText(work, makeCoord(10, 40));
	writeText(bladder, makeCoord(10, 50));
	writeText(timeProgress, makeCoord(10, 70));

	if(plume.origin.x > 0) {
		if(timer(&plume.spawnTime, 500)) {
			plume.origin.x = 0;
			return;
		}else{
			Sprite sprite = makeSimpleSprite("drink.png");
			drawSprite(sprite, makeCoord(screenBounds.x/2, plume.origin.y));
		}
	}
}