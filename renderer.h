#ifndef RENDERER_H
#define RENDERER_H

#include "mysdl.h"
#include "common.h"

extern SDL_Renderer *renderer;
extern SDL_Texture *renderBuffer;
extern Coord screenBounds;
extern Coord pixelGrid;
extern Coord screenBounds;
extern void initRenderer();
extern void shutdownRenderer();

typedef struct {
    SDL_Texture *texture;
    Coord offset;
    Coord size;
    SDL_RendererFlip flip;
} Sprite;

#endif