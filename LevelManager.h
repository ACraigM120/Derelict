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

#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "GameCore.h"
#include "ScrollingLayer.h"

class LevelManager {
public:
	//ctor/dtor
	LevelManager(char *filename);
	~LevelManager();

	int getNumBgLayers();
	int getNumFgLayers();
	ScrollingLayer **getBgLayers();
	ScrollingLayer **getFgLayers();
	ScrollingLayer *getMainground();

	bool loadNextLevel();
	bool isSpedUp();
	int getCurrLevelNum();
	int getNumLevels();

	void update(float dt);
	void scrollLayers();
	void fadeLayers();
	HRESULT drawBgLayers(int alpha = -1);
	HRESULT drawFgLayers(int alpha = -1);
	HRESULT drawMgLayer(int alpha = -1);
	void reset();

private:
	HRESULT loadTextures();  //loads the texture of each layer
	int numBgLayers;
	int numFgLayers;
	ScrollingLayer **bgLayers;
	ScrollingLayer **fgLayers;
	int numLevels;
	char **levels;
	int currLevelNum;
	int speedUpX;      //start position of where to speed up (in tiles)
	int speedUpVal;    //how much to speed up
	bool spedUp;       //has the new speed been applied yet?
};

#endif