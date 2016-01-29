#ifndef ASSETS_H
#define ASSETS_H

#include "SDL2_Image/SDL_image.h"
#include "SDL2_Mixer/SDL_mixer.h"

typedef struct {
    char* key;
    SDL_Texture* texture;
} Asset;

typedef struct {
    char* key;
    Mix_Chunk* sound;
} SoundAsset;

extern void initAssets(void);
extern SDL_Texture *getTexture(char *path);
extern Asset getAsset(char *path);
extern void shutdownAssets(void);
extern SoundAsset getSound(char *path);
extern void play(char* path);

#endif
