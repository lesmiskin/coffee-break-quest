#include "hud.h"
#include "renderer.h"

static Sprite letters[10];
static const int LETTER_WIDTH = 4;

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

void hudRenderFrame(void) {
	writeText(123, makeCoord(50,50));
}
