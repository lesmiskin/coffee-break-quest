#include "scene.h"
#include "renderer.h"
#include "assets.h"

void sceneRenderFrame() {
    Sprite s = makeSprite(getTexture("scene.png"), zeroCoord(), SDL_FLIP_NONE);
    drawSprite(s, makeCoord(screenBounds.x/2, screenBounds.y/2));
}
