#include "scene.h"
#include "renderer.h"
#include "common.h"
#include "assets.h"

double bobHeadInc = 1;
double bobBodyInc = 1;
double breakOffset = 0;
double drinkOffset = 0;
bool bobDir = false;
bool bob2Dir = false;
bool toyOn = false;
bool onBreak = false;
bool onDrink = false;
int drinkInc = 0;
int maxDrink = 50;

char *deskToys[] = {
        "lamp.png",
        "plant.png",
        "newtonscradle.png",
        "rubikscube.png",
        "rubikscubesolved.png",
        "tree.png",
        "clockoff.png",
        "cat.png"
};

//double coffeeInc = 0;
//double coffeePos = 0;

void sceneAnimateFrame() {

//    coffeePos = sineInc(100, &coffeeInc, 0.05, 32);

    if(bobDir && bobHeadInc > 1) {
        bobDir = false;
    }else if(!bobDir && bobHeadInc < 0){
        bobDir = true;
    }

    if(bob2Dir && bobBodyInc > 2) {
        bob2Dir = false;
    }else if(!bob2Dir && bobBodyInc < 1){
        bob2Dir = true;
    }

    if(bobDir) {
        bobHeadInc += 0.5;
    }else{
        bobHeadInc -= 0.5;
    }

    if(bob2Dir) {
        bobBodyInc += 0.5;
    }else{
        bobBodyInc -= 0.5;
    }

    if(toyOn){
        toyOn=false;
    }else{
        toyOn=true;
    }

}

void sceneRenderFrame() {
    if(mode == MODE_COMBAT) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderClear(renderer);
    }else{
        int xOffset = 3;
        int yOffset = 30;

        if (onBreak){
            breakOffset += 10;
            if (breakOffset > 200){
                onBreak = false;
                breakOffset = 0;
            }
        }

        if (onDrink && drinkInc < maxDrink){
            drinkOffset = 20;
            drinkInc++;
        } else {
            drinkOffset = 0;
            onDrink = false;
            drinkInc = 0;
        }

        Sprite sprite = makeSimpleSprite("office-bg.png");
        drawSprite(sprite, makeCoord(screenBounds.x/2, screenBounds.y/2));
        Sprite leg1 = makeSimpleSprite("leg.png");
        drawSprite(leg1, makeCoord(screenBounds.x/2 - xOffset - breakOffset + drinkOffset, screenBounds.y/2 + yOffset + 30 - bobBodyInc));
        Sprite leg2 = makeSimpleSprite("leg.png");
        drawSprite(leg2, makeCoord(screenBounds.x/2 - xOffset - 30 - breakOffset + drinkOffset, screenBounds.y/2 + yOffset + 30 + bobBodyInc));
        Sprite chair1 = makeSimpleSprite("chair-bottom-1.png");
        drawSprite(chair1, makeCoord(screenBounds.x/2 - xOffset + drinkOffset, screenBounds.y/2 + yOffset + 31));
        if (drinkInc < maxDrink/2) {
            if (randomNo > 90) {
                Sprite coffee = makeSimpleSprite("coffee.png");
                drawSprite(coffee, makeCoord(235/*ceil(coffeePos)*/, screenBounds.y / 2 + 5));
            }
            else {
                Sprite energydrink = makeSimpleSprite("energydrink.png");
                drawSprite(energydrink, makeCoord(235/*ceil(coffeePos)*/, screenBounds.y / 2 + 5));
            }
        }
        if(toyOn){
            Sprite deskToy = makeSimpleSprite("clockon.png");
            drawSprite(deskToy, makeCoord(65/*ceil(coffeePos)*/, screenBounds.y/2 + 5));
        }else{
            Sprite deskToy = makeSimpleSprite("clockoff.png");
            drawSprite(deskToy, makeCoord(65/*ceil(coffeePos)*/, screenBounds.y/2 + 5));
        }
        //Sprite chair1 = makeSimpleSprite("chair-bottom.png");
        //drawSprite(chair1, makeCoord(screenBounds.x/2 - xOffset + 20, screenBounds.y/2 + yOffset));
        //Sprite chair1 = makeSimpleSprite("chair-bottom.png");
        //drawSprite(chair1, makeCoord(screenBounds.x/2 - xOffset, screenBounds.y/2 + yOffset));
        if (onDrink && drinkInc < maxDrink/2){
            Sprite arm = makeSimpleSprite("arm.png");
            drawSprite(arm, makeCoord(screenBounds.x/2 - xOffset - breakOffset + drinkOffset, screenBounds.y/2 + yOffset - bobBodyInc));
            Sprite body = makeSimpleSprite("body-yellow.png");
            drawSprite(body, makeCoord(screenBounds.x/2 - xOffset - breakOffset + drinkOffset, screenBounds.y/2 + yOffset - bobBodyInc));
        }
        if (drinkInc >= maxDrink/2) {
            Sprite armdrink = makeSimpleSprite("armdrink.png");
            drawSprite(armdrink, makeCoord(screenBounds.x/2 - xOffset - breakOffset + drinkOffset, screenBounds.y/2 + yOffset - bobBodyInc));
            Sprite bodydrink = makeSimpleSprite("bodyyellowdrink.png");
            drawSprite(bodydrink, makeCoord(screenBounds.x/2 - xOffset - breakOffset + drinkOffset, screenBounds.y/2 + yOffset - bobBodyInc));
        } else {
            Sprite body = makeSimpleSprite("body-yellow.png");
            drawSprite(body, makeCoord(screenBounds.x / 2 - xOffset - breakOffset + drinkOffset,
                                       screenBounds.y / 2 + yOffset - bobBodyInc));
        }
        Sprite chair2 = makeSimpleSprite("chair-top.png");
        drawSprite(chair2, makeCoord(screenBounds.x/2 - xOffset + drinkOffset, screenBounds.y/2 + yOffset));
        Sprite head = makeSimpleSprite("head-black.png");
        drawSprite(head, makeCoord(screenBounds.x/2 - xOffset - breakOffset + drinkOffset, screenBounds.y/2 + yOffset + 1 - bobHeadInc));

    }
}
