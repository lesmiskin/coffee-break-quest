#include "scene.h"
#include "renderer.h"
#include "common.h"
#include "assets.h"
#include "hud.h"
#include "meter.h"
#include "enemy.h"

int typeInc = 0;
double bobHeadInc = 1;
double bobBodyInc = 1;
double breakOffset = 0;
double drinkOffset = 0;
bool bobDir = false;
bool bob2Dir = false;
Prop props[MAX_PROPS];
int propInc = 0;
bool toyOn = false;
bool onBreak = false;
bool onDrink = false;
int drinkInc = 0;
int maxDrink = 50;
double minuteDeg = 0;
double hourDeg = 270;
int newProg;
int oldProg = 0;

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

//TODO: Random vases etc. around the playing field.
//TODO: Shooting people makes them fly backwards (inverse of target).
//TODO: Coffee cups vanish when hitting player.

int titleAnim = 0;
Coord coffeeCoord = { 100, 100 };
double coffeeInc = 0;
bool textFlicker = false;

void sceneAnimateFrame() {
	switch(currentMode) {
		case MODE_COMBAT_INTRO:
		case MODE_OFFICE_INTRO:
		case MODE_TITLE:
			titleAnim = titleAnim == 0 ? 1 : 0;
			textFlicker = !textFlicker;
			break;
	}

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

void officeFrame(void) {
//	if(typeInc == typeNo || typeInc == 40){
//		play("type.wav");
//		typeNo = randomMq(0,30);
//		typeInc = 0;
//	} else {
//		typeInc++;
//	}

    newProg = (int)timeProgress;
    if(newProg != oldProg) {

		//Chunky movement (still cool - make an option?)
//		if (newProg % 5 == 0) {
//			minuteDeg += 30;
//			if (newProg % 60 == 0) {
//				hourDeg += 30;
//			}
//		}

        //Smooth movement.
		if (newProg % 1 == 0) {
            minuteDeg += 6;
			hourDeg += 0.5;
        }
    }
    oldProg = newProg;

    Sprite sprite = makeSimpleSprite("office-bg.png");
    drawSprite(sprite, makeCoord(screenBounds.x/2, screenBounds.y/2));
    Sprite screen;
    switch ((int)screenNo) {
        case 0:
        case 1:
        case 2:
        case 3:
            screen = makeSimpleSprite("screenblank.png");
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            screen = makeSimpleSprite("screentext.png");
            break;
        case 9:
            screen = makeSimpleSprite("bsod.png");

            break;
    }
    drawSprite(screen, makeCoord(screenBounds.x/2 - 6, screenBounds.y/2 - 41));

    // clock
    Sprite clock = makeSimpleSprite("clockface.png");
    drawSprite(clock, makeCoord(screenBounds.x/2+100, screenBounds.y/2-85));
    Sprite minHand = makeSimpleSprite("minutehand.png");
    drawSpriteFull(minHand, makeCoord(screenBounds.x/2+100, screenBounds.y/2-85),1,minuteDeg);
    Sprite hrHand = makeSimpleSprite("hourhand.png");
    drawSpriteFull(hrHand, makeCoord(screenBounds.x/2+100, screenBounds.y/2-85),1,hourDeg);

    int xOffset = 3;
    int yOffset = 30;

    if (onDrink && drinkInc < maxDrink){
        drinkOffset = 20;
        drinkInc+=timeRate;
    } else {
        drinkOffset = 0;
        onDrink = false;
        drinkInc = 0;
    }

    if (onBreak){
        breakOffset += 5*timeRate;
        if (breakOffset > (screenBounds.x/2 - xOffset + drinkOffset)) breakOffset = screenBounds.x/2 - xOffset + drinkOffset;
        if (breakOffset >= (screenBounds.x/2 - xOffset + drinkOffset)){
            onBreak = false;
            breakOffset = 0;
            aggro = false;
            changeMode(MODE_BREAK_INTRO);
        }

    }

    Sprite leg1 = makeSimpleSprite("leg.png");
    drawSprite(leg1, makeCoord(screenBounds.x/2 - xOffset - breakOffset + drinkOffset, screenBounds.y/2 + yOffset + 30 - bobBodyInc));
    Sprite leg2 = makeSimpleSprite("leg.png");
    drawSprite(leg2, makeCoord(screenBounds.x/2 - xOffset - 30 - breakOffset + drinkOffset, screenBounds.y/2 + yOffset + 30 + bobBodyInc));
    Sprite chair1 = makeSimpleSprite("chair-bottom-1.png");
    drawSprite(chair1, makeCoord(screenBounds.x/2 - xOffset + drinkOffset, screenBounds.y/2 + yOffset + 31));
    if (drinkInc < maxDrink/2) {
        if (randomNo > 2) {
            Sprite coffee = makeSimpleSprite("coffee.png");
            drawSprite(coffee, makeCoord(228/*ceil(coffeePos)*/, screenBounds.y / 2 + 12));
        }
        else {
            Sprite energydrink = makeSimpleSprite("energydrink.png");
            drawSprite(energydrink, makeCoord(228/*ceil(coffeePos)*/, screenBounds.y / 2 + 12));
        }
    }

    Sprite item;
    switch ((int)itemNo) {
        case 0:
        case 1:
        case 2:
            item = (toyOn) ? makeSimpleSprite("newtonscradle.png") : makeFlippedSprite("newtonscradle.png",SDL_FLIP_HORIZONTAL);
            break;   
        case 3:
            item = makeSimpleSprite("lamp.png");
            break;
        case 4:
            item = makeSimpleSprite("rubikscubesolved.png");
            break;
        case 5:
            item = makeSimpleSprite("rubikscube.png");
            break;
        case 6:
            item = makeSimpleSprite("plant.png");
            break;
        case 7:
            item = makeSimpleSprite("tree.png");
            break;
        case 8:
            item = (toyOn) ? makeSimpleSprite("cat.png") : makeSimpleSprite("cat1.png");
            break;
        case 9:
            item = (toyOn) ? makeSimpleSprite("clockon.png") : makeSimpleSprite("clockoff.png");
            break;
    }
    drawSprite(item, makeCoord(65/*ceil(coffeePos)*/, screenBounds.y/2 + 5));

//    if(toyOn){
//        Sprite item = makeSimpleSprite("clockon.png");
//        drawSprite(deskToy, makeCoord(65/*ceil(coffeePos)*/, screenBounds.y/2 + 5));
//    }else{
//        Sprite deskToy = makeSimpleSprite("clockoff.png");
//        drawSprite(deskToy, makeCoord(65/*ceil(coffeePos)*/, screenBounds.y/2 + 5));
//    }
    //Sprite chair1 = makeSimpleSprite("chair-bottom.png");
    //drawSprite(chair1, makeCoord(screenBounds.x/2 - xOffset + 20, screenBounds.y/2 + yOffset));
    //Sprite chair1 = makeSimpleSprite("chair-bottom.png");
    //drawSprite(chair1, makeCoord(screenBounds.x/2 - xOffset, screenBounds.y/2 + yOffset));
    if (onDrink && drinkInc < maxDrink/2){
        Sprite arm = makeSimpleSprite("arm.png");
        drawSprite(arm, makeCoord(screenBounds.x/2 - xOffset - breakOffset + drinkOffset + 5, screenBounds.y/2 + yOffset - bobBodyInc - 20));
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


void sceneRenderFrame() {

	switch(currentMode) {
		case MODE_TITLE: {
			Sprite carpet = makeSimpleSprite("carpet.png");
			drawSprite(carpet, makeCoord(screenBounds.x/2, screenBounds.y/2));

			Sprite coffee = makeSimpleSprite("coffee.png");
			drawSprite(coffee, makeCoord(sineInc(screenBounds.x/2, &coffeeInc, 0.05, 64), 120));

			Sprite sprite;

			if(titleAnim == 0) {
				sprite = makeSimpleSprite("title-coffee-break-quest.png");
			}else{
				sprite = makeSimpleSprite("title-coffee-break-quest-1.png");
			}
			drawSprite(sprite, makeCoord(screenBounds.x / 2, screenBounds.y / 3));

			if(textFlicker) {
				writeFont("press spacebar to begin your adventure!", makeCoord(90, 220));
			}

			break;
		}

		case MODE_OFFICE_INTRO: {
			Sprite bg = makeSimpleSprite("carpet.png");
			drawSprite(bg, makeCoord(screenBounds.x/2, screenBounds.y/2));

			Sprite sprite = makeSimpleSprite("title-hai.png");
			drawSprite(sprite, makeCoord(screenBounds.x/2, 32));

			writeFont("you must achieve full productivity before the workday ends!", makeCoord(60, 64));

			writeFont("keep your productivity up by drinking coffee", makeCoord(60, 90));
			writeFont("press the d key", makeCoord(60, 100));

			writeFont("take regular breaks to maintain your hygiene", makeCoord(60, 120));
			writeFont("press the b key", makeCoord(60, 130));

			writeFont("change the speed of the workday as needed", makeCoord(60, 150));
			writeFont("press up arrow to speed up", makeCoord(60, 160));
			writeFont("press down arrow to slow down", makeCoord(60, 170));

			if(textFlicker) {
				writeFont("press space again to start", makeCoord(113, 220));
			}
			break;
		}
		case MODE_OFFICE:
			officeFrame();
			break;

		case MODE_BREAK: {
			Sprite sprite = makeSimpleSprite("carpet.png");
			drawSprite(sprite, makeCoord(screenBounds.x / 2, screenBounds.y / 2));

			for (int i = 0; i < MAX_PROPS; i++) {
				if (props[i].coord.x == 0) continue;
				drawSprite(props[i].sprite, props[i].coord);
			}
			break;
		}
		case MODE_BREAK_INTRO: {
			Sprite bg = makeSimpleSprite("carpet.png");
			drawSprite(bg, makeCoord(screenBounds.x/2, screenBounds.y/2));

			Sprite sprite = makeSimpleSprite("title-hai.png");
			drawSprite(sprite, makeCoord(screenBounds.x/2, 32));

			writeFont("your mission if you choose to accept it is", makeCoord(70, 64));
			writeFont("find the correct bathroom to use!", makeCoord(70, 74));

			Sprite door1 = makeSimpleSprite("doormale.png");
			drawSprite(door1, makeCoord(130, 140));
			Sprite door2 = makeSimpleSprite("doorfemale.png");
			drawSprite(door2, makeCoord(180, 140));

			writeFont("press space to begin", makeCoord(113, 220));

			break;
		}
		case MODE_BREAK_OOPS: {
			Sprite bg = makeSimpleSprite("carpet.png");
			drawSprite(bg, makeCoord(screenBounds.x/2, screenBounds.y/2));

			Sprite sprite = makeSimpleSprite("title-uhh.png");
			drawSprite(sprite, makeCoord(screenBounds.x/2, 32));

			writeFont("that is the wrong bathroom", makeCoord(70, 64));
			writeFont("hey you have a reputation to maintain!", makeCoord(70, 80));

			writeFont("press space to continue", makeCoord(113, 220));

			break;
		} case MODE_COMBAT_INTRO: {
			Sprite bg = makeSimpleSprite("carpet.png");
			drawSprite(bg, makeCoord(screenBounds.x/2, screenBounds.y/2));

			Sprite sprite = makeSimpleSprite("title-war.png");
			drawSprite(sprite, makeCoord(screenBounds.x/2, 32));

			writeFont("your colleagues are angry at your poor sanitation", makeCoord(70, 64));
			writeFont("and have banded together to destroy you!", makeCoord(70, 74));

			writeFont("defend yourself with the spacebar", makeCoord(70, 94));
			writeFont("you must push all colleagues offscreen to win", makeCoord(70, 104));

			if(textFlicker) {
				writeFont("press space to start", makeCoord(113, 220));
			}
			break;

		} case MODE_COMBAT_LOST: {
			Sprite bg = makeSimpleSprite("carpet.png");
			drawSprite(bg, makeCoord(screenBounds.x/2, screenBounds.y/2));

			Sprite sprite = makeSimpleSprite("title-fail.png");
			drawSprite(sprite, makeCoord(screenBounds.x/2, 32));

			writeFont("your combat skills leave much to be desired", makeCoord(70, 64));

			writeFont("better luck next round!", makeCoord(70, 84));

			writeFont("press space to play again", makeCoord(113, 220));
			break;
		}
		case MODE_COMBAT_WON: {
			Sprite bg = makeSimpleSprite("carpet.png");
			drawSprite(bg, makeCoord(screenBounds.x/2, screenBounds.y/2));

			Sprite sprite = makeSimpleSprite("good-aim.png");
			drawSprite(sprite, makeCoord(screenBounds.x/2, 32));

			writeFont("congratulations!", makeCoord(70, 64));
			writeFont("your prowess in battle bring your colleagues to their knees", makeCoord(70, 84));

			writeFont("press space to play again", makeCoord(113, 220));
			break;
		}
		case MODE_OFFICE_WON: {
			Sprite bg = makeSimpleSprite("carpet.png");
			drawSprite(bg, makeCoord(screenBounds.x/2, screenBounds.y/2));

			Sprite sprite = makeSimpleSprite("title-winning.png");
			drawSprite(sprite, makeCoord(screenBounds.x/2, 32));

			writeFont("congratulations!", makeCoord(70, 64));
			writeFont("you were promoted to management", makeCoord(70, 84));
			writeFont("enjoy your life of endless hell", makeCoord(70, 104));

			writeFont("press space to play again", makeCoord(113, 220));
			break;
		}
		case MODE_OFFICE_LOST: {
			Sprite bg = makeSimpleSprite("carpet.png");
			drawSprite(bg, makeCoord(screenBounds.x/2, screenBounds.y/2));

			Sprite sprite = makeSimpleSprite("title-fail.png");
			drawSprite(sprite, makeCoord(screenBounds.x/2, 32));

			writeFont("youre fired!", makeCoord(70, 64));
			writeFont("maybe try your luck as an indie game developer", makeCoord(70, 84));

			writeFont("press space to play again", makeCoord(113, 220));
			break;
		}
	}

	return;
}

//Should happen once per-game (e.g. set time to 9am)
void initOnceScene() {
	minuteDeg = 0;
	hourDeg = 270;
}

//Should happen each time the scene is shown.
void initScene() {
	if(currentMode == MODE_BREAK) {
		propInc = 0;

        for (int i = 0; i < MAX_PROPS; i++) {
            Sprite sprite;

			//Add door sprites at magic indexes for breaktime.
			if (!aggro && i == 6){
                sprite = makeSimpleSprite("doormale.png");
            }else if(!aggro && i==7){
                sprite = makeSimpleSprite("doorfemale.png");

			//Add usual random props.
			}else {
                switch (randomMq(0, 5)) {
                    case 0:
                        sprite = makeSimpleSprite("plant.png");
                        break;
                    case 1:
                        sprite = makeSimpleSprite("tree.png");
                        break;
					case 2:
						sprite = makeSimpleSprite("newtonscradle.png");
						break;
					case 3:
						sprite = makeSimpleSprite("cat.png");
						break;
					case 4:
						sprite = makeSimpleSprite("cactus.png");
						break;
//                case 2:
//                    sprite = makeSimpleSprite("lamp.png");
//                    break;
//				case 3:
//					sprite = makeSimpleSprite("coffee.png");
//					break;
//				case 5:
//					sprite = makeSimpleSprite("energydrink.png");
//					break;
//				case 6:
//					sprite = makeSimpleSprite("clockon.png");
//					break;
                }
            }
			Prop prop = {
				sprite,
				makeCoord(randomMq(0, screenBounds.x), randomMq(0, screenBounds.y))
			};
			props[propInc++] = prop;
        }
    }
}
