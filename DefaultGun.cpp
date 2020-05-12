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

#include "DefaultGun.h"

// constructor & destructor
DefaultGun::DefaultGun(char* texFile) : Weapon(texFile)
{
	//setup the texture
	weaponTexture.texture = 0;

	//setup the source rect
	srcRect.top = 0;
	srcRect.bottom = 15;
	srcRect.left = 0;
	srcRect.right = 15;

	speed = 12;
	weaponStrength = 0.25f;

	//setup polygon armature
	POINT p;
	p.x = 0; p.y = 2;
	boundingPoints.push_back(p);
	p.x = 15;
	boundingPoints.push_back(p);
	p.y = 6;
	boundingPoints.push_back(p);
	p.x = 0;
	boundingPoints.push_back(p);
	p.x = -2 * speed; p.y = 0;
	boundingPoints.push_back(p);
}

DefaultGun::~DefaultGun()
{

}


/*
 * Draws the weapon
 *
 * @returns - HRESULT
 */
HRESULT DefaultGun::draw(int alpha) 
{
	HRESULT r = S_OK;
	
	for(int i = 0; i < getNumAmmo(); i++) 
	{
		r = SpriteManager::Instance()->drawSprite(&weaponTexture,
												  &srcRect,
												  (float)getPos(i).x,
												  (float)getPos(i).y);

		if(FAILED(r)) {
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
void DefaultGun::update(float dt)
{
	std::vector<BulletData>::iterator iter = ammoPos.begin();

	while(iter != ammoPos.end()) 
	{
      if(((*iter).position.x += (long)speed) > WINDOW_WIDTH) 
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


void DefaultGun::fire(POINT startPos)
{
	//makes sure enough time has passed since the last bullet was fired before firing another one.
	//this is to make sure the firing animation looks more realistic, rather than a single
	//stationary line going across the screen
	if(timeElapsed > DEFAULT_BULLET_DELAY)
	{
		POINT pos = startPos;
      BulletData ammoInf;
      
      // Set the type
      ammoInf.type = NORMAL;

      // offset a bit forward
		pos.x += 10;

		// top bullet
		pos.y -= 24;
		
      ammoInf.position = pos;
      ammoInf.srcRect = srcRect;
      ammoPos.push_back( ammoInf );

		// reset pos
		pos.y = startPos.y;

		// bottom bullet
		pos.y += 12;

      ammoInf.position = pos;
		ammoPos.push_back( ammoInf );

		SoundManager::Instance()->playSoundEffect( "audio/sfx/s_default_gun.wav" );

		timeElapsed = 0.0f;
	}
}