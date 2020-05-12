/**
 * All code created by *.* (Star Dot Star) except where noted (or not)
 * It may be modified, hacked and abused as you see fit.
 *
 * Star Dot Star Team:	Brian Adeloye
 *		                         Jason Gowan
 *		                         Mark Hazlewood
 *		                         Andy McCartney
 *		                         Brian Weis
 *	    
 * All work copyright *.* (Star Dot Star) 2005-infinite
 * 
 */

#include "IntroManager.h"
#include <fstream>

/*
 * constructor
 *
 * param - filename - name of file that contains info about which screens to
 *                    display during the intro
 */
IntroManager::IntroManager(char *filename) {
	currScreen = -1;
	currScreenTime = 0.0;
	screens = 0;      //zero the pointer(s)

	//loads all texture metadata
	loadTextureInfo(filename);
}

/*
 * destructor
 */
IntroManager::~IntroManager() {
	if(screens) {
		for(int i=0; i < numScreens; ++i) {
			if(screens[i].texture) {
				screens[i].texture->Release();
				screens[i].texture = 0;
			}

			if(screens[i].filename) {
				delete[] screens[i].filename;
				screens[i].filename = 0;
			}
		}
		delete []screens;
	}
}

/*
 * loadTextureInfo
 *
 * loads texture metadata (texture filename, dimensions) for each screen to
 * be displayed during the intro. RIGHT NOW IT'S ASSUMED EACH INTRO SCREEN
 * WILL BE WINDOW_WIDTH x WINDOW_HEIGHT
 */
void IntroManager::loadTextureInfo(char *filename) {
	//open an input filestream
	std::ifstream introFile(filename);

	//find out how long each screen will be shown during the intro,
	//as well as how many screens there are
	introFile >> secsPerScreen;
	introFile >> numScreens;

	screens = new Texture[numScreens];

	//loads texture info for each intro screen (filename, width, height)
	for(int i = 0; i < numScreens; ++i) {
		//get the filename of the texture of this intro screen
		char buffer[256];
		introFile >> buffer;
		screens[i].filename = new char[strlen(buffer) + 1];
		strcpy(screens[i].filename, buffer);

		screens[i].texture = 0;  //zero out the texture pointer
	}

	//the first screen of the intro is ready to be shown
	currScreen = 0;

	//clean up
	introFile.close();
}

/*
 * update (could prob think of a better name for this function)
 *
 * param - timePassed - num of secs passed since the last call to update()
 * returns false if the intro has ended, true otherwise
 */
bool IntroManager::update(float timePassed) {
	bool retVal = true;

	//if the current screen's been displayed for longer than it should be,
	//moves on to the next intro screen
	if((currScreenTime += timePassed) > secsPerScreen && currScreen < numScreens - 1) {
		return nextScreen();
	}

	return retVal;
}

/*
 * nextScreen
 *
 * display the next intro screen
 *
 * returns false if the intro has ended, true otherwise
 */
bool IntroManager::nextScreen() {
	bool retVal = true;

	currScreenTime = 0.0;
	
	//if there are no more intro screens, let the calling function know
	//that the intro is finished
	if(++currScreen > numScreens - 1) {
		retVal = false;
	}

	return retVal;
}

/*
 * getCurrTexture
 *
 * gets the texture that should currently be displayed
 *
 * returns the current texture to be displayed
 */
Texture *IntroManager::getCurrTexture() {
	return &screens[currScreen];
}

/*
* getNumTextures
*
* how many textures the intro has
*
* returns how many textures the intro has
*/
int IntroManager::getNumTextures() {
	return numScreens;
}

HRESULT IntroManager::loadTextures() {
	HRESULT r = S_OK;

	for(int i = 0; i < getNumTextures(); ++i) {
		r = SpriteManager::Instance()->loadTexture(&screens[i]);

		if(FAILED(r)) {
			return r;
		}
	}

	return r;
}

HRESULT IntroManager::draw() {
	return SpriteManager::Instance()->drawSprite(getCurrTexture(), 0, 0, 0);
}