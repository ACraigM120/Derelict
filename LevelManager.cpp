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

#include "LevelManager.h"
#include "PowerUpManager.h"
#include "EnemyManager.h"
#include <fstream>

/*
 * constructor
 *
 * param - filename - name of file containing the names of each level file
 */
LevelManager::LevelManager(char* filename) {
	currLevelNum = -1;
	numLevels = -1;
	numFgLayers = -1;
	numBgLayers = -1;
	levels = 0;  //init pointers to zero
	bgLayers = 0;
	fgLayers = 0;

	std::ifstream levelInput(filename);

	//get the number of levels, then each level metadata filename
	levelInput >> numLevels;
	levels = new char*[numLevels];

	for(int i = 0; i < numLevels; ++i) {
        char buffer[256];
		levelInput >> buffer;
		levels[i] = new char[strlen(buffer) + 1];
		strcpy(levels[i], buffer);
	}

	levelInput.close();
}

/*
 * reset
 *
 * puts the levelmanager back into its intial state. basically...puts it in the state it needs to be in
 * when a new game is started
 */
void LevelManager::reset() {
	currLevelNum = -1;
	loadNextLevel();
}

/*
 * destructor
 */
LevelManager::~LevelManager() {
	if(levels) {
		for(int i = 0; i < getNumLevels(); ++i) {
			if(levels[i]) {
				delete[] levels[i];
				levels[i] = 0;
			}
		}

		delete[] levels;
		levels = 0;
	}

	if(bgLayers) {
		for(int i = 0; i < getNumBgLayers(); ++i) {
			if(bgLayers[i]) {
				delete bgLayers[i];
				bgLayers[i] = 0;
			}
		}
		delete[] bgLayers;
		bgLayers = 0;
	}

	if(fgLayers) {
		for(int i = 0; i < getNumFgLayers(); ++i) {
			if(fgLayers[i]) {
				delete fgLayers[i];
				fgLayers[i] = 0;
			}
		}
		delete[] fgLayers;
		fgLayers = 0;
	}
}

/*
 * getNumBgLayers
 *
 * returns the number of background layers in the current level
 */
int LevelManager::getNumBgLayers() {
	return numBgLayers;
}

/*
 * getNumFgLayers
 *
 * returns the number of foreground layers in the current level
 */
int LevelManager::getNumFgLayers() {
	return numFgLayers;
}

/*
 * getBgLayers
 * 
 * returns an array of the background layers from the current level
 */
ScrollingLayer **LevelManager::getBgLayers() {
	return bgLayers;
}

/*
 * getFgLayers
 * 
 * returns an array of the foreground layers from the current level
 */
ScrollingLayer **LevelManager::getFgLayers() {
	return fgLayers;
}

/*
 * getMainground
 *
 * returns the layer directly underneath the player
 */
ScrollingLayer *LevelManager::getMainground() {
	return bgLayers[getNumBgLayers() - 1];
}

/*
 * getCurrentLevelNum
 *
 * returns the number of the current level. first level = level 0. maybe change this in the future
 */
int LevelManager::getCurrLevelNum() {
	return currLevelNum;
}

/*
 * getNumLevels
 *
 * returns the number of levels in the entire game
 */
int LevelManager::getNumLevels() {
	return numLevels;
}

/*
 * loadNextLevel
 *
 * loads the next level. if another level was previously loaded, gets rid of it before loading the new one.
 * returns true if it loaded succesfully, false otherwise
 */
bool LevelManager::loadNextLevel() {
	//make sure we don't try to load a level that doesnt exist
	if(++currLevelNum > getNumLevels()) {
		--currLevelNum;
		return false;
		//never reached
	}

	//if we already have a level loaded, get rid of it before loading in the new one
	if(bgLayers) {
		for(int i = 0; i < getNumBgLayers(); ++i) {
			if(bgLayers[i]) {
				delete bgLayers[i];
				bgLayers[i] = 0;
			}
		}
		delete[] bgLayers;
		bgLayers = 0;
	}

	//get rid of any previously loaded foreground layers
	if(fgLayers) {
		for(int i = 0; i < getNumFgLayers(); ++i) {
			if(fgLayers[i]) {
				delete fgLayers[i];
				fgLayers[i] = 0;
			}
		}
		delete[] fgLayers;
		fgLayers = 0;
	}

	std::ifstream levelInfo(levels[currLevelNum]);

	//get all info about speeding up scrolling part way through the level
	levelInfo >> speedUpX;
	levelInfo >> speedUpVal;
	spedUp = false;
	
	levelInfo >> numBgLayers;
	bgLayers = new ScrollingLayer*[numBgLayers];

	for(int i = 0; i < numBgLayers; ++i) {
		char layerFilename[256];
		levelInfo >> layerFilename;
		bgLayers[i] = new ScrollingLayer(layerFilename);
	}

	levelInfo >> numFgLayers;
	fgLayers = new ScrollingLayer*[numFgLayers];

	for(int i = 0; i < numFgLayers; ++i) {
		char layerFilename[256];
		levelInfo >> layerFilename;
		fgLayers[i] = new ScrollingLayer(layerFilename);
	}
	levelInfo.close();

	//load in the new textures
	if(FAILED(loadTextures())) {
		GameError("Error loading layers of level");
		return false;
	}

	PowerUpManager::Instance()->setDx(getMainground()->getScrollSpeed());
	EnemyManager::Instance()->setDx(getMainground()->getScrollSpeed());
	
	return true;
}

/*
 * scrollLayers
 *
 * updates world->screen coordinates for each layer
 */
void LevelManager::scrollLayers() {
	for(int i = 0; i < getNumBgLayers(); ++i) {
		bgLayers[i]->scrollWorld();
	}

	for(int i = 0; i < getNumFgLayers(); ++i) {
		fgLayers[i]->scrollWorld();
	}
}

void LevelManager::fadeLayers()
{
	for(int i = 0; i < getNumBgLayers(); ++i) 
	{
		bgLayers[i]->fadeLayer();
	}

	for(int i = 0; i < getNumFgLayers(); ++i) 
	{
		fgLayers[i]->fadeLayer();
	}
}

HRESULT LevelManager::loadTextures() {
	HRESULT r;

	//load each texture of each layer in the current level
	for(int i = 0; i < getNumBgLayers(); ++i) {
		r = bgLayers[i]->loadTextures();

		if(FAILED(r)) {
			return r;
		}
	}

	//load each texture of each layer in the current level
	for(int i = 0; i < getNumFgLayers(); ++i) {
		r = fgLayers[i]->loadTextures();

		if(FAILED(r)) {
			return r;
		}
	}

	return r;
}

HRESULT LevelManager::drawBgLayers(int alpha) {
	//get a handle to the instance of the sprite manager
	SpriteManager *spriteMgr = SpriteManager::Instance();

	HRESULT r;

	//for each layer
	for(int layerNum = 0; layerNum < getNumBgLayers() - 1; ++layerNum) {
		r = bgLayers[layerNum]->draw(alpha);

		if(FAILED(r)) {
			GameError("error drawing layer");
			return r;
		}
	}

	return r;
}

HRESULT LevelManager::drawMgLayer(int alpha) {
	//get a handle to the instance of the sprite manager
	SpriteManager *spriteMgr = SpriteManager::Instance();

	HRESULT r = bgLayers[getNumBgLayers() - 1]->draw(alpha);

	if(FAILED(r)) {
		GameError("error drawing layer");
		return r;
	}

	return r;
}

HRESULT LevelManager::drawFgLayers(int alpha) {
	//get a handle to the instance of the sprite manager
	SpriteManager *spriteMgr = SpriteManager::Instance();

	HRESULT r = S_OK;

	//for each layer
	for(int layerNum = 0; layerNum < getNumFgLayers(); ++layerNum) {
		r = fgLayers[layerNum]->draw(alpha);

		if(FAILED(r)) {
			GameError("error drawing layer");
			return r;
		}
	}

	return r;
}

void LevelManager::update(float dt) {
	scrollLayers();
	fadeLayers();

	//update bg layers
	for(int i = 0; i < getNumBgLayers(); ++i) {
		bgLayers[i]->update(dt);
	}

	//update fg layers
	for(int i = 0; i < getNumFgLayers(); ++i) {
		fgLayers[i]->update(dt);
	}

	//if we're at the part of the leve that needs to be sped up, do so for all layers
	if(getMainground()->getEndX() == speedUpX && !spedUp) {
		for(int i = 0; i < getNumBgLayers(); ++i) {
			bgLayers[i]->speedUp(speedUpVal);
		}

		//update fg layers
		for(int i = 0; i < getNumFgLayers(); ++i) {
			fgLayers[i]->speedUp(speedUpVal);
		}
		spedUp = true;
		PowerUpManager::Instance()->setDx(getMainground()->getScrollSpeed());
		EnemyManager::Instance()->setDx(getMainground()->getScrollSpeed());
	}

	//if at the end of the level, return the bg layers to their original scrolling speed
	if(getMainground()->isDoneScrolling() && spedUp) {
		for(int i = 0; i < getNumBgLayers(); ++i) {
			bgLayers[i]->speedUp(0 - speedUpVal);
		}

		//update fg layers
		for(int i = 0; i < getNumFgLayers(); ++i) {
			fgLayers[i]->speedUp(0 - speedUpVal);
		}
		spedUp = false;
		EnemyManager::Instance()->setDx(0);
		PowerUpManager::Instance()->setDx(0);
	}
}

bool LevelManager::isSpedUp() {
	return spedUp;
}