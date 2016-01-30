#include "meter.h"
#include "hud.h"
#include "renderer.h"
#include "time.h"
#include "scene.h"

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
	if(mode == MODE_OFFICE) {
		//Stat text.
		writeText(alertness, makeCoord(30, 10));
		writeText(hygiene, makeCoord(30, 20));
		writeText(bladder, makeCoord(30, 30));
		writeText(work, makeCoord(30, 40));
		writeText(timeProgress, makeCoord(30, 55));

		//Captions
		drawSprite(makeSimpleSprite("caff.png"), makeCoord(12, 10));
		drawSprite(makeSimpleSprite("hyg.png"), makeCoord(12, 20));
		drawSprite(makeSimpleSprite("blad.png"), makeCoord(12, 30));
		drawSprite(makeSimpleSprite("prod.png"), makeCoord(12, 40));
		drawSprite(makeSimpleSprite("grind.png"), makeCoord(11, 55));

		if(alertness <= 10){
			drawSprite(makeSimpleSprite("warning.png"), makeCoord(45, 10));
			writeFont("press d to drink", makeCoord(55,10));
			// warning + "press D to drink"
		}
		if(bladder >= 90){
			drawSprite(makeSimpleSprite("warning.png"), makeCoord(45, 30));
			writeFont("press b to break", makeCoord(55,30));
			// warning + "press B to break"
		}
		if(hygiene <= 40){
			drawSprite(makeSimpleSprite("warning.png"), makeCoord(45, 20));
			// warning
		}
		if(timeProgress >= 420){
			drawSprite(makeSimpleSprite("warning.png"), makeCoord(45, 55));
			writeFont("work day almost over", makeCoord(55,55));
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