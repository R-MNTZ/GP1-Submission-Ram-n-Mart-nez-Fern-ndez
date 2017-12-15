/*
=================
cBasket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cBasket.h"

/*
=================================================================
Default Constructor
=================================================================
*/
cBasket::cBasket() : cSprite()
{
}
/*
=================================================================
Update the the basket position

As a function of deltaTime, SPEED and whether keys are being pressed
=================================================================
*/

void cBasket::update(double deltaTime, bool KeyPressedRIGHT, bool KeyPressedLEFT)
{
	// Only when a key has been pressed we need to update
	if (!KeyPressedRIGHT && !KeyPressedLEFT) return;
	
	SDL_Rect currentSpritePos = this->getSpritePos();

	// We calculate the x length corresponding to deltaTime for the given rocket speed
	double deltaX = SPEED * deltaTime;

	// If KeyForRight was being pressed, increment the position decrease in case of left
	int x = currentSpritePos.x;

	if (KeyPressedRIGHT && x <= MaxX) {
		x += deltaX;
	}
	else if (KeyPressedLEFT && x >= MinX) {
		x -= deltaX;
	}

	this->setSpritePos({ x , currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());	
}
