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

#include "WaveGun.h"

// constructor & destructor
WaveGun::WaveGun(char* texFile) : Weapon(texFile)
{
	//setup the texture
	weaponTexture.texture = 0;

	//setup the source rect
	srcRect.top = 0;
	srcRect.bottom = WAVE_SIZE;
	srcRect.left = 0;
	srcRect.right = WAVE_SIZE;

	speed = 5;
	weaponStrength = 0.25f;
	maxDistance = 60;

	//setup poly armature
	POINT p;
	p.x = 0; p.y = 0;
	boundingPoints.push_back(p);
	p.x = 16;
	boundingPoints.push_back(p);
	p.y = 16;
	boundingPoints.push_back(p);
	p.x = 0;
	boundingPoints.push_back(p);
	p.x = -2 * speed; p.y = 0;
	boundingPoints.push_back(p);
}

WaveGun::~WaveGun()
{

}


/*
 * Draws the weapon
 *
 * @returns - HRESULT
 */
HRESULT WaveGun::draw(int alpha) 
{
	HRESULT r = S_OK;
	
	for(int i = 0; i < getNumAmmo(); i++) 
	{
		r = SpriteManager::Instance()->drawSprite(&weaponTexture,
												  &getAnimateRect(i),
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
void WaveGun::update(float dt)
{
	std::vector<BulletData>::iterator iter = ammoPos.begin();

	while(iter != ammoPos.end()) 
	{
      // Animate the bullets
      if ( (*iter).animateTimer >= ANIMATE_SPEED_WAVE )
      {
         (*iter).animateRect.left = ((*iter).animateRect.left + WAVE_SIZE) % weaponTexture.width;
         (*iter).animateRect.right = ((*iter).animateRect.right % (weaponTexture.width)) + WAVE_SIZE;
         (*iter).animateTimer = 0.0f;
      }
      (*iter).animateTimer += dt;

      // Move the bullet up
      if ( (*iter).direction == 1 )
      {
         (*iter).position.y -= (long)speed;
      }
      else {
         // Move the bullet down
         if ( (*iter).direction == -1 )
         {
            (*iter).position.y += (long)speed;
         }
      }
      
      // If we have traveled the max distance for this cycle
      // Then go in the other direction.
      if ( (*iter).distance == maxDistance )
      {
         (*iter).direction *= -1;
         (*iter).distance = 0;
      }

      // Increment distance traveled.
      (*iter).distance += (int)speed;

      // Increment along the x
      (*iter).position.x += (long)speed;

      // Make sure we haven't gone outside the viewable window of the game
      //
      if(((*iter).position.x ) > WINDOW_WIDTH || ((*iter).position.y) < (0 - WAVE_SIZE || (*iter).position.y > WINDOW_HEIGHT )) 
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


void WaveGun::fire(POINT startPos)
{
	//makes sure enough time has passed since the last bullet was fired before firing another one.
	//this is to make sure the firing animation looks more realistic, rather than a single
	//stationary line going across the screen
	if(timeElapsed > WAVE_DELAY)
	{
      // Get the starting position
		POINT pos = startPos;
	   BulletData ammoInf;

      // Set the type
      ammoInf.type = NORMAL;

		// offset a bit forward
		pos.x += 25;

      // Setup the animation timer and
      // source rect
      ammoInf.srcRect = srcRect;
      ammoInf.animateRect = srcRect;
      ammoInf.animateTimer = 0.0;

		// top wave bullet
      // Oscillates Up/Then Down.
		pos.y -= 26;
      ammoInf.direction = 1;
      ammoInf.distance = 0;
      ammoInf.position = pos;
      ammoPos.push_back( ammoInf );

		// bottom wave bullet
      // Oscillates Down/Then Up.
      pos.y = startPos.y;
		pos.y += 8;
      ammoInf.direction = -1;
		ammoInf.position = pos;
      ammoPos.push_back( ammoInf );

		SoundManager::Instance()->playSoundEffect( "audio/sfx/s_wave_gun.wav" );
		timeElapsed = 0.0f;
	}
}