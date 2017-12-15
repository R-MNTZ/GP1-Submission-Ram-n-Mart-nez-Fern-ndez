#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

// SDL2 Headers
#include <SDL.h>

#include "cSDL2WNDManager.h"
#include "cGame.h"

using namespace std;

//This is the SDL window
static cSDL2WNDManager* pgmWNDMgr = cSDL2WNDManager::getInstance();

//This is the our Game
static cGame* theGame = cGame::getInstance();

int main(int argc, char *argv[])
{
	if (!pgmWNDMgr->initWND(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT))
		return -1;

	
	char buffer[1000];

	ifstream inpFile("data.txt");
	if (inpFile.fail()) {
		cout << "inpFile.fail() " << endl;
		// Do something when it fails to find data.txt
		// ...
	}
	else { 
		// It did not fail to open, we read the two lines
		inpFile.getline(buffer, 1000);
		cout << "Line 1 =" << buffer << endl;

		inpFile.getline(buffer, 255);
		cout << "Line 2 =" << buffer << endl;

		int x = atoi(buffer);

		cout << "x value = " << x << endl;
		
		inpFile.close();
	}

	// Writing a file
	int y = 2000;
	ofstream outFile("data2.txt");

	// from integer to string of array of chars
	string pepe;
	pepe = std::to_string(y);
	
	char buff[100];
	buff[0] = '2';
	buff[1] = '0';
	buff[2] = '0';
	buff[3] = '0';
	buff[4] = '\0';

	long size = 8 ;
	outFile.write(buff, size);
	outFile.close();


	/*
	string wholeData;
	string aLine;
	
	cout << "What is your name? ";
	std::getline(inpFile, aLine);
	cout << "Hello " << mystr << ".\n";
	cout << "What is your favorite team? ";
	getline(cin, mystr);
	cout << "I like " << mystr << " too!\n";
	return 0;

	
	char buffer[1000] ;
	SDL_RWops *dataFileptr = SDL_RWFromFile("data.txt", "a+");
	SDL_RWread(dataFileptr, buffer, 9, 1); 
	buffer[9] = '\0';
	//SDL_RWwrite(dataFileptr, "hello file", 11, 1);

	cout << "just read buffer =" << buffer << endl;

	SDL_RWclose(dataFileptr); 
	*/



	theGame->initialise(pgmWNDMgr->getSDLWindow(), pgmWNDMgr->getSDLRenderer());

	theGame->run(pgmWNDMgr->getSDLWindow(), pgmWNDMgr->getSDLRenderer());

	theGame->cleanUp(pgmWNDMgr->getSDLWindow());

	return 0;
}