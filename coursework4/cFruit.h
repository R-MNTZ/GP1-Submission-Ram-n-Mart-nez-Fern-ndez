/*
=================
cFruit.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CFRUIT_H
#define _CFRUIT_H

#include "cSprite.h"

class cFruit : public cSprite
{
private:
	SDL_Point fruitVelocity;

public:
	cFruit();
	void update(double deltaTime);		// Rocket update method
	void setFruitVelocity(SDL_Point asteroidVel);   // Sets the velocity for the asteroid
	SDL_Point getFruitVelocity();				 // Gets the asteroid velocity
};
#endif