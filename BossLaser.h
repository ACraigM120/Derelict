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

// BossLaser.h -- header file for the DefaultGun class
// This is the basic weapon that the players starts with.
//
#ifndef BOSS_LASER_H
#define BOSS_LASER_H

#include "Weapon.h"
#include "SoundManager.h"

#define BOSS_LASER_DELAY 1.5f  //amount of seconds between each bullet being fired
#define BOSS_LASER_HEIGHT 192 // The height of the bullet
#define BOSS_LASER_WIDTH 128  // the width of the bullet
#define BOSS_ANIMATE_SPEED_LASER 0.1 

// DefaultGun class is derived from Weapon
class BossLaser : public Weapon
{
public:
	
	//ctor/dtor
	BossLaser(char* texFile);
	~BossLaser();

	// additional default gun-specific functions
	void update(float dt);
	
	float getSpeed();
	void setSpeed(float newSpeed);
	void fire(POINT startPos);

	HRESULT draw(int alpha = -1);

private:
	
	float speed;	// speed of the bullet (pixels per frame?)
};
#endif