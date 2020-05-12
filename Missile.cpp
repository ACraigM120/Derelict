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

#include "Missile.h"

// constructor & destructor
Missile::Missile(char* texFile) : Weapon(texFile)
{
	//setup the texture
	weaponTexture.texture = 0;

	//setup the source rect
	srcRect.top = 0;
	srcRect.bottom = MISSILE_SIZE;
	srcRect.left = 0;
	srcRect.right = MISSILE_SIZE;

	speed = 8.0;
	weaponStrength = 5.0f;

	//setup poly armature
	POINT p;
	p.x = 0; p.y = 7;
	boundingPoints.push_back(p);
	p.x = 12;
	boundingPoints.push_back(p);
	p.y = 10;
	boundingPoints.push_back(p);
	p.x = 0;
	boundingPoints.push_back(p);
	p.x = -2 * speed; p.y = 0;
	boundingPoints.push_back(p);
}

Missile::~Missile()
{

}


/*
 * Draws the weapon
 *
 * @returns - HRESULT
 */
HRESULT Missile::draw(int alpha) 
{
	HRESULT r = S_OK;
	
	for(int i = 0; i < getNumAmmo(); i++) 
	{

		// draw missile at lastPos!!
		
		POINT center;
		center.x = getPos(i).x+(MISSILE_SIZE/2);
		center.y = getPos(i).y+(MISSILE_SIZE/2);
		
		r = SpriteManager::Instance()->drawSprite(&weaponTexture,
												  &srcRect,
												  getPos(i),
												  &getMyTransform(i),
												  center);

		if(FAILED(r)) 
		{
			GameError("MISSILE FAILED");
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
void Missile::update(float dt)
{
	int index = 0;
	
	// Iterator for the missiles.
	std::vector<BulletData>::iterator iter = ammoPos.begin();

	// While a missile has been fired
	while(iter != ammoPos.end()) 
	{
      if ( (*iter).animateTimer >= ANIMATE_SPEED_MISSILE )
      {
         (*iter).animateRect.left = ((*iter).animateRect.left + MISSILE_SIZE) % weaponTexture.width;
         (*iter).animateRect.right = ((*iter).animateRect.right % (weaponTexture.width)) + MISSILE_SIZE;
         (*iter).animateTimer = 0.0f;
      }
      (*iter).animateTimer += dt;

      // Target an enemy within range
      POINT enemyPos = getEnemy( (*iter).position );
      int targetX = enemyPos.x;
      int targetY = enemyPos.y;

      // If there isn't currently a target, send the missiles straight ahead
      //
      if ( targetX == 0 && targetY == 0 )
      {
         (*iter).position.x += (long)speed;
      }
      else 
	  {
         // Get the position of the missile
         int missileX = (*iter).position.x;
         int missileY = (*iter).position.y;

         // Get the difference from the target
         float vx = (float)( targetX - missileX );
         float vy = (float)( targetY - missileY );

         // Calculate the distance to target
         float length = sqrt( vx*vx + vy*vy );

         // Calculate velocity vector
         vx = (float)(MISSILE_TRACK_RATE*(vx/length));
         vy = (float)(MISSILE_TRACK_RATE*(vy/length));

		 // Update the velocity of the missile
         (*iter).velocityX += vx;
         (*iter).velocityY += vy;

         // Update the position of the missile.
         //
         (*iter).position.x += (long)((*iter).velocityX);
         (*iter).position.y += (long)((*iter).velocityY);

		  // reAim the missile to point at pos.... will be drawn at lastPos
		  // reAim(&(*iter));
	      D3DXVECTOR2 vec1;
	      vec1.x = missileX;
	      vec1.y = missileY;

	      D3DXVECTOR2 vec2;
	      vec2.x = targetX;
	      vec2.y = targetY;

         D3DXVec2Normalize( &vec1, &vec1);
         D3DXVec2Normalize( &vec2, &vec2);

	      float dotProd = D3DXVec2Dot(&vec1, &vec2);
         float crossProd = D3DXVec2CCW(&vec1, &vec2);
      	
	      // get the angle between the vectors
	      float angle = acos(dotProd);

         if ( crossProd < 0 )
         {
            (*iter).rotation = -angle;
         }
         else {
            (*iter).rotation = angle;
         }

     }

      // Set some local transform values
      //
      D3DXMATRIX myTransform;
   	  D3DXMATRIX rotationMatrix;
	  D3DXMATRIX translationMatrix;

      // set rotation matrix
      D3DXMatrixRotationZ(&rotationMatrix, (*iter).rotation );

      // set translation matrix (may not need this for dx9)
      D3DXMatrixTranslation(&translationMatrix, (float)((*iter).position.x), (float)((*iter).position.y), 0.0f);

      // concatenate matrices
      D3DXMatrixMultiply(&myTransform, &rotationMatrix, &translationMatrix); 

      // Update the current bullets transform
      (*iter).transform = myTransform;

      // Make sure we haven't gone outside the viewable window of the game
      //
      if(((*iter).position.x ) > WINDOW_WIDTH || ((*iter).position.y) < 0 - MISSILE_SIZE) 
		{
			ammoPos.erase(iter);
		}
		else 
		{
			++iter;
		}
	}

	index = 0;
	
	timeElapsed += dt;
}


void Missile::fire(POINT startPos)
{
	TrailEmitter* emitter;
	//makes sure enough time has passed since the last missile was fired before firing another one.
	if(timeElapsed > MISSILE_DELAY)
	{
		// Get the starting position
		POINT pos = startPos;
	    BulletData ammoInf;

		// Set the type
		ammoInf.type = NORMAL;

		// Setup the animation timer and
		// source rect
		ammoInf.srcRect = srcRect;
		ammoInf.animateRect = srcRect;
		ammoInf.animateTimer = 0.0;

		// offset a bit forward
		pos.x += 20;

		// Set up the first missile.
		pos.y -= 20;
		ammoInf.position = pos;
		ammoInf.lastPosition = pos;
		ammoInf.velocityX = 1;
		ammoInf.velocityY = 1;
		ammoInf.rotation = 0.0f;
		ammoPos.push_back( ammoInf );

		// Set up the second missile.
		pos = startPos;
		pos.x += 20;
		pos.y += 15;
		ammoInf.position = pos;
		ammoInf.lastPosition = pos;
		ammoInf.rotation = 0.0f;
		ammoPos.push_back( ammoInf );

		// Play the sound effect for this weapon.
		SoundManager::Instance()->playSoundEffect( "audio/sfx/s_missile.wav" );
		timeElapsed = 0.0f;

	}
}

/*
 * Returns the position of an enemy
 * vector of Enemies.
 *
 * @param - missilePosition - the current position of the missile.
 * @returns - the POINT position for the enemy.
 */
POINT Missile::getEnemy( POINT missilePosition )
{
   POINT enemyPosition;
   enemyPosition.x = 0;
   enemyPosition.y = 0;

   // Get the vector of enemies.
   std::vector<Enemy*> *enemyVector = EnemyManager::Instance()->getActiveEnemies();

   // Loop through all the current enemies, and go after the closest one.
   //
	for( int i = 0; i < (int)enemyVector->size(); ++i)
	{
      // Enemy.
      Enemy *enemyShip = (*enemyVector)[i];
      int tempX = enemyShip->getPosX();
      int tempY = enemyShip->getPosY();

      // Check to see which enemy is within range of the missile.
      // grab the closest one.
      //
      float dX = (float)(tempX - missilePosition.x);
      float dY = (float)(tempY - missilePosition.y);

      // Get the distance to the enemy
      float distanceToEnemy = sqrt( dX*dX + dY * dY );

      if ( distanceToEnemy < MISSILE_RANGE )
      {
         enemyPosition.x = tempX;
         enemyPosition.y = tempY;
      }
   }

   // Return the position of the enemy
   return enemyPosition;
}


void Missile::reAim(BulletData* missile)
{
	// rotate the lastPos to face the current pos

	D3DXVECTOR2 vec1;
	vec1.x = missile->position.x;
	vec1.y = missile->position.y;

	D3DXVECTOR2 vec2;
	vec2.x = missile->lastPosition.x;
	vec2.y = missile->lastPosition.y;

	//POINT vec1 = missile->position;
	//POINT vec2 = missile->lastPosition;

	float dotProd;
	float mags;

	// get the magnitude of both vectors
	// magnitude = sqrt((v1*v1)+(v2*v2))
	float magPos = sqrt((float)((vec1.x*vec1.x)+(vec1.y*vec1.y)));
	float magLastPos = sqrt((float)((vec2.x*vec2.x)+(vec2.y*vec2.y)));

	dotProd = D3DXVec2Dot(&vec1, &vec2);//(vec1.x*vec2.x)+(vec1.y*vec2.y);
	mags = magPos*magLastPos;
	
	// get the angle between the vectors
	// angle = acos((v1*v2)/(mag1*mag2))
	float angle = acos(dotProd/mags);

	missile->rotation += angle;
}
