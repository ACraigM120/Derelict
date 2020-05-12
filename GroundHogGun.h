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

#ifndef GROUNDHOG_GUN_H
#define GROUNDHOG_GUN_H

#include "Weapon.h"
#include "SoundManager.h"

#define GROUND_DELAY 0.1f  //amount of seconds between each bullet being fired
#define GROUND_BULLET_SIZE 8 // The size of the bullet
#define ANIMATE_SPEED_GROUND 0.2 

class GroundHogGun : public Weapon
{
public:
	
	//ctor/dtor
	GroundHogGun(char* texFile);
	~GroundHogGun();

	// additional bullet-specific functions
	void update(float dt);
	
	float getSpeed();
	void setSpeed(float newSpeed);
	void fire(POINT startPos);

	HRESULT draw(int alpha = -1);

private:
	float speed;	// speed of the bullet (pixels per frame?)
};
#endif