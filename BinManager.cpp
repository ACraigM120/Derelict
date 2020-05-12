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

#include "BinManager.h"

BinManager::BinManager()
{
 
}

BinManager::~BinManager()
{
	if(binTexture.texture)
	{
		binTexture.texture->Release();
		binTexture.texture = 0;
	}

	if(binTextureOcc.texture)
	{
		binTextureOcc.texture->Release();
		binTextureOcc.texture = 0;
	}
}

BinManager *BinManager::Instance() 
{
	//instead of 'new'ing 'this' and returning the pointer, just return the address. this way we dont
	//have to worry about explicitly calling the destructor of 'this'
	static BinManager binMgr;
	return &binMgr; 
}

std::vector<Bin> *BinManager::getWorldBins()
{
	return &worldBins;
}

Bin* BinManager::getBin(int index)
{
	return &worldBins[index];
}

/*
 * initBins
 *
 * builds the worldBins vector with Bin objects and
 * initializes their rects and occupied state
 * 
 */
void BinManager::initBins(char* textureFile, char* textureFile2)
{
	//char buffer[250];
	int binIndex = 0;

	binSrcRect.top = 0;
	binSrcRect.left = 0;
	binSrcRect.bottom = 100;
	binSrcRect.right = 100;

	binTexture.texture = 0;
	binTextureOcc.texture = 0;
	
	// split the screen into an equal number of grids
	// screen is 800x600, there are 48 bins
	// each bin is 100x100, there are 8 columns of bins
	// and 6 rows of bins
	for(int i = 0; i < NUMBINS_H; i++)
	{
		for(int j = 0; j < NUMBINS_W; j++)
		{
			Bin bin;
			
			bin.binID = binIndex;
			bin.occupied = false;

			bin.myRect.top = i*100;
			bin.myRect.left = j*100;
			bin.myRect.bottom = bin.myRect.top+BINSIZE;
			bin.myRect.right = bin.myRect.left+BINSIZE;

			bin.myPos.x = bin.myRect.left;
			bin.myPos.y = bin.myRect.top;

			worldBins.push_back(bin);
			binIndex++;
		}

	}

	HRESULT r = loadTexture(textureFile, textureFile2);
	if(FAILED(r))
	{
		GameError("failed loading bin textures");
	}
}

/*
 * checkBin
 * 
 * returns which bin the given point is in
 *
 * parameters - pos -> position to check
 *
 * returns - reference to the bin the point is in
 *
 */
Bin* BinManager::checkBin(POINT pos)
{
	int x = pos.x;
	int y = pos.y;
	RECT rect;

	for(int i = 0; i < (int)worldBins.size(); i++)
	{
		rect = worldBins[i].myRect;
		worldBins[i].occupied = false;

		if(((x >= rect.left) && (x < rect.right)) && ((y >= rect.top) && (y < rect.bottom)))
		{
			return &worldBins[i];
		}
	}

	return NULL;
}

HRESULT BinManager::loadTexture(char *textureFile, char *textureFile2)
{
	HRESULT r;

	binTexture.filename = textureFile;
	binTextureOcc.filename = textureFile2;

	r = SpriteManager::Instance()->loadTexture(&binTexture);
	if(FAILED(r))
	{
		GameError("failed loading binTexture");
		return r;
	}

	r = SpriteManager::Instance()->loadTexture(&binTextureOcc);
	if(FAILED(r))
	{
		GameError("failed loading binTextureOcc");
		return r;
	}

	return r;
}

HRESULT BinManager::drawBins()
{
	HRESULT r;

	// iterate through bins and draw them according to occupied state
	std::vector<Bin>::iterator iter = worldBins.begin();
	while(iter != worldBins.end())
	{
		if((*iter).occupied)
		{
			r = SpriteManager::Instance()->drawSprite(&binTextureOcc, &binSrcRect, (float)((*iter).myPos.x), (float)((*iter).myPos.y));
			if(FAILED(r))
			{
				GameError("failed drawing occupied bin");
				return r;
			}
		}
		else
		{
			r = SpriteManager::Instance()->drawSprite(&binTexture, &binSrcRect, (float)((*iter).myPos.x), (float)((*iter).myPos.y));
			if(FAILED(r))
			{
				GameError("failed drawing occupied bin");
				return r;
			 
			}
		}

		++iter;
	}

	return NULL;
}