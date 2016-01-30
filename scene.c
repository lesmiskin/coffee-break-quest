#include "scene.h"
#include "renderer.h"
#include "assets.h"

double bobHeadInc = 1;
double bobBodyInc = 1;
bool bobDir = false;
bool bob2Dir = false;

//double coffeeInc = 0;
//double coffeePos = 0;

void sceneAnimateFrame() {

//    coffeePos = sineInc(100, &coffeeInc, 0.05, 32);

    if(bobDir && bobHeadInc > 1) {
        bobDir = false;
    }else if(!bobDir && bobHeadInc < 0){
        bobDir = true;
    }

    if(bob2Dir && bobBodyInc > 2) {
        bob2Dir = false;
    }else if(!bob2Dir && bobBodyInc < 1){
        bob2Dir = true;
    }

    if(bobDir) {
        bobHeadInc += 0.5;
    }else{
        bobHeadInc -= 0.5;
    }

    if(bob2Dir) {
        bobBodyInc += 0.5;
    }else{
        bobBodyInc -= 0.5;
    }
}

void sceneRenderFrame() {
    int xOffset = 3;
    int yOffset = 30;
    
    Sprite sprite = makeSimpleSprite("office-bg.png");
    drawSprite(sprite, makeCoord(screenBounds.x/2, screenBounds.y/2));
    Sprite chair1 = makeSimpleSprite("chair-bottom.png");
    drawSprite(chair1, makeCoord(screenBounds.x/2 - xOffset, screenBounds.y/2 + yOffset));
    Sprite body = makeSimpleSprite("body-yellow.png");
    drawSprite(body, makeCoord(screenBounds.x/2 - xOffset, screenBounds.y/2 + yOffset - bobBodyInc));
    Sprite chair2 = makeSimpleSprite("chair-top.png");
    drawSprite(chair2, makeCoord(screenBounds.x/2 - xOffset, screenBounds.y/2 + yOffset));
    Sprite head = makeSimpleSprite("head-black.png");
    drawSprite(head, makeCoord(screenBounds.x/2 - xOffset, screenBounds.y/2 + yOffset + 1 - bobHeadInc));

    Sprite coffee = makeSimpleSprite("coffee.png");
    drawSprite(coffee, makeCoord(235/*ceil(coffeePos)*/, screenBounds.y/2 + 5));
}
