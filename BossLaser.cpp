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

#include "BossLaser.h"

// constructor & destructor
BossLaser::BossLaser(char* texFile) : Weapon(texFile)
{
	//setup the texture
	weaponTexture.texture = 0;

	//setup the source rect
	srcRect.top = 0;
	srcRect.bottom = BOSS_LASER_HEIGHT;
	srcRect.left = 0;
	srcRect.right = BOSS_LASER_WIDTH;

	speed = 18;
	weaponStrength = 0.5f;

	//setup poly armature
	POINT p;
	p.x = 0; p.y = 0;
	boundingPoints.push_back(p);
	p.x = BOSS_LASER_WIDTH;
	boundingPoints.push_back(p);
	p.y = BOSS_LASER_HEIGHT;
	boundingPoints.push_back(p);
	p.x = 0;
	boundingPoints.push_back(p);
}

BossLaser::~BossLaser()
{

}


/*
 * Draws the weapon
 *
 * @returns - HRESULT
 */
HRESULT BossLaser::draw(int alpha) 
{
	HRESULT r = S_OK;
	
	for(int i = 0; i < getNumAmmo(); i++) 
	{
		r = SpriteManager::Instance()->drawSprite(&weaponTexture,
												  &getAnimateRect(i),
												  (float)getPos(i).x,
												  (float)getPos(i).y, alpha);

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
void BossLaser::update(float dt)
{
	std::vector<BulletData>::iterator iter = ammoPos.begin();

	while(iter != ammoPos.end()) 
	{
      // Animate the bullets until it has reached the last frame of animation (width of texture)
      if ( (*iter).animateTimer >= BOSS_ANIMATE_SPEED_LASER && (*iter).animateRect.right != weaponTexture.width )
      {
         (*iter).animateRect.left = ((*iter).animateRect.left + BOSS_LASER_WIDTH) % weaponTexture.width;
         (*iter).animateRect.right = (*iter).animateRect.left + BOSS_LASER_WIDTH;
         (*iter).animateTimer = 0.0f;
      }
      (*iter).animateTimer += dt;

      // Update the speed
      (*iter).position.x -= (long)speed;

      if((*iter).position.x  <  -BOSS_LASER_WIDTH) 
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


void BossLaser::fire(POINT startPos)
{
	//makes sure enough time has passed since the last bullet was fired before firing another one.
	//this is to make sure the firing animation looks more realistic, rather than a single
	//stationary line going across the screen
	POINT pos = startPos;
   BulletData ammoInf;
   
	// top bullet
	pos.y -= 32;
   //pos.x += 36;

   // Set the type
   ammoInf.type = LASER;

   // Setup the animation timer and
   // source rect
   ammoInf.animateRect = srcRect;
   ammoInf.animateTimer = 0.0;

   ammoInf.srcRect = srcRect;
   ammoInf.position = pos;
   ammoPos.push_back( ammoInf );

   for (int i = 0; i < 10; i++ )
   {
      ammoInf.position.x+=BOSS_LASER_WIDTH;
      ammoPos.push_back( ammoInf );
   }
   
   // Play a sound effect
   SoundManager::Instance()->playSoundEffect( "audio/sfx/laser.wav" );
}