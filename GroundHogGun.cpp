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

#include "GroundHogGun.h"

// constructor & destructor
GroundHogGun::GroundHogGun(char* texFile) : Weapon(texFile)
{
	//setup the texture
	weaponTexture.texture = 0;

	//setup the source rect
	srcRect.top = 0;
	srcRect.bottom = 8;
	srcRect.left = 0;
	srcRect.right = 8;

	speed = 10;
	weaponStrength = 0.5f;

	//setup poly armature
	POINT p;
	p.x = 6; p.y = 6;
	boundingPoints.push_back(p);
	p.x = 19;
	boundingPoints.push_back(p);
	p.y = 24;
	boundingPoints.push_back(p);
	p.x = 6;
	boundingPoints.push_back(p);
}

GroundHogGun::~GroundHogGun()
{

}


/*
 * Draws the weapon
 *
 * @returns - HRESULT
 */
HRESULT GroundHogGun::draw(int alpha) 
{
	HRESULT r = S_OK;
	for(int i = 0; i < getNumAmmo(); i++) 
	{
		r = SpriteManager::Instance()->drawSprite(&weaponTexture,
												  &srcRect,
												  (float)getPos(i).x,
												  (float)getPos(i).y,
												  alpha);

		if(FAILED(r)) 
		{
			return r;
		}
	}

	return r;
}


/*
 * update
 *
 * updates the position of the bullets currently in the air
 *
 * args  dt  how much time passed since the last frame (in seconds). useful for animation of bullets
 */
void GroundHogGun::update(float dt)
{
	std::vector<BulletData>::iterator iter = ammoPos.begin();

	while(iter != ammoPos.end()) 
	{


     // Move the bullet up
	 (*iter).position.y -= (long)(speed/2);
	 (*iter).position.x -= (long)(speed/2);

      // Make sure we haven't gone outside the viewable window of the game
      //
      if(((*iter).position.x ) < 0 || ((*iter).position.y) < (0 - GROUND_BULLET_SIZE)) 
		{
			ammoPos.erase(iter);

		}
		else 
		{
			++iter;
		}
	}

	timeElapsed += dt;
}


void GroundHogGun::fire(POINT startPos)
{

	
		// Get the starting position
		POINT pos = startPos;
	    BulletData ammoInf;

		// offset a bit forward/up
		pos.x -= 7;
		//pos.y -= 20;

		// Set the type
		ammoInf.type = NORMAL;

		// Setup the animation timer and
		// source rect for the animation (which is always the full size)
		RECT initialRect;
		initialRect.top = 0;
		initialRect.bottom = 8;
		initialRect.left = 0;
		initialRect.right = 8;
      
		ammoInf.srcRect = srcRect;          // The collision detection source rect.
		ammoInf.animateTimer = 0.0;

		ammoInf.position = pos;
		ammoPos.push_back( ammoInf );

		//SoundManager::Instance()->playSoundEffect( "audio/sfx/s_spread_shot.mp3" );
		timeElapsed = 0.0f;
}