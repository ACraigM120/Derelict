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

#include "Laser.h"

// constructor & destructor
Laser::Laser(char* texFile) : Weapon(texFile)
{
	//setup the texture
	weaponTexture.texture = 0;

	//setup the source rect
	srcRect.top = 0;
	srcRect.bottom = LASER_SIZE;
	srcRect.left = 0;
	srcRect.right = LASER_SIZE;

	speed = 10;
	weaponStrength = 0.2f;

	//setup poly armature
	POINT p;
	p.x = 0; p.y = 8;
	boundingPoints.push_back(p);
	p.x = 32;
	boundingPoints.push_back(p);
	p.y = 24;
	boundingPoints.push_back(p);
	p.x = 0;
	boundingPoints.push_back(p);
}

Laser::~Laser()
{

}


/*
 * Draws the weapon
 *
 * @returns - HRESULT
 */
HRESULT Laser::draw(int alpha) 
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
void Laser::update(float dt)
{
	std::vector<BulletData>::iterator iter = ammoPos.begin();

	while(iter != ammoPos.end()) 
	{
      // Animate the bullets until it has reached the last frame of animation (width of texture)
      if ( (*iter).animateTimer >= ANIMATE_SPEED_LASER && (*iter).animateRect.right != weaponTexture.width )
      {
         (*iter).animateRect.left = ((*iter).animateRect.left + LASER_SIZE) % weaponTexture.width;
         (*iter).animateRect.right = ((*iter).animateRect.right % (weaponTexture.width)) + LASER_SIZE;
         (*iter).animateTimer = 0.0f;
      }
      (*iter).animateTimer += dt;

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


void Laser::fire(POINT startPos)
{
	//makes sure enough time has passed since the last bullet was fired before firing another one.
	//this is to make sure the firing animation looks more realistic, rather than a single
	//stationary line going across the screen
	if(timeElapsed > LASER_DELAY)
	{
		POINT pos = startPos;
      BulletData ammoInf;
      
		// top bullet
		pos.y -= 18;
      pos.x += 36;

      // Set the type
      ammoInf.type = LASER;

      // Setup the animation timer and
      // source rect
      ammoInf.animateRect = srcRect;
      ammoInf.animateTimer = 0.0;

      ammoInf.srcRect = srcRect;
      ammoInf.position = pos;
      ammoPos.push_back( ammoInf );

      for (int i = 0; i < 25; i++ )
      {
         ammoInf.position.x+=32;
         ammoPos.push_back( ammoInf );
      }

      SoundManager::Instance()->playSoundEffect( "audio/sfx/s_laser_cannon.wav" );
		timeElapsed = 0.0f;
	}
}