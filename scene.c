#include "scene.h"
#include "renderer.h"
#include "assets.h"

int sceneAnimInc = 1;
int MAX_FRAMES = 5;

void sceneAnimateFrame() {
    sceneAnimInc = (sceneAnimInc == MAX_FRAMES ? 1 : sceneAnimInc + 1);
}

void sceneRenderFrame() {
    char frameFile[255];
    char* animGroupName;
    animGroupName = "scene-%02d.png";
    sprintf(frameFile, animGroupName, sceneAnimInc);

    Sprite sprite = makeSimpleSprite(frameFile);
    drawSprite(sprite, makeCoord(screenBounds.x/2, screenBounds.y/2));
}
