#ifndef SCENE_H
#define SCENE_H

#include <stdbool.h>

extern void sceneRenderFrame(void);
extern void sceneAnimateFrame(void);
extern void initScene();
extern double coffeeInc;
extern bool onBreak;
extern bool onDrink;

#endif
