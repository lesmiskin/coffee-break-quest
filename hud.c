#include "meter.h"
#include "hud.h"
#include "renderer.h"
#include "time.h"
#include "scene.h"
#include "enemy.h"
#include "player.h"

static Sprite letters[10];
static const int LETTER_WIDTH = 4;

bool hudFlash = false;

typedef struct {
	PlumeType type;
	Coord origin;
	long spawnTime;
} Plume;

Plume plume;

void hudAnimFrame() {
	hudFlash = !hudFlash;
}

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

void writeFont(char *text, Coord pos) {
	for(int i=0; i < strlen(text); i++) {
		//Print text if it's not a space.
		if(text[i] != ' ') {

			char fontFile[50];

			if(text[i] == '!') {
				sprintf(fontFile, "font-bang.png");
			}else{
				sprintf(fontFile, "font-%c.png", text[i]);
			}

			Sprite sprite = makeSimpleSprite(fontFile);
			drawSprite(sprite, makeCoord(pos.x, pos.y));

			if(text[i] == 'q') {
				pos.x += 4;
			}else if(text[i] == 'w' || text[i] == 'm') {
				pos.x += 5;
			}else if(text[i] == 'o') {
				pos.x += 5;
			}else if(text[i] == 'u') {
				pos.x += 5;
			}else if(text[i] == 'i' || text[i] == 'e') {
				pos.x += 3;
			}else{
				pos.x += sprite.size.x - 1;
			}
		}else{
			pos.x += 2;
		}
	}


//	drawSprite(makeSimpleSprite("chair-bottom.png"), makeCoord(50, 50));
//	for(int i=0; i < 10; i++) {
//	}
//
//
//	if(amount == 0) {
//		drawSprite(letters[0], pos);
//	}else{
//		while(amount != 0) {
//			drawSprite(letters[amount % 10], pos);
//			amount /= 10;
//			pos.x -= LETTER_WIDTH;
//		}
//	}
}

void hudGameFrame(void) {
	if(plume.origin.x > 0) {
		plume.origin.y -= 2;
	}
}

void hudRenderFrame(void) {
	if(aggro && mode == MODE_COMBAT) {
		writeText(health, makeCoord(50, 10));
		writeFont("health", makeCoord(10, 10));
	}

	if(mode == MODE_OFFICE) {
		//Stat text.
		writeText(alertness, makeCoord(75, 10));
		writeText(hygiene, makeCoord(75, 20));
		writeText(bladder, makeCoord(75, 30));
		writeText(work, makeCoord(75, 40));

		//Captions
		writeFont("caffiene", makeCoord(12, 10));
		writeFont("hygiene", makeCoord(12, 20));
		writeFont("bladder", makeCoord(12, 30));
		writeFont("productivity", makeCoord(12, 40));

		if(alertness <= 10 && hudFlash){
			drawSprite(makeSimpleSprite("warning.png"), makeCoord(95, 10));
			writeFont("press d to drink", makeCoord(105,10));
			// warning + "press D to drink"
		}
		if(bladder >= 90 && hudFlash){
			drawSprite(makeSimpleSprite("warning.png"), makeCoord(95, 30));
			writeFont("press b to break", makeCoord(105,30));
			// warning + "press B to break"
		}
		if(hygiene <= 40 && hudFlash){
			drawSprite(makeSimpleSprite("warning.png"), makeCoord(95, 20));
			// warning
		}
		if(timeProgress >= 420 && hudFlash){
			drawSprite(makeSimpleSprite("warning.png"), makeCoord(10, 55));
			writeFont("work day almost over", makeCoord(20,55));
			// warning + "day almost over"
		}

		//Draw plumes.
		if(plume.origin.x > 0) {
			if(timer(&plume.spawnTime, 500)) {
				plume.origin.x = 0;
				return;
			}else{
				Sprite sprite = makeSimpleSprite(plume.type == PLUME_BREAK ? "break.png" : "drink.png");
				drawSprite(sprite, makeCoord(screenBounds.x/2, plume.origin.y));
			}
		}
	}
}