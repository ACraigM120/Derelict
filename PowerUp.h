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

#ifndef POWER_UP_H
#define POWER_UP_H

#include "GameCore.h"
#include "Ship.h"
#include "Texture.h"
#include "SoundManager.h"

class PowerUp : public Ship
{
public:

	//ctor/dtor
	PowerUp(Texture myTexture, int powerupId, int y, int spriteWidth, int spriteHeight);
	~PowerUp();


	// a&m
	void setPosX(int x);
	void setPosY(int y);
	int getID();
	void setPolygonArmature(std::vector<POINT> points);
	void setDx(int newDx);

	// funcs
	bool isCollected();
	bool isOffscreen();

	void damage(float strength);	// not used for PowerUp
	void collectMe();

	void update(float dt);
	HRESULT draw(int alpha = -1);


private:

	bool collected;
	
	// stuff for movement
	bool goingUp;
	int initialY;
	int maxY;
	int minY;
	int id;	
	int dx;

};

#endif