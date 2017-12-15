/*
=================
cFruit.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cFruit.h"
#include "cGame.h"

/*
=================================================================
Default Constructor
=================================================================
*/
cFruit::cFruit() : cSprite()
{
	this->fruitVelocity = { 0, 0 };
}

/*
=================================================================
Updates the fruit position.
Also Checks for :
 -- Collisions with basket
 -- Touching the ground.

 Note:
 The responsibility for collision detection is transferred hereby to each
falling fruit, this has two advantages: 
 - 1) Collisions are checked for only inmediately after movement, this way whenever
 a fruit did not move we do not waste cpu time in checking. 
   2) We can safely change the number of falling fruits with no fear that the "loop" that (over)checks
   for collisions might get out of sync. (it does not exist anymore)

 The "con" is that the class fruits now needs a handle to cGame (to see the basket), 
 which makes this class less transferrable (if we ever want to use it in different codes).
================================================================
*/
void cFruit::update(double deltaTime)
{

	this->setSpriteRotAngle((float)(this->getSpriteRotAngle() + (5.0f * deltaTime)));
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() - 360);
	}

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += (int)(this->getSpriteTranslation().x * deltaTime);
	currentSpritePos.y += (int)(this->getSpriteTranslation().y * deltaTime * 30);

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	// cout << "Fruit position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());

	// Check if this Fruit has just collided with the basket

	// Get both associated position rectangles
	SDL_Rect * box1 = &(this->getSpritePos());

	cGame * pGame = cGame::getInstance(); // Grab pointer to the singleton Game
	SDL_Rect * box2 = &(pGame->getBasket()->getSpritePos());

	// Check for collision
	if (this->collidedWith(box1, box2)) {
		cout << "Fruit collided with basket!!" << endl;
		pGame->playBasketClick();
		this->setActive(false); // This fruit has been collected by the basket
		pGame->increaseScore(); // We increase the score by 100   
	}

	// Check for fruit touching the ground  
	if (this->getSpritePos().y >= 768) {
		cout << " touching the ground!! " << endl;
		// This fruit has been dropped on the ground
		pGame->decreaseHealth();
		this->setActive(false);
	}

}
/*
=================================================================
  Sets the velocity for the fruit
=================================================================
*/
void cFruit::setFruitVelocity(SDL_Point FruitVel)
{
	fruitVelocity = FruitVel;
}
/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
SDL_Point cFruit::getFruitVelocity()
{
	return fruitVelocity;
}
