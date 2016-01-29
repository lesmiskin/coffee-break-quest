#include "common.h"
#include <stdbool.h>
#include <SDL.h>
#include <time.h>
#include <unistd.h>

static const double RADIAN_CIRCLE = 6.28;

long ticsToMilliseconds(long tics) {
    //we want the duration version of the platform-independent seconds, so we / 1000.
    long platformAgnosticMilliseconds = CLOCKS_PER_SEC / 1000;

    return tics / platformAgnosticMilliseconds;
}

bool isDue(long now, long lastTime, double hertz) {
    long timeSinceLast = ticsToMilliseconds(now - lastTime);
    return timeSinceLast >= hertz;
}

bool timer(long *lastTime, double hertz){
    long now = clock();
    if(isDue(now, *lastTime, hertz)) {
        *lastTime = now;
        return true;
    }else{
        return false;
    }
}

void fatalError(const char *title, const char *message) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        title,
        message,
        window
    );
    SDL_Quit();
}

void quit(void) {
    running = false;
}

char *combineStrings(const char *a, const char *b) {
    //Allocate exact amount of space necessary to hold the two strings.
    char *result = malloc(strlen(a) + strlen(b) + 1);		//+1 for the zero-terminator
    strcpy(result, a);
    strcat(result, b);

    return result;
}

bool fileExists(const char *path) {
    return access(path, R_OK ) == 0;
}
