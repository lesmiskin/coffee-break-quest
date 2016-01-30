#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

#define MAX_SHOTS 100

typedef struct {
	Coord coord;
	Coord target;
	double inc;
} Shot;

extern void playerGameFrame(void);
extern void playerRenderFrame(void);
extern void playerShadowFrame(void);
extern void shoot(void);
extern void initPlayer();
extern Coord pos;
extern void playerAnimateFrame(void);
extern Shot shots[255];

#endif
