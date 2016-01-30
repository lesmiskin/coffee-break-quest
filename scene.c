#include "scene.h"
#include "renderer.h"
#include "assets.h"

double bobInc = 1;
double bob2Inc = 1;
bool bobDir = false;
bool bob2Dir = false;

void sceneAnimateFrame() {
    if(bobDir && bobInc > 1) {
        bobDir = false;
    }else if(!bobDir && bobInc < 0){
        bobDir = true;
    }

    if(bob2Dir && bob2Inc > 2) {
        bob2Dir = false;
    }else if(!bob2Dir && bob2Inc < 1){
        bob2Dir = true;
    }

    if(bobDir) {
        bobInc += 0.5;
    }else{
        bobInc -= 0.5;
    }

    if(bob2Dir) {
        bob2Inc += 0.5;
    }else{
        bob2Inc -= 0.5;
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
    drawSprite(body, makeCoord(screenBounds.x/2 - xOffset, screenBounds.y/2 + yOffset - bob2Inc));
    Sprite chair2 = makeSimpleSprite("chair-top.png");
    drawSprite(chair2, makeCoord(screenBounds.x/2 - xOffset, screenBounds.y/2 + yOffset));
    Sprite head = makeSimpleSprite("head-black.png");
    drawSprite(head, makeCoord(screenBounds.x/2 - xOffset, screenBounds.y/2 + yOffset + 1 - bobInc));

    Sprite coffee = makeSimpleSprite("coffee.png");
    drawSprite(coffee, makeCoord(235, screenBounds.y/2 + 5));
}
