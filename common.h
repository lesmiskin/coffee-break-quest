#include <SDL.h>
#include <stdbool.h>

typedef struct {
    double x, y;
} Coord;

extern SDL_Window *window;
extern bool running;
extern Coord makeCoord(double x, double y);
extern bool timer(long *lastTime, double hertz);
extern bool isDue(long now, long lastTime, double hertz);
extern void fatalError(const char *title, const char *message);
extern void quit(void);
