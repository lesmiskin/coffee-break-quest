#ifndef HUD_H
#define HUD_H

#include "common.h"

typedef enum {
	PLUME_DRINK,
	PLUME_BREAK,
} PlumeType;

extern void spawnPlume(PlumeType type);
extern void initHud(void);
extern void writeText(int amount, Coord pos);
extern void writeFont(char *text, Coord pos);
extern void hudRenderFrame(void);
extern void hudGameFrame(void);

#endif
