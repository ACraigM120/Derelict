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

#ifndef BIN_MANAGER_H
#define BIN_MANAGER_H

#include "GameCore.h"
#include "Bin.h"
#include "Texture.h"
#include "SpriteManager.h"
#include <vector>


class BinManager
{
public:

	// ctor/dtor
	BinManager();
	~BinManager();

	// singleton?
	static BinManager* Instance();

	// a&m
	std::vector<Bin> *getWorldBins();
	Bin* getBin(int index);

	// other funcs
	void initBins(char* textureFile, char* textureFile2);		// initialize bin objects
	Bin* checkBin(POINT pos);				// check what bin the given point is in

	HRESULT loadTexture(char* textureFile, char* textureFile2);
	HRESULT drawBins();


private:

	std::vector<Bin> worldBins;
	Texture binTexture;
	Texture binTextureOcc;
	RECT binSrcRect;


};

#endif