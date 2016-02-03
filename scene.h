#ifndef SCENE_H
#define SCENE_H

#include "common.h"
#include "renderer.h"

#include <stdbool.h>

#define MAX_PROPS 8

typedef struct {
    Sprite sprite;
    Coord coord;
} Prop;

extern void initOnceScene(void);
extern void sceneRenderFrame(void);
extern void sceneAnimateFrame(void);
extern void initScene();
extern double coffeeInc;
extern bool onBreak;
extern bool onDrink;
extern Prop props[MAX_PROPS];

#endif
