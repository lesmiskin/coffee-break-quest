#ifndef HUD_H
#define HUD_H

#include "common.h"

typedef enum {
	PLUME_DRINK,
	PLUME_BREAK,
} PlumeType;

void spawnPlume(PlumeType type);
void initHud(void);
void writeText(int amount, Coord pos);
void hudRenderFrame(void);
void hudGameFrame(void);

#endif
