#ifndef ENEMY_H
#define ENEMY_H

#include "common.h"

#define MAX_CUPS 50

typedef struct {
    Coord coord;
    Coord target;
    double angle;
} Cup;

extern void enemyGameFrame(void);
extern void enemyRenderFrame(void);
extern void enemyNameRenderFrame(void);
extern void initEnemy(void);
extern void enemyAnimateFrame(void);
extern void enemyShadowRenderFrame(void);
extern bool aggro;
extern Cup cups[MAX_CUPS];

#endif
