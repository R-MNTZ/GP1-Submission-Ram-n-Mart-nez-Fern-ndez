#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>

// Game specific includes
#include "fruitsGame.h"

using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void newStartingBasket();
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void renderPlaying(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void renderMenu(SDL_Window * theSDLWND, SDL_Renderer * theRenderer);
	void renderEnd(SDL_Window * theSDLWND, SDL_Renderer * theRenderer);
	void updatePlaying(double deltaTime);
	bool getInputPlaying();
	bool getInputMenu();
	bool getInputEnd();
	double getElapsedSeconds();
	//--------------------------
	
	void increaseScore();
	void decreaseHealth();

	void renderTextAt(int xpos, int ypos, string s, SDL_Renderer* theRenderer);

	void newStartingSetOfFruits(); // set starting positions and velocities for fruits

	void addNewFruit(); // Adds new randomized fruit at the top of the screen

	//void loadDataFile(); // Loads data stored in file such as high score etc.
	//7void saveDataFile();

	//--------------------------
	

	static cGame* getInstance();

	void readHighScoreFile();

	void writeHighestScore();

	void playBasketClick();

	// Allows access to basket and fruits by grabbing the pointer to cGame
	cBasket* getBasket();   
	vector<cFruit*> getFruits();

	// Buffer for reading highscore file
	char buffer[500];

	int playerHighestScore = 0;

private:

	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;

	// Sprites for displaying background and  textures
	cSprite spriteBkgd;
	cSprite spriteBkgdMenu;
	cSprite spriteBkgdOver;
	cBasket theBasket;
	cFruit theFruit;

	// game related variables
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	vector<cFruit*> theFruits;

	// Fonts to use
	vector<LPCSTR> fontList;
	vector<LPCSTR> fontsToUse;
	// Text for Game
	vector<LPCSTR> gameTextNames;
	vector<LPCSTR> gameTextList;
	// Game Sounds
	vector<LPCSTR> soundList;
	vector<soundType> soundTypes;
	vector<LPCSTR> soundsToUse;
	
	// Create vector array of button textures
	vector<LPCSTR> btnNameList;
	vector<LPCSTR> btnTexturesToUse;
	vector<SDL_Point> btnPos;
	vector <cButton> theButtons;
	
	// Game objects
	// Define the elements and their position in/on the array/map
	int renderWidth, renderHeight;
	gameState theGameState;
	btnTypes theBtnType;
	SDL_Rect pos;
	FPoint scale;
	SDL_Rect aRect;
	SDL_Color aColour;
	cTexture* tempTextTexture;
	SDL_Point theAreaClicked;

	// Flags carrying information about key pressed state
	bool KeyPressedRIGHT = false; 
	bool KeyPressedLEFT = false; 
	bool KeyPressedESC = false;

	// Parameters meant to be constant
	int maxPlayerHealth = 3 ; // This one sets all the initial healths in the game
	int maxNumFruits = 4 ; // initial and max number of fruits

	int numFruits ; // current Number of fruits in this game
	int playerScore ;	
	int playerHealth ;

	// flag to know if we saved yet or not the data file.
	bool dataSaved = false;

};

#endif
