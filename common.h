#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include "mysdl.h"

typedef struct {
    double x, y;
} Coord;

extern SDL_Window *window;
extern bool running;
extern Coord makeCoord(double x, double y);
extern Coord zeroCoord();
extern bool timer(long *lastTime, double hertz);
extern bool isDue(long now, long lastTime, double hertz);
extern void fatalError(const char *title, const char *message);
extern void quit(void);
extern char *combineStrings(const char *a, const char *b);
extern bool fileExists(const char *path);

#endif