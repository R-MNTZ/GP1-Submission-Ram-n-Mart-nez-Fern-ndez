/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();

/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{
	// Init Variables to given parameters
	// Number of fruits in this game
	this->numFruits = maxNumFruits;
	this->playerScore = 0;
	this->playerHealth = maxPlayerHealth;
}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}



void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Read the high score input file, we get in  playerHighestScore the value
	readHighScoreFile();

	dataSaved = false; // we did not save the highes score data yet

	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Store the textures
	textureName = { "apple", "banana", "orange", "Dragonfruit", "bullet","basket","theBackground", "menuScreen" };
	texturesToUse = { "Images\\apple.png", "Images\\banana.png", "Images\\orange.png", "Images\\Dragonfruit.png", "Images\\bullet.png", "Images\\basket.png", "Images\\backgroundlvl.png", "Images\\menuscreen.jpg" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}


	theGameState = MENU;
	

	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge", "Pacifico", "TrashHand", "Merienda", "MeriendaLarge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/Pacifico.ttf", "Fonts/TrashHand.ttf", "Fonts/Merienda.ttf", "Fonts/Merienda.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	theFontMgr->addFont(fontList[5], fontsToUse[5], 72);

	gameTextNames = { "ReplayTxt", "CHallengeTxt", "Instruction1Txt", "Instruction2Txt",
		"SeeYouTxt","ScoreTxt", "GameOverTxt", "WelcomeTxt", "Instruction3Txt" , "DummyTxt" };

	gameTextList = { "Press space to play again", "Don't let the fruit fall!", "Press Space to Play",
		"Press Esc at any time to exit", "See you again soon!",
		"Score : ", "Oh no! Its Game Over", "Welcome to Fruit Picker!",
		"Use the arrow keys to move" ,
		"        " };

	theTextureMgr->addTexture("Replay", theFontMgr->getFont("Pacifico")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Score", theFontMgr->getFont("Pacifico")->createTextTexture(theRenderer, gameTextList[5], SOLID, { 0, 0, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("GameOver", theFontMgr->getFont("Pacifico")->createTextTexture(theRenderer, gameTextList[6], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instruction1", theFontMgr->getFont("Merienda")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 0, 0, 255 }, { 255, 255, 255, 255 }));
	theTextureMgr->addTexture("Instruction2", theFontMgr->getFont("Merienda")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 0, 0, 0, 255 }, { 255, 255, 255, 255 }));
	theTextureMgr->addTexture("Welcome", theFontMgr->getFont("MeriendaLarge")->createTextTexture(theRenderer, gameTextList[7], SOLID, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }));
	theTextureMgr->addTexture("Instruction3", theFontMgr->getFont("Merienda")->createTextTexture(theRenderer, gameTextList[8], SOLID, { 0, 0, 0, 255 }, { 255, 255, 255, 255 }));

	// handle used by the function renderTextAt
	theTextureMgr->addTexture("Dummy", theFontMgr->getFont("Merienda")->createTextTexture(theRenderer, gameTextList[9], SOLID, { 0, 0, 0, 255 }, { 0, 0, 0, 0 }));


	// Load game sounds
	soundList = { "theme", "shot", "explosion", "basketClick" };
	soundTypes = { MUSIC, SFX, SFX, SFX };
	soundsToUse = { "Audio/R-216.mp3", "Audio/shot007.wav", "Audio/explosion2.wav", "Audio/basketClick.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	 theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	spriteBkgdMenu.setSpritePos({ 0, 0 });
	spriteBkgdMenu.setTexture(theTextureMgr->getTexture("menuScreen"));
	spriteBkgdMenu.setSpriteDimensions(theTextureMgr->getTexture("menuScreen")->getTWidth(), theTextureMgr->getTexture("menuScreen")->getTHeight());

	newStartingBasket();  // Create new basket

	newStartingSetOfFruits(); // Create new sets of fruits

	//loadDataFile(); // Loads the data file with high scores etc
}

/*
* Creates a new basket suitable for the game beginning
*/
void cGame::newStartingBasket() {
	theBasket.setSpritePos({ 350, 500 });
	theBasket.setTexture(theTextureMgr->getTexture("basket"));
	theBasket.setSpriteDimensions(theTextureMgr->getTexture("basket")->getTWidth(), theTextureMgr->getTexture("basket")->getTHeight());
}

/**
 * Creates a new set of fruits suitable for the game beginning
*/
void cGame::newStartingSetOfFruits() {

	theFruits.clear(); // Erase completely what is left 

	for (int frt = 0; frt < this->maxNumFruits; frt++) {
		addNewFruit();
	}
}

/* Adds a single randomized fruit at the top of the screen, that will fall
*/
void cGame::addNewFruit() {
	theFruits.push_back(new cFruit);
	int frt = theFruits.size() - 1;

	cout << "addNewFruit: frt = " << frt << endl;

	theFruits[frt]->setSpritePos({ rand() % 900 + 50 , rand() % 50 });
	int i = rand() % 900;
	cout << "i = " << i << endl;
	//		theFruits[frt]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
	theFruits[frt]->setSpriteTranslation({ 5, 5 });
	int randFruit = rand() % 4;
	theFruits[frt]->setTexture(theTextureMgr->getTexture(textureName[randFruit]));
	theFruits[frt]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randFruit])->getTWidth(), theTextureMgr->getTexture(textureName[randFruit])->getTHeight());
	int v = rand() % 3 + 2;
	theFruits[frt]->setFruitVelocity({ v,  v });
	theFruits[frt]->setActive(true);
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;
	bool relevantKeyPressed = false;
	theGameState = MENU;
	bool exitTheGame = false;

	while (loop)
	{
		switch (theGameState) {

		case MENU:
		{
			relevantKeyPressed = this->getInputMenu();
			if (this->KeyPressedESC == true) exitTheGame = true; // quit event or ESC
			if (this->KeyPressedRIGHT == true) {
				theGameState = PLAYING; // We pressed SPACE
				this->m_lastTime = high_resolution_clock::now();
			}
			this->renderMenu(theSDLWND, theRenderer);

		}
		break;
		case PLAYING:
		{
			dataSaved = false; // e reset the flag for data saving in file

			//We get the time that passed since the last frame
			double elapsedTime = this->getElapsedSeconds();

			relevantKeyPressed = this->getInputPlaying();

			if (this->KeyPressedESC == true) exitTheGame = true; // quit event or ESC

			this->updatePlaying(elapsedTime);
			this->renderPlaying(theSDLWND, theRenderer);
		}
		break;
		case END: {

			if (dataSaved == false)
			{
				writeHighestScore(); // The game finished, we write the highest score in file, onlyh once
				dataSaved = true;
			}
			 
			relevantKeyPressed = this->getInputEnd();
			if (this->KeyPressedESC == true) exitTheGame = true; // quit event or ESC		

			if (this->KeyPressedRIGHT == true) { // We pressed SPACE
				// reset health, score, basket, fruits and back to playing
				this->playerHealth = this->maxPlayerHealth;
				this->playerScore = 0;
				this->newStartingBasket();
				this->newStartingSetOfFruits(); // We reset the fruits to starting points
				theGameState = PLAYING;
				this->m_lastTime = high_resolution_clock::now();
			}

			this->renderEnd(theSDLWND, theRenderer);

		}
				  break;

		default:
			break;
		}

		if (exitTheGame == true)  break;
	} // this key ends the while(loop)

}

void cGame::renderPlaying(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);

	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
	// Render each fruit in the vector array
	for (int frt = 0; frt < theFruits.size(); frt++)
	{
		theFruits[frt]->render(theRenderer, &theFruits[frt]->getSpriteDimensions(),
			&theFruits[frt]->getSpritePos(), theFruits[frt]->getSpriteRotAngle(),
			&theFruits[frt]->getSpriteCentre(), theFruits[frt]->getSpriteScale());
	}

	

	// Display current score + highscore
	string s = "Highscore: "     + std::to_string(this->playerHighestScore) + 
		       " | Score: " + std::to_string(this->playerScore);
	cout << " s = " << s << endl;

	this->renderTextAt(100, 100, s, theRenderer);

	// render the basket
	theBasket.render(theRenderer, &theBasket.getSpriteDimensions(),
		&theBasket.getSpritePos(), theBasket.getSpriteRotAngle(),
		&theBasket.getSpriteCentre(), theBasket.getSpriteScale());

	SDL_RenderPresent(theRenderer);
}

void cGame::renderMenu(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	spriteBkgdMenu.render(theRenderer, NULL, NULL, spriteBkgdMenu.getSpriteScale());
	tempTextTexture = theTextureMgr->getTexture("Instruction1");
	pos = { 100,500, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	scale = { 1, 1 };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	tempTextTexture = theTextureMgr->getTexture("Instruction2");
	pos = { 500,500, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	tempTextTexture = theTextureMgr->getTexture("Welcome");
	pos = { 100,100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	tempTextTexture = theTextureMgr->getTexture("Instruction3");
	pos = { 100,550, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	SDL_RenderPresent(theRenderer);

}

void cGame::renderEnd(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
	// Render the Title
	tempTextTexture = theTextureMgr->getTexture("GameOver");
	pos = { 300,300, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	scale = { 1, 1 };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	tempTextTexture = theTextureMgr->getTexture("Replay");
	pos = { 300,350, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	SDL_RenderPresent(theRenderer);
}


/**
* Update for playing phase
*/
void cGame::updatePlaying(double deltaTime)
{
	// Update the visibility and position of each fruit

	vector<cFruit*>::iterator fruit = theFruits.begin();
	while (fruit != theFruits.end())
	{
		if ((*fruit)->isActive() == false) {
			// this eliminates the fruit of index fruit and shortens the vector by one element
			fruit = theFruits.erase(fruit);
			// We create a replacement up there on the screen at a random place
			this->addNewFruit();
		}
		else {
			(*fruit)->update(deltaTime);
			++fruit;
		}

	}

	// Update the Basket position
	theBasket.update(deltaTime, this->KeyPressedRIGHT, this->KeyPressedLEFT);

	// Once the event of pressed keys has been used up, reset the keypressed flags
	this->KeyPressedLEFT = false;
	this->KeyPressedRIGHT = false;
}


/**
* For the case PLAYING
* Returns true if a relevant key has been pressed activate its flags,
* it activates the ESC also in case of a quit event
*/
bool cGame::getInputPlaying()
{
	SDL_Event currentEvent;

	SDL_PollEvent(&currentEvent); // Get event in "curentEvent"

	if (currentEvent.type == SDL_QUIT) {
		this->KeyPressedESC = true;
		return true;
	}

	if (currentEvent.type == SDL_KEYDOWN) {

		if (currentEvent.key.keysym.sym == SDLK_ESCAPE) {
			this->KeyPressedESC = true;
			return true;
		}

		if (currentEvent.key.keysym.sym == SDLK_RIGHT) {
			this->KeyPressedRIGHT = true;
			this->KeyPressedLEFT = false;
			return true;
		}

		if (currentEvent.key.keysym.sym == SDLK_LEFT) {
			this->KeyPressedRIGHT = false;
			this->KeyPressedLEFT = true;
			return true;
		}
	}

	return false; // no relevant event detected in this call
}

/**
* For the case MENU
* Returns true if a relevant key has been pressed activate its flags,
* it activates the ESC also in case of a quit event
*/
bool cGame::getInputMenu()
{
	SDL_Event currentEvent;

	SDL_PollEvent(&currentEvent); // Get event in "curentEvent"

	if (currentEvent.type == SDL_QUIT) {
		this->KeyPressedESC = true;
		return true;
	}

	if (currentEvent.type == SDL_KEYDOWN) {

		if (currentEvent.key.keysym.sym == SDLK_ESCAPE) {
			this->KeyPressedESC = true;
			return true;
		}

		if (currentEvent.key.keysym.sym == SDLK_SPACE) {
			this->KeyPressedRIGHT = true;
			this->KeyPressedLEFT = false;
			return true;
		}

	}

	return false; // no relevant event detected in this call
}

/**
* For the case END
* Returns true if a relevant key has been pressed activate its flags,
* it activates the ESC also in case of a quit event
*/
bool cGame::getInputEnd()
{
	SDL_Event currentEvent;

	SDL_PollEvent(&currentEvent); // Get event in "curentEvent"

	if (currentEvent.type == SDL_QUIT) {
		this->KeyPressedESC = true;
		return true;
	}

	if (currentEvent.type == SDL_KEYDOWN) {

		if (currentEvent.key.keysym.sym == SDLK_ESCAPE) {
			this->KeyPressedESC = true;
			return true;
		}

		if (currentEvent.key.keysym.sym == SDLK_SPACE) {
			this->KeyPressedRIGHT = true;
			this->KeyPressedLEFT = false;
			return true;
		}

	}

	return false; // no relevant event detected in this call
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

// Returns pointer to the Basket
cBasket* cGame::getBasket() {
	return &(this->theBasket);
}

vector<cFruit*> cGame::getFruits() {
	return this->theFruits;
}

void cGame::increaseScore()
{
	playerScore += 100;

	if (playerScore > playerHighestScore) {
		this->playerHighestScore = this->playerScore;
	}

	//cout << playerScore << endl;
}

void cGame::decreaseHealth()
{
	this->playerHealth -= 1;
	cout << "health = " << this->playerHealth << endl;

	if (this->playerHealth <= 0) {
		theGameState = END;
	}
}

/*
* convert string into an LPCSTR
*/
LPCSTR getLPCSTR(string s) {

	int numC = s.size() + 1;
	char *cv = new char[numC]; // char

	for (int j = 0; j < (numC - 1); ++j) cv[j] = s.at(j);

	cv[numC - 1] = '\0'; // null terminated

	return cv;
}

/**
   Displays some text at some point of the screen
*/
void cGame::renderTextAt(int xpos, int ypos, string myText, SDL_Renderer* theRenderer)
{
	// cast the argument to the require type
	LPCSTR text = getLPCSTR(myText);

	/*	cout << "text =";
		for (int j = 0; j < myText.size(); ++j) cout << text[j];
		cout << endl; */

		// create the corresponding texture (image) using font manager
	SDL_Texture *tempTextTexture = theFontMgr->getFont("Merienda")->
		createTextTexture(theRenderer, text,
			SOLID, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });

	// Load the just created texture with the dummy handle ...
	theTextureMgr->deleteTexture("Dummy"); //uses this handle
	theTextureMgr->addTexture("Dummy", tempTextTexture);

	// Create with it an object of the class cTexture, grab a pointer x to it
	cTexture *x = theTextureMgr->getTexture("Dummy");

	// set position and scale and render
	scale = { 1, 1 };
	pos = { xpos,ypos, x->getTextureRect().w , x->getTextureRect().h };
	x->renderTexture(theRenderer, x->getTexture(), &x->getTextureRect(), &pos, scale);
}

/*
Read data.txt to get the highest score value
*/
void cGame::readHighScoreFile() {

	ifstream cinf; // declare an onject of the type input file stream
	cinf.open("data.txt"); //

	if (cinf.fail()) {
		cout << "cinf.fail() " << endl;

		// Do something when it fails to find data.txt
		// ...
		cout << "Failed to open data.txt, we create it" << endl;
		ofstream coutf("data.txt");
		coutf << "highest_score:" << endl;
		int j = 100;
		coutf << j << endl;
		coutf.close();

		// And we open it
		cinf.open("data.txt");

		// if this time fails we exit with an error code
		if (cinf.fail()) {
			cerr << " Failed to open twice data txt" << endl;
			exit(-1);
		}
	}

	// It did not fail to open, we read the two lines
	string comlin;
	int x;
	cinf >> comlin;
	cinf >> x;

	cout << "Line 1 =" << comlin << endl;
	//int x = atoi(buffer);
	cout << "x value = " << x << endl;

	this->playerHighestScore = x;  // Assign the highest score read
	cinf.close();
}

/**
 OverWrite data.txt with the current highest score value
*/
void cGame::writeHighestScore() {
	ofstream coutf("data.txt");

	coutf << "highest_score:" << endl;
	coutf << playerHighestScore << endl;
	coutf.close();

	cout << "Written file data.txt with:" << endl;
	cout << "highest_score:" << endl;
	cout << playerHighestScore << endl;
}

void cGame::playBasketClick()
{
	theSoundMgr->getSnd("basketClick")->play(0);
}
