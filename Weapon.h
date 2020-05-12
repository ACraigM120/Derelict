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

#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "GameCore.h"
#include "Texture.h"
#include "SpriteManager.h"
#include "BulletData.h"
#include <vector>

// base class for most all weapon types
class Weapon
{
public:

	// c & d
	Weapon(char* texFile);
	Weapon();
	~Weapon();
	
	// a & m
	int getNumAmmo();
	POINT getPos(int index);
	POINT getLastPos(int index);
	void setPos(POINT pos);
	void setLastPos(POINT pos, int index);
	float getWeaponStrength();
	std::vector<BulletData> *getAmmoPos();
	std::vector<POINT> *getBoundingPoints();
	void reset();

	virtual void update(float dt) {}
	virtual void fire(POINT startPos) {}
	virtual HRESULT draw(int alpha = -1) = 0;
	void upgrade();
	void unUpgrade();

	RECT getRect();
	RECT getAnimateRect(int index);
	D3DXMATRIX getMyTransform(int index);

	HRESULT loadTexture();

	
protected:

	Texture weaponTexture;
	float timeElapsed;  //amount of time (in seconds) elapsed since the last bullet was fired

	std::vector<BulletData> ammoPos;		// holds position POINTs for all instances of this weapon
	RECT srcRect;
	float weaponStrength;
	std::vector<POINT> boundingPoints;  //polygon armature
	bool upgraded;
};

#endif