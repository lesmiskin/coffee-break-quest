#include "scene.h"
#include "renderer.h"
#include "assets.h"

void sceneRenderFrame() {
    Sprite s = makeSimpleSprite("scene.png");
    drawSprite(s, makeCoord(screenBounds.x/2, screenBounds.y/2));
}
