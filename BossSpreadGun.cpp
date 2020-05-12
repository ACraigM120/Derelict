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

#include "BossSpreadGun.h"

// constructor & destructor
BossSpreadGun::BossSpreadGun(char* texFile) : Weapon(texFile)
{
	//setup the texture
	weaponTexture.texture = 0;

	//setup the initial source rect
   // NOTE, for the spreadshot the initial
   // source rect is for the first bullet 
	srcRect.top = 12;
	srcRect.bottom = 20;
	srcRect.left = 12;
	srcRect.right = 20;

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
	p.x = -2 * speed; p.y = 0;
	boundingPoints.push_back(p);
}

BossSpreadGun::~BossSpreadGun()
{

}


/*
 * Draws the weapon
 *
 * @returns - HRESULT
 */
HRESULT BossSpreadGun::draw(int alpha) 
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
void BossSpreadGun::update(float dt)
{
	std::vector<BulletData>::iterator iter = ammoPos.begin();

	while(iter != ammoPos.end()) 
	{
      // Animate the bullets until it has reached the last frame of animation (width of texture)
      if ( (*iter).animateTimer >= BOSS_ANIMATE_SPEED_SPREAD && (*iter).animateRect.right != weaponTexture.width )
      {
         (*iter).animateRect.left = ((*iter).animateRect.left + BOSS_SPREAD_BULLET_SIZE) % weaponTexture.width;
         (*iter).animateRect.right = ((*iter).animateRect.right % (weaponTexture.width)) + BOSS_SPREAD_BULLET_SIZE;
         (*iter).animateTimer = 0.0f;
      }
      (*iter).animateTimer += dt;

      // The source rect needs to be updated
      // based on the current frame of animation
      // since the bullets get bigger as they animate
      //
      switch( (*iter).animateRect.right )
      {
      case 32:   //8 16 24 32
         (*iter).srcRect.top = 12;
	      (*iter).srcRect.bottom = 20;
	      (*iter).srcRect.left = 12;
	      (*iter).srcRect.right = 20;
         break;
      case 64:
      	(*iter).srcRect.top = 10;
	      (*iter).srcRect.bottom = 22;
	      (*iter).srcRect.left = 10;
	      (*iter).srcRect.right = 22;
         break;
      case 96:
      	(*iter).srcRect.top = 8;
	      (*iter).srcRect.bottom = 24;
	      (*iter).srcRect.left = 8;
	      (*iter).srcRect.right = 24;
         break;
      case 128:
      	(*iter).srcRect.top = 6;
	      (*iter).srcRect.bottom = 26;
	      (*iter).srcRect.left = 6;
	      (*iter).srcRect.right = 26;
         break;
      }

      // Move the bullet up
      if ( (*iter).direction == 1 )
      {
         (*iter).position.y -= (long)(speed/4);
      }else if((*iter).direction == -1 ) {
		(*iter).position.y += (long)(speed/4);
	  }      

     (*iter).position.x -= (long)speed;

      // Make sure we haven't gone outside the viewable window of the game
      //
     if(((*iter).position.x ) < (0 - BOSS_SPREAD_BULLET_SIZE) || ((*iter).position.y) < (0 - BOSS_SPREAD_BULLET_SIZE || (*iter).position.y > WINDOW_HEIGHT )) 
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


void BossSpreadGun::fire(POINT startPos)
{
	//makes sure enough time has passed since the last bullet was fired before firing another one.
	//this is to make sure the firing animation looks more realistic, rather than a single
	//stationary line going across the screen
   // Get the starting position
	POINT pos = startPos;
	BulletData ammoInf;

	// offset a bit forward/up
	pos.x -= 56;
   pos.y -= 20;

   // Set the type
   ammoInf.type = NORMAL;

   // Setup the animation timer and
   // source rect for the animation (which is always the full size)
   RECT initialRect;
   initialRect.top = 0;
	initialRect.bottom = 32;
	initialRect.left = 0;
	initialRect.right = 32;
   
   ammoInf.animateRect = initialRect;  // The animation source rect
   ammoInf.srcRect = srcRect;          // The collision detection source rect.
   ammoInf.animateTimer = 0.0;

	// top spread bullet
   ammoInf.direction = 1;
   ammoInf.position = pos;
   ammoPos.push_back( ammoInf );

   if(upgraded) {
		// top middle spread bullet
		ammoInf.direction = 0;
		ammoInf.position = pos;
		ammoInf.position.y += 32;
		ammoPos.push_back( ammoInf );

	 	// bottom middle spread bullet
		ammoInf.direction = 0;
		ammoInf.position = pos;
		ammoInf.position.y -= 32;
		ammoPos.push_back( ammoInf );
   }else {
		// bottom middle spread bullet
		ammoInf.direction = 0;
		ammoInf.position = pos;
		ammoPos.push_back( ammoInf );
   }

	// bottom spread bullet
   ammoInf.direction = -1;
   ammoInf.position = pos;
   ammoPos.push_back( ammoInf );

	SoundManager::Instance()->playSoundEffect( "audio/sfx/s_spread_shot.mp3" );
}
