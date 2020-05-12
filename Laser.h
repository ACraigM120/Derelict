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

// DefaultGun.h -- header file for the DefaultGun class
// This is the basic weapon that the players starts with.
//
#ifndef LASER_H
#define LASER_H

#include "Weapon.h"
#include "SoundManager.h"

#define LASER_DELAY 1.5f  //amount of seconds between each bullet being fired
#define LASER_SIZE 32 // The size of the bullet
#define ANIMATE_SPEED_LASER 0.1 

// DefaultGun class is derived from Weapon
class Laser : public Weapon
{
public:
	
	//ctor/dtor
	Laser(char* texFile);
	~Laser();

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