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

#include "ClamFighterGun.h"

// constructor & destructor
ClamFighterGun::ClamFighterGun(char* texFile) : Weapon(texFile)
{
	//setup the texture
	weaponTexture.texture = 0;

	//setup the source rect
	srcRect.top = 0;
	srcRect.bottom = ENEMY_GUN_SIZE;
	srcRect.left = 0;
	srcRect.right = ENEMY_GUN_SIZE;

	speed = 12;
	weaponStrength = 0.25f;

	//set up poly armature
	POINT p;
	p.x = 0; p.y = 0;
	boundingPoints.push_back(p);
	p.x = 8;
	boundingPoints.push_back(p);
	p.y = 8;
	boundingPoints.push_back(p);
	p.x = 0;
	boundingPoints.push_back(p);
}

ClamFighterGun::~ClamFighterGun()
{

}


/*
 * Draws the weapon
 *
 * @returns - HRESULT
 */
HRESULT ClamFighterGun::draw(int alpha) 
{
	HRESULT r = S_OK;
	
	for(int i = 0; i < getNumAmmo(); i++) 
	{
		r = SpriteManager::Instance()->drawSprite(&weaponTexture,
												  &getAnimateRect(i),
												  (float)getPos(i).x,
												  (float)getPos(i).y,
												  alpha);

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
void ClamFighterGun::update(float dt)
{
	std::vector<BulletData>::iterator iter = ammoPos.begin();

	while(iter != ammoPos.end()) 
	{
      // Animate the bullets
      if ( (*iter).animateTimer >= ENEMY_GUN_ANIMATE_SPEED )
      {
         (*iter).animateRect.left = ((*iter).animateRect.left + ENEMY_GUN_SIZE) % weaponTexture.width;
         (*iter).animateRect.right = ((*iter).animateRect.right % (weaponTexture.width)) + ENEMY_GUN_SIZE;
         (*iter).animateTimer = 0.0f;
      }
      (*iter).animateTimer += dt;

      if(((*iter).position.x -= (long)speed) < 0) 
		{
			ammoPos.erase(iter);
		}
		else 
		{
			++iter;
		}
	}
}


void ClamFighterGun::fire(POINT startPos)
{
	//makes sure enough time has passed since the last bullet was fired before firing another one.
	//this is to make sure the firing animation looks more realistic, rather than a single
	//stationary line going across the screen
	POINT pos = startPos;
   BulletData ammoInf;
      
   // Set the type
   ammoInf.type = NORMAL;

   // offset a bit forward
   pos.x += 10;

	// top bullet
	pos.y -= 24;

   // Setup the animation timer and
   // source rect
   ammoInf.animateRect = srcRect;
   ammoInf.srcRect = srcRect;
   ammoInf.animateTimer = 0.0;

   ammoInf.position = pos;
   ammoPos.push_back( ammoInf );

	// reset pos
	pos.y = startPos.y;

	// bottom bullet
	pos.y += 12;

   ammoInf.position = pos;
	ammoPos.push_back( ammoInf );

	SoundManager::Instance()->playSoundEffect( "audio/sfx/s_default_gun.mp3" );
}