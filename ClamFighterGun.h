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

// ClamFighterGun.h -- header file for the Clam Fighters weapon class
//
#ifndef CLAMFIGHTER_GUN_H
#define CLAMFIGHTER_GUN_H

#include "Weapon.h"
#include "SoundManager.h"

#define ENEMY_GUN_SIZE 8               // The size of the bullet
#define ENEMY_GUN_ANIMATE_SPEED 0.2    // The animation speed

// DefaultGun class is derived from Weapon
class ClamFighterGun : public Weapon
{
public:
	
	//ctor/dtor
	ClamFighterGun(char* texFile);
	~ClamFighterGun();

	// additional default gun-specific functions
	void update(float dt);
	void fire(POINT startPos);
	HRESULT draw(int alpha = -1);

private:
	
	float speed;	// speed of the bullet (pixels per frame?)
};
#endif