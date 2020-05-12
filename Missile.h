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

#ifndef MISSILE_H
#define MISSILE_H

#include "Weapon.h"
#include "SoundManager.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "TrailEmitter.h"
#include <math.h>
#include <cmath>

#define MISSILE_DELAY 1.25f  //amount of seconds between each bullet being fired
#define MISSILE_RANGE 150   // The distance in which a missile can see it's target.
#define MISSILE_SIZE 16 // the size of the missile
#define MISSILE_TRACK_RATE 1.0 // The missile tracking rate.
#define ANIMATE_SPEED_MISSILE 0.2 

class Missile : public Weapon
{
public:
	
	//ctor/dtor
	Missile(char* texFile);
	~Missile();

	// additional missile-specific functions
	void update(float dt);
	
	float getSpeed();
	void setSpeed(float newSpeed);
	void fire(POINT startPos);
    POINT getEnemy( POINT missilePosition );

	HRESULT draw(int alpha = -1);

	void reAim(BulletData* missile);

private:
	
	float speed;
};
#endif