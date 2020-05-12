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

// Ship.h -- header file for the Player class
#ifndef SHIP_H
#define SHIP_H

#include "GameCore.h"
#include "Texture.h"
#include "SpriteManager.h"
#include "BinManager.h"
#include <vector>

#define FLASH_TIME .05f  //how long a ship will stay red when it gets hit
struct Bin;

class Ship
{
public:
	//ctor/dtor
	Ship(int width, int height);
	~Ship();

	//accessors/mutators
	int getPosX();
	int getPosY();
	virtual void setPosX(int x) = 0;
	virtual void setPosY(int y) = 0;
	void setRelX(int deltaX);
	void setRelY(int deltaY);
	int getSpriteWidth();
	int getSpriteHeight();

	int getMyQuad();					         // returns this ship's screen quadrant (1, 2, 3, 4)
	int getRectQuad(RECT* rect);		         // returns the quadrant of this rect
	void getBounds(RECT* rect);		 // returns the bounding rect of a ship
	std::vector<POINT> *getBoundingPoints();     // get points of this ship's polygon armature

	//sprite related functions
	virtual HRESULT draw(int alpha = -1) = 0;

	//life related functions
	virtual void damage(float strength) = 0;
	void setLife(float newLife);
	bool isDead();
	float getLife();

	void displayPosition(LPD3DXFONT &font);
	void setVisible(bool vis);

	void reBin();

// use protected here because classes will be derived from this one
protected:
	Texture texture;		//texture for the player
	RECT sourceRect;		//source rect (which part of the texture should currently be drawn)

	int posX;
	int posY;
	int spriteWidth;           //the size of the side of a ship in pixels
	int spriteHeight;
	float life;
	bool dead;
	bool visible;
	float hitTimer;
	std::vector<POINT> boundingPoints; //polygon armature

	std::vector<Bin*> myBins;	// vector of bins that this ship is in, int is an index into worldBins
};
#endif