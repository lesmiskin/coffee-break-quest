#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include "mysdl.h"

typedef struct {
    double x, y;
} Coord;

typedef enum {
	MODE_TITLE,
	MODE_BREAK_INTRO,
	MODE_BREAK,
	MODE_BREAK_OOPS,
	MODE_COMBAT_INTRO,
	MODE_COMBAT_LOST,
	MODE_COMBAT_WON,
	MODE_OFFICE_INTRO,
	MODE_OFFICE,
	MODE_OFFICE_WON,
	MODE_OFFICE_LOST,
} GameMode;

typedef struct {
	double x, y;
	int width, height;
} Rect;

extern bool sceneInited;
extern double typeNo;
extern Rect makeRect(double x, double y, double width, double height);
extern bool inBounds(Coord point, Rect area);
extern Rect makeBounds(Coord origin, double width, double height);
extern Rect makeSquareBounds(Coord origin, double size);
extern GameMode currentMode;
extern void changeMode(GameMode newMode);
extern SDL_Window *window;
extern bool running;
extern Coord makeCoord(double x, double y);
extern Coord deriveCoord(Coord original, double xOffset, double yOffset);
extern Coord zeroCoord();
extern bool timer(long *lastTime, double hertz);
extern bool isDue(long now, long lastTime, double hertz);
extern void fatalError(const char *title, const char *message);
extern void quit(void);
extern char *combineStrings(const char *a, const char *b);
extern bool fileExists(const char *path);
extern int randomMq(int min, int max);
extern double sineInc(double offset, double *sineInc, double speed, double magnitude);
extern double cosInc(double offset, double *sineInc, double speed, double magnitude);
extern double getAngle(Coord a, Coord b);
extern Coord getStep(Coord a, Coord b, double speed, bool negativeMagic);
extern double randomNo;
extern double screenNo;
extern double itemNo;
extern bool chance(int probability);

#endif