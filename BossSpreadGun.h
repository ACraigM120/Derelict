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

#ifndef BOSS_SPREAD_GUN_H
#define BOSS_SPREAD_GUN_H

#include "Weapon.h"
#include "SoundManager.h"

#define BOSS_SPREAD_DELAY 0.1f  //amount of seconds between each bullet being fired
#define BOSS_SPREAD_BULLET_SIZE 32 // The size of the bullet
#define BOSS_ANIMATE_SPEED_SPREAD 0.2 

class BossSpreadGun : public Weapon
{
public:
	
	//ctor/dtor
	BossSpreadGun(char* texFile);
	~BossSpreadGun();

	// additional bullet-specific functions
	void update(float dt);
	void upgrade();
	
	float getSpeed();
	void setSpeed(float newSpeed);
	void fire(POINT startPos);

	HRESULT draw(int alpha = -1);

private:
	float speed;	// speed of the bullet (pixels per frame?)
};
#endif