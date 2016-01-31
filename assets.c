#include <assert.h>
#include "assets.h"
#include "renderer.h"

typedef struct {
    char* filename;
} AssetDef;

typedef struct {
    char* filename;
    int volume;
} SoundDef;

static char* assetPath;
static Asset *assets;
static int assetCount;
static SoundAsset *sounds;
static int soundCount;

SoundAsset getSound(char *path) {
    //Loop through register until key is found, or we've exhausted the array's iteration.
    for(int i=0; i < soundCount; i++) {
        if(strcmp(sounds[i].key, path) == 0)			//if strings match.
            return sounds[i];
    }

    fatalError("Could not find Asset in register", path);
}

static Asset makeAsset(AssetDef definition) {
    assert(renderer != NULL);

    char *absPath = combineStrings(assetPath, definition.filename);
    //Check existence on file system.
    if(!fileExists(absPath))
        fatalError("Could not find Asset on disk", absPath);

    //Load file from disk.
    SDL_Surface *original = IMG_Load(absPath);
    free(absPath);

    Asset asset = {	definition.filename	};

    //Build initial texture and assign.
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, original);

    asset.texture = texture;

    //Free up RAM used for original surface object.
    free(original);

    return asset;
}

SDL_Texture *getTexture(char *path) {
    return getAsset(path).texture;
}
Asset getAsset(char *path) {
    //Loop through register until key is found, or we've exhausted the array's iteration.
    for(int i=0; i < assetCount; i++) {
        if(strcmp(assets[i].key, path) == 0)			//if strings match.
            return assets[i];
    }

    fatalError("Could not find Asset in register", path);
}

void shutdownAssets(void) {
    free(assetPath);
    free(assets);

    for(int i=0; i < soundCount; i++) Mix_FreeChunk(sounds[i].sound);

    free(sounds);
}

static void loadImages(void) {
    //Define assets to be loaded.
    AssetDef definitions[] = {
        { "scene-01.png" },
        { "scene-02.png" },
        { "scene-02.png" },
        { "scene-03.png" },
        { "scene-04.png" },
        { "scene-05.png" },
        { "font-00.png" },
        { "font-01.png" },
        { "font-02.png" },
        { "font-03.png" },
        { "font-04.png" },
        { "font-05.png" },
        { "font-06.png" },
        { "font-07.png" },
        { "font-08.png" },
        { "font-09.png" },
        { "font-x.png" },
        { "drink.png" },
        { "break.png" },
        { "prod.png" },
        { "blad.png" },
        { "hyg.png" },
        { "caff.png" },
        { "grind.png" },
        { "body-yellow.png" },
        { "bodyyellowdrink.png" },
        { "head-black.png" },
        { "arm.png" },
        { "armdrink.png" },
        { "chair-bottom.png" },
        { "chair-top.png" },
        { "coffee.png" },
        { "coffee-small.png" },
        { "energydrink.png" },
        { "office-bg.png" },
		{ "combat-player.png" },
		{ "combat-collegue.png" },
		{ "shot.png" },
        { "chair-bottom-1.png" },
        { "carpet.png" },
        { "shadow.png" },
        { "good-aim.png" },

        { "dilbertaggro.png" },
        { "dilbertalarmed.png" },
        { "dilbertcalm.png" },
        { "dilbertcalmstep.png" },
        { "dwightaggro.png" },
        { "dwightalarmed.png" },
        { "dwightcalm.png" },
        { "dwightcalmstep.png" },
        { "stevecalm.png" },
        { "stevecalmstep.png" },
        { "steveaggro.png" },
        { "stevealarmed.png" },

        { "clockoff.png" },
        { "clockon.png" },
        { "lamp.png" },
        { "plant.png" },
        { "newtonscradle.png" },
        { "rubikscube.png" },
        { "rubikscubesolved.png" },
        { "tree.png" },
        { "timcalm.png" },
        { "timcalmstep.png" },
        { "timalarmed.png" },
        { "cactus.png" },
        { "leg.png" },
        { "cat.png" },
        { "cat1.png" },
        { "clockface.png" },
        { "minutehand.png" },
        { "hourhand.png" },
        { "doormale.png" },
        { "doorfemale.png" },
        { "bsod.png" },
        { "screenblank.png" },
        { "screentext.png" },
        { "warning.png" },


        { "name-owen.png" },
        { "name-dilbert.png" },
        { "name-dwight.png" },
        { "name-bill.png" },
        { "name-steve.png" },

        { "font-a.png" },
        { "font-b.png" },
        { "font-c.png" },
        { "font-d.png" },
        { "font-e.png" },
        { "font-f.png" },
        { "font-g.png" },
        { "font-h.png" },
        { "font-i.png" },
        { "font-j.png" },
        { "font-k.png" },
        { "font-l.png" },
        { "font-m.png" },
        { "font-n.png" },
        { "font-o.png" },
        { "font-p.png" },
        { "font-q.png" },
        { "font-r.png" },
        { "font-s.png" },
        { "font-t.png" },
        { "font-u.png" },
        { "font-v.png" },
        { "font-w.png" },
        { "font-x.png" },
        { "font-y.png" },
        { "font-z.png" },
        { "font-bang.png" },

        { "title-fail.png" },
        { "title-winning.png" },
        { "title-coffee-break-quest.png" },
        { "title-coffee-break-quest-1.png" },
        { "title-uhh.png" },
        { "title-hai.png" },
        { "title-war.png" },
    };

    //Infer asset path from current directory.
    char* workingPath = SDL_GetBasePath();
    char assetsFolder[] = "assets/";
    assetPath = combineStrings(workingPath, assetsFolder);

    //Allocate memory to Asset register.
    assetCount = sizeof(definitions) / sizeof(AssetDef);
    assets = malloc(sizeof(Asset) * assetCount);

    //Build and load each Asset into the register.
    for(int i=0; i < assetCount; i++) {
        assets[i] = makeAsset(definitions[i]);
    }
}

static void loadSounds(void) {
    const int SOUND_VOLUME = 12;

    SoundDef defs[] = {
        { "break.wav", SOUND_VOLUME },
        { "down.wav", SOUND_VOLUME },
        { "drink.wav", SOUND_VOLUME },
        { "up.wav", SOUND_VOLUME },
		{ "shoot3.wav", SOUND_VOLUME },
		{ "fire-coffee.wav", SOUND_VOLUME },
    };

    soundCount = sizeof(defs) / sizeof(SoundDef);
    sounds = malloc(sizeof(SoundAsset) * soundCount);

    for(int i=0; i < soundCount; i++) {
        //Load music.
        char* path = combineStrings(assetPath, defs[i].filename);
        Mix_Chunk* chunk = Mix_LoadWAV(path);
        if(!chunk) fatalError("Could not find Asset on disk", path);

        //Reduce volume if called for.
        if(defs[i].volume < SDL_MIX_MAXVOLUME) Mix_VolumeChunk(chunk, defs[i].volume);

        //Add to register
        SoundAsset snd = {
            defs[i].filename,
            chunk
        };
        sounds[i] = snd;
    }
}

void initAssets(void) {
    loadImages();
    loadSounds();
}

void play(char* path) {
    Mix_PlayChannel(-1, getSound(path).sound, 0);
}
