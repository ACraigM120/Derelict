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

#include "Ship.h"
#include "GameCore.h"
#include <stdio.h>

// constructor
Ship::Ship(int width, int height) 
{
	spriteWidth = width;
	spriteHeight = height;
	sourceRect.left = 0;
	sourceRect.top = 0;
	sourceRect. right = spriteWidth;
	sourceRect.bottom = spriteHeight;
	dead = false;
	hitTimer = 0.0f;
}

// destructor
Ship::~Ship() 
{
}

int Ship::getPosX() 
{
	return posX;
}

int Ship::getPosY() 
{
	return posY;
}

void Ship::setRelX(int deltaX) 
{
	setPosX(posX + deltaX);
}

void Ship::setRelY(int deltaY) 
{
	setPosY(posY + deltaY);
}

void Ship::displayPosition(LPD3DXFONT &font)
{
	RECT rect =  {20, 60, 200, 200};
	RECT rect2 = {20, 80, 200, 200};

	char positionX[256];
	char positionY[256];

	sprintf(positionX, "Player X Position: %d", getPosX());
	sprintf(positionY, "Player Y Posiiton: %d", getPosY());
    
	font->DrawText(NULL, positionX, -1, &rect, DT_NOCLIP, D3DCOLOR_XRGB(255,255,255));
	font->DrawText(NULL, positionY, -1, &rect2, DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}

int Ship::getMyQuad()
{
	if(getPosX() <= (WINDOW_WIDTH/2) )
	{
		if(getPosY() <= (WINDOW_HEIGHT/2))
		{
			//GameError("i am in quadrant 1");
			return 1;
		}
		else
		{
			//GameError("i am in quadrant 3");
			return 3;
		}
	}
	else
	{
		if(getPosY() <= (WINDOW_HEIGHT/2))
		{
			//GameError("i am in quadrant 2");
			return 2;
		}
		else
		{
			//GameError("i am in quadrant 4");
			return 4;
		}
	}
}

int Ship::getRectQuad(RECT* rect)
{
	if(rect->left <= (WINDOW_WIDTH/2) )
	{
		if(rect->top <= (WINDOW_HEIGHT/2))
		{
			GameError("rect is in quadrant 1");
			return 1;
		}
		else
		{
			GameError("rect is in quadrant 3");
			return 3;
		}
	}
	else
	{
		if(rect->top <= (WINDOW_HEIGHT/2))
		{
			GameError("rect is in quadrant 2");
			return 2;
		}
		else
		{
			GameError("rect is in quadrant 4");
			return 4;
		}
	}
}

/*
 * isDead
 * 
 * is this enemy dead?
 *
 * returns true if this enemy's dead, false otherwise
 */
bool Ship::isDead() {
	return dead;
}

/*
 * setLife
 *
 * sets how much life this ship has left
 *
 * param - newLife - new life value
 */
void Ship::setLife(float newLife) {
	life = newLife;
}

/*
 * getLife
 *
 * gets how much life this ship has left
 *
 * returns how much life this ship has left
 */
float Ship::getLife() {
	return life;
}

void Ship::setVisible(bool vis) {
	//visible = vis;
	if(vis) {
		texture.red = texture.green = texture.blue = 255;
	}else {
		texture.green = texture.blue = 0;
	}
}

/*
 * getBoundingPoints
 *
 * gets the points of this ship's polygon armature (ship relative)
 *
 * returns points of this ship's polygon armature
 */
std::vector<POINT> *Ship::getBoundingPoints() {
	return &boundingPoints;
}

/*
 * reBin
 *
 * re-populates my vector of bin indexes
 *
 */
void Ship::reBin()
{
	//char buffer[250];
	Bin* bin;
	POINT pos;
	
	// clear out vector
	std::vector<Bin*>::iterator iter = myBins.begin();
	while(iter != myBins.end())
	{
		myBins.erase(iter);
	}

	// re-populate vector
	for(int i = 0; i < (int)boundingPoints.size(); i++)
	{
		pos = boundingPoints[i];
		pos.x += getPosX();
		pos.y += getPosY();

		bin = BinManager::Instance()->checkBin(pos);
		
		myBins.push_back(bin);
	}
	
	std::vector<Bin*>::iterator iter2 = myBins.begin();
	while(iter2 != myBins.end())
	{
		(*iter2)->shipsInMe.push_back(this);
		++iter2;
	}
	
	
}

void Ship::getBounds(RECT* rect)
{
	// Win32 function
	SetRect(rect, getPosX(), getPosY(), getPosX()+ spriteWidth, getPosY()+ spriteHeight);
}


int Ship::getSpriteWidth() {
	return spriteWidth;
}

int Ship::getSpriteHeight() {
	return spriteHeight;
}