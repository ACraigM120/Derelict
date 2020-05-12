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

#include "Weapon.h"
#include <stdio.h>

Weapon::Weapon(char* texFile)
{
	// setup texture
	weaponTexture.filename = texFile;
	timeElapsed = 0.0f;
	upgraded = false;
}

Weapon::~Weapon()
{
	if(weaponTexture.texture) 
	{
		weaponTexture.texture->Release();
		weaponTexture.texture = 0;
	}
}

void Weapon::reset() 
{
	std::vector<BulletData>::iterator iter = ammoPos.begin();
	while(iter != ammoPos.end()) 
	{
		ammoPos.erase(iter);
	}
}

/*
 * getNumAmmo
 *
 * returns the number of instances of this weapon currently on the screen
 * (size of ammoPos vector)
 *
 */
int Weapon::getNumAmmo()
{
	return (int)ammoPos.size();
}

/*
 * getPos
 *
 * returns the position(s) of this weapon on the screen
 *
 */
POINT Weapon::getPos(int index)
{	
   return ammoPos[index].position;
}

POINT Weapon::getLastPos(int index)
{
	return ammoPos[index].lastPosition;
}

void Weapon::setLastPos(POINT pos, int index)
{
	ammoPos[index].lastPosition = pos;
}

/*
 * getWeaponStrength
 *
 * returns the strength of this weapon
 */
float Weapon::getWeaponStrength() {
	return weaponStrength;
}

/*
 * getRect
 *
 * returns the Rect representing the weapon texture.
 */
RECT Weapon::getRect() {
	return srcRect;
}

/*
 * getAnimateRect
 *
 * returns the RECT representing the current frame of animation for the weapons
 * 
 */
RECT Weapon::getAnimateRect(int index)
{	
   return ammoPos[index].animateRect;
}

/*
 * getMyTransform
 *
 * returns the matrix representing the transform of the ammo.
 * 
 */
D3DXMATRIX Weapon::getMyTransform(int index)
{	
   return ammoPos[index].transform;
}


HRESULT Weapon::loadTexture() {
	return SpriteManager::Instance()->loadTexture(&weaponTexture);
}

std::vector<BulletData> *Weapon::getAmmoPos() {
	return &ammoPos;
}

/*
 * getBoundingPoints
 *
 * gets the bounding points (polygon armature) for this weapon
 *
 * returns pointer to this weapon's polygon armature
 */
std::vector<POINT> *Weapon::getBoundingPoints() {
	return &boundingPoints;
}

void Weapon::upgrade() {
	upgraded = true;
}

void Weapon::unUpgrade() {
	upgraded = false;
}