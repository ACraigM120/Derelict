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

#ifndef INTRO_MANAGER_H
#define INTRO_MANAGER_H

#include "GameCore.h"
#include "Texture.h"
#include "SpriteManager.h"

class IntroManager {
public:
	//ctor/dtor
	IntroManager(char *filename);
	~IntroManager();

	bool update(float timePassed);
	Texture *getCurrTexture();
	int getNumTextures();
	HRESULT loadTextures();
	HRESULT draw();
	bool nextScreen();

private:
	void loadTextureInfo(char *filename);
	
	Texture *screens;
	int numScreens;
	int currScreen;
	float secsPerScreen;  //how long each screen gets shown
	float currScreenTime; //how long the current screen's been displayed
};

#endif