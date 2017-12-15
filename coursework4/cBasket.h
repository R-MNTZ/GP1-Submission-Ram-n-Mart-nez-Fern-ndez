/*
=================
cBasket.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CBASKET_H
#define _CBASKET_H
#include "cSprite.h"

class cBasket : public cSprite
{
private:
	double SPEED = 250 ; // speed of the moving basket 
	int MaxX = 820; // Basket motion limits
	int MinX = 0; 

public:
	cBasket();
	void update(double deltaTime, bool keyrightpressed, bool keyleftpressed);		
};
#endif