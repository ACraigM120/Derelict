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

#include "Player.h"
#include "FXManager.h"
#include "SoundManager.h"
#include "Collision.h"

// constructor
Player::Player(char* texFile, int spriteWidth, int spriteHeight ) : Ship(spriteWidth, spriteHeight)
{
	// populate some texture information for the player
	texture.filename = texFile;
	texture.texture = 0;        // zero the texture pointer
	flameTexture.filename = "graphics/Flame.dds";
	flameTexture.texture = 0;
	
	// setup flame source rect
	flameSrcRect.top = 0;
	flameSrcRect.bottom = 18;
	flameSrcRect.left = 0;
	flameSrcRect.right = 32;

	//init polygon armature (hard coded..... maybe should put in file somewhere?)
	POINT p;
	p.x = 4;
	p.y = 4;
	boundingPoints.push_back(p);
	p.x = 54;
	boundingPoints.push_back(p);
	p.y = 56;
	boundingPoints.push_back(p);
	p.x = 4;
	boundingPoints.push_back(p);

	selectWeapon(0);

	//init player's state
	reset();
}

// destructor
Player::~Player() 
{
	// if a texture object has been assigned to the Player, release it
	if(texture.texture) 
	{
		char buf[250];
		sprintf(buf, "Releasing texture: %s", texture.filename);
		GameError(buf);
		texture.texture->Release();
		texture.texture = 0;
	}

	// same for flame
	if(flameTexture.texture) 
	{
		flameTexture.texture->Release();
		flameTexture.texture = 0;
	}
}

/*
 * reset
 *
 * puts the player in the initial state. basically, used every time a new game is started
 */
void Player::reset() {
	// initialize position
	posX = WINDOW_WIDTH/2;
	posY = WINDOW_HEIGHT/2;
	
    // For showing flames only when ship is moving forward
    showFlame = false;

	// begin with a default weapon
	selectWeapon(0);

	//init score & ship speed
	score = 0;
	speed = 3;
	life = MAX_PLAYER_LIFE;
	lives = startLives;
	gravityBombs = startGBombs;
	dead = false;
	respawning = false;
	secsToRespawn = RESPAWN_TIME;
	sourceRect.left = sourceRect.top = 0;
	sourceRect.right = spriteWidth;
	sourceRect.bottom = spriteHeight;

	weaponMgr.reset();
}

/*
 * Sets the x position of the player
 * 
 * @param - The x position
 * @returns - none
 */
void Player::setPosX(int x) 
{
	int dx = x - posX;
	posX = x;

   // Only show flames if the ship is moving forward
	if(dx <= 0) 
	{
		//moving left
		showFlame = false;
	}
	else if(dx > 0) 
	{
		//moving right
	    showFlame = true;
	}

	// check bounds for left edge of screen
	if(posX < 0) 
	{
		posX = 0;
	}
	
	// check bounds for right edge
	else if(posX > WINDOW_WIDTH - spriteWidth) 
	{
		posX = WINDOW_WIDTH - spriteWidth;
	}
}

/*
 * Sets the y position of the player
 * 
 * @param - The y position
 * @returns - none
 */
void Player::setPosY(int y) 
{
	int dy = y - posY;
	posY = y;
	
	//check to see which frame of ship to draw
	if(dy < 0) 
	{
		//going up
		sourceRect.left = spriteWidth;
		sourceRect.right = sourceRect.left + spriteWidth;
	}
	else if(dy > 0) 
	{
		//going down
		sourceRect.left = spriteWidth * 2;
		sourceRect.right = sourceRect.left + spriteWidth;
	}
	else 
	{
		//no change
		sourceRect.left = 0;
		sourceRect.right = spriteWidth;
	}

	// check bounds for top of screen
	if(posY < 0) 
	{
		posY = 0;
	}
	
	// check bounds for bottom
	else if(posY > WINDOW_HEIGHT - spriteHeight) 
	{
		posY = WINDOW_HEIGHT - spriteHeight;
	}
}

/*
 * Fires the weapons
 * 
 * @returns - none
 */
void Player::fire()
{
	POINT startPos;
	startPos.x = getPosX()+32;
	startPos.y = getPosY()+32;
	
	weaponMgr.fireWeapon(startPos);
}

/*
 * update
 *
 * updates the state of the player since the last frame
 *
 * args  dt  how much time passed since the last frame (in seconds). useful for things such as animation
 */
void Player::update(float dt, std::vector<Enemy*> *enemyVector, std::vector<PowerUp*> *powerUpVector)
{
	if(hitTimer > 0.0f) {
		hitTimer -= dt;
	}

	//update weapons
	weaponMgr.updateWeapons(dt);

	if(respawning) 
	{
		if((secsToRespawn -= dt) < 0.0f) 
		{
			dead = false;
			respawning = false;
			texture.red = texture.green = texture.blue = 255;
			secsToRespawn = RESPAWN_TIME;
		}
		else 
		{
			texture.red = texture.green = texture.blue = (texture.blue + 255) % 510;
		}
	}

	//check for collisions on each enemy
	for(int i = 0; i < (int)enemyVector->size(); ++i) 
	{
		Enemy *enemyShip = (*enemyVector)[i];
	
		//check for enemy collisions
		if(enemyShip) 
		{
			checkForCollisionWith(enemyShip);
		}
	}

	// check for collision with power-ups
	for(int i = 0; i < (int)powerUpVector->size(); i++)
	{
		PowerUp *powerUp = (*powerUpVector)[i];

		if(powerUp)
		{
			checkForCollisionWith(powerUp);
		}
	}
	
	// Check for collision with enemy bullets
	if( !dead ) {
		checkForCollisionWithBullets();
	}

	//update flame's source rect
	flameSrcRect.left = (flameSrcRect.left + 32) % 64;
	flameSrcRect.right = (flameSrcRect.right % (64)) + 32;
	
	
	/******* Binning Stuff... no longer being used ******
	
	// recalculate my bins
	reBin();

	// set all my bins to occupied
	std::vector<Bin*>::iterator iter = myBins.begin();
	while(iter != myBins.end())
	{	
		(*iter)->occupied = true;
		++iter;
	}
	*/
}

/*
 * selectWeapon
 *
 * selects the current weapon to use
 *
 * params -> weaponNum - index into the weapon array
 *
 */
void Player::selectWeapon(int weaponNum)
{
	selectedWeapon = weaponNum;
	weaponMgr.selectWeapon(weaponNum);
}

/*
 * selectSpecialWeapon
 *
 * selects the special weapon to use
 *
 * params -> weaponNum - index into the weapon array
 *
 */
void Player::selectSpecialWeapon(int weaponNum)
{
	selectedSpecialWeapon = weaponNum;
   weaponMgr.selectSpecialWeapon(weaponNum);
}

void Player::setStartingValues( int startingLives, int startingGBombs, float difficulty ) {
	startLives = startingLives;
	startGBombs = startingGBombs;
	diffMultiplier = difficulty;
}

/*
 * Checks for collision with an enemy ship
 * 
 * @param - The Enemy ship to check collision with.
 * @returns - Whether a collision occured or not
 */
bool Player::checkForCollisionWith(Enemy *enemyShip)	
{
	// check for collision with player's bullets
	weaponMgr.checkForCollision(enemyShip);

	//don't check for coll with other enemies if we're still respawning
	if(dead) 
	{
		return false;
	}

	RECT overlapRect, myBoundingRect, hisBoundingRect;
	
	getBounds(&myBoundingRect);
	enemyShip->getBounds(&hisBoundingRect);

	/***********************************************************************/
	// if the rects overlap, possibly check for polygon armature collision
	/***********************************************************************/
	if(IntersectRect(&overlapRect, &myBoundingRect, &hisBoundingRect))
	{
		if(checkPolyArmature(&boundingPoints, getPosX(), getPosY(),
			enemyShip->getBoundingPoints(), enemyShip->getPosX(), enemyShip->getPosY())) {
			damage(MAX_PLAYER_LIFE);
			
			//only hurt the enemy ship if it's not a boss element
			if(!enemyShip->isBossElement()) {
				enemyShip->damage(20.0);
			}
			return true;
		}
	}
	else
	{
		return false;
	}
}

/*
 * Checks for collision with a power up.
 * 
 * @param - The Powerup to check collision with.
 * @returns - Whether a collision occured or not
 */
bool Player::checkForCollisionWith(PowerUp *powerUp)	
{
	//check for collision with other ships (only if we are in the same quadrant)
	if(checkQuad(powerUp->getMyQuad()))
	{
		RECT overlapRect, myBoundingRect, hisBoundingRect;
	
		getBounds(&myBoundingRect);
		powerUp->getBounds(&hisBoundingRect);

		/***********************************************************************/
		// if the rects overlap, possibly check for polygon armature collision
		/***********************************************************************/
		if(IntersectRect(&overlapRect, &myBoundingRect, &hisBoundingRect))
		{
			powerUp->collectMe();
			int powerupId = powerUp->getID();

			//see what kind of power up it is
			switch(powerupId) {
				//weapon powerups
				case 0:
               SoundManager::Instance()->playSoundEffect("audio/sfx/s_wavegun_voice.wav");
               selectWeapon(powerUp->getID() + 1);
               break;
				case 1:
               SoundManager::Instance()->playSoundEffect("audio/sfx/s_spreadshot_voice.wav");
               selectWeapon(powerUp->getID() + 1);
               break;
            case 2:
               SoundManager::Instance()->playSoundEffect("audio/sfx/s_laser_voice.wav");
               selectWeapon(powerUp->getID() + 1);
               break;
				case 3:
               SoundManager::Instance()->playSoundEffect("audio/sfx/s_missiles_voice.wav");
               selectSpecialWeapon(powerUp->getID() + 1);
               break;
				case 4:
					++gravityBombs;
               SoundManager::Instance()->playSoundEffect("audio/sfx/s_gravity_bomb_voice.wav");
					break;
				case 5:
					++lives;
               SoundManager::Instance()->playSoundEffect("audio/sfx/s_one_up_voice.wav");
               break;
			}

			return true;
		}
		else
		{
			return false;
		}

	}
	else
	{
		return false;
	}

}


/*
 * Checks for collisions with the enemy bullets
 *
 * @returns - Whether the ship was hit by a bullet
 */
bool Player::checkForCollisionWithBullets() 
{
   Weapon **weaponArray = EnemyManager::Instance()->getWeapons();

   for(int i = 0; i < NUM_ENEMY_WEAPONS; i++)
	{
		std::vector<BulletData> *ammoPos = weaponArray[i]->getAmmoPos();
		std::vector<BulletData>::iterator iter = ammoPos->begin();

		RECT playerBoundingRect;
		getBounds(&playerBoundingRect);

		while(iter != ammoPos->end()) {
		   RECT ammoRect, overlapRect;
			//first set the rect for the ammo

			// Get the size of the texture used to represent the 
			// ammo for the weapon.  Then get the offset used
			// for checking intersection.
			RECT ammoTempRect = (*iter).srcRect;
			int leftOffset = ammoTempRect.left;
			int topOffset = ammoTempRect.top;
			int bottomOffset = ammoTempRect.bottom;
			int rightOffset = ammoTempRect.right;

			SetRect(&ammoRect,
					(*iter).position.x + leftOffset,
					(*iter).position.y + topOffset,
					(*iter).position.x + rightOffset,
					(*iter).position.y + bottomOffset);
			
			if(IntersectRect(&overlapRect, &playerBoundingRect, &ammoRect)) {
				// Remove the bullet that hit the player from the screen
				// only if it is not a "laser" bullet.
				//
				if ( (*iter).type == NORMAL )
				{
					ammoPos->erase(iter);
				}

				// Cause damage.
				damage(weaponArray[i]->getWeaponStrength());
				return true;
			} 
         else 
         {
				++iter;
			}
		}
	}
	return false;
}

/*
 * Checks for to see if an enemy is in the same quadrant as the player
 * 
 * @param - The quadrant the enemy ship is in.
 * @returns - Whether the enemy is within the same quadrant as the player
 */
bool Player::checkQuad(int enemyQuad)
{
	if(getMyQuad() == enemyQuad)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * getWeaponManager
 *
 * returns the weapon manager for this player
 *
 * returns -> weapon manager
 *
 */
WeaponManager* Player::getWeaponManager()
{
	return &weaponMgr;
}

/*
 * loadTextures
 *
 * loads the player ship and flame textures into memory
 *
 * returns status code of loading (did it succeed or not)
 */
HRESULT Player::loadTextures() 
{
	HRESULT r;

	//load the ship itself
	r = SpriteManager::Instance()->loadTexture(&texture);
	if(FAILED(r)) {
		GameError("failure loading player texture");
		return r;
	}

	//load the ship's flames
	r = SpriteManager::Instance()->loadTexture(&flameTexture);
	if(FAILED(r)) {
		GameError("failure loading player's flame texture");
		return r;
	}

	//load all weapons
	r = getWeaponManager()->loadTextures();
	if(FAILED(r)) {
		GameError("failure loading player's weapon textures");
		return r;
	}

	return r;
}

/*
 * draw
 *
 * draws the player, its flames, possibly its weapons
 */
HRESULT Player::draw(int alpha) 
{
	HRESULT r = S_OK;

	//if the player isn't respawning and is dead...that means they have no more lives left
	//hence, game over
	if(!respawning && dead) {
		return r;
	}

	//draw the flame 
    if ( showFlame ){
	   r = SpriteManager::Instance()->drawSprite(&flameTexture,
	       						  			     &flameSrcRect,
		  	   							         (float)(posX - 24),
				   						         (float)(posY + 24),
												 alpha);
	
	   if(FAILED(r)) {
   		GameError("Error drawing player ship's flames");
		   return r;
	   }
    }

   
	//draw the weapons
	r = getWeaponManager()->draw();
	if(FAILED(r)) {
		GameError("Error drawing player ship's weapons");
		return r;
	}

	if(visible) {
	//draw the player ship
	r = SpriteManager::Instance()->drawSprite(&texture,
	    						  			  &sourceRect,
		  								      (float)posX,
										      (float)posY,
											  alpha);
	if(FAILED(r)) {
		GameError("Error drawing player ship");
		return r;
	}}

	if(!respawning && hitTimer < 0.0f) {
		texture.green = texture.blue = texture.red = 255;
	}
	return r;
}

/*
 * getScore
 *
 * returns the player's current score
 */
int Player::getScore() {
	return score;
}

/*
 * getSpeed
 *
 * returns the speed of the player's ship
 */
int Player::getSpeed() {
	return speed;
}

/*
 * damage
 *
 * indicates the specified amount of damage has been inflicted upon 'this'
 * 
 * args - strength - the strength of the damage inflicted upon this
 */
void Player::damage(float strength) {
	texture.red = 150;
	texture.green = texture.blue = 0;
	hitTimer = FLASH_TIME;
	SoundManager::Instance()->playSoundEffect("audio/sfx/damage.wav");

	if((life -= ( strength * diffMultiplier ) ) <= 0) {
		//first make the ship explosion
		//then set it up for respawn
		POINT screenCoords;
		screenCoords.x = posX + 32;
		screenCoords.y = posY + 32;
		FXManager::Instance()->createExplosion(screenCoords);

		setPosX(100);
		setPosY(100);
		dead = true;
		hitTimer = 0.0f;
		texture.red = texture.green = texture.blue = 255;

		//reset the source rect to reflect no player ship damage
		sourceRect.left = sourceRect.top = 0;
		sourceRect.right = sourceRect.bottom = spriteHeight;

		//if the player has no more lives, left, don't respawn
		if((--lives) == 0) {
			life = 0;
			respawning = false;
 		}else {
			//respawn with the default weapon
			weaponMgr.selectWeapon(0);
		    weaponMgr.selectSpecialWeapon(0);
			life = MAX_PLAYER_LIFE;
			respawning = true;
			texture.red = texture.green = texture.blue = 0;
		}
	}else if(life >= (2 * MAX_PLAYER_LIFE / 3)) {
		//dont change the source rect if the player's "healthy"
	}else if(life >= (MAX_PLAYER_LIFE / 3)) {
		//change to 'medium' health
		sourceRect.top = spriteHeight;
		sourceRect.bottom = texture.height - spriteHeight;
	}else {
		//ready to die
		sourceRect.top = texture.height - spriteHeight;
		sourceRect.bottom = texture.height;
	}

}

/*
 * getLives
 *
 * number of lives the player has left
 *
 * returns the number of lives the player has left
 */
int Player::getLives() {
	return lives;
}

/*
 * getGravityBombs
 *
 * number of gravity bombs the player has left
 *
 * returns the number of gravity bombs the player has left
 */
int Player::getGravityBombs() {
	return gravityBombs;
}

/*
 * fireGravityBomb
 *
 * fires the gravity bomb
 *
 * returns true if the player is able to fire a gravity bomb (do they have any left?), false o.w.
 */
bool Player::fireGravityBomb() {
	bool retVal = false;

	if(gravityBombs > 0) {
		retVal = true;
		--gravityBombs;
		FXManager::Instance()->flashScreen();
	}

	return retVal;
}

/*
 * addPoints
 *
 * adds the specified number of points to the player's score
 *
 * arg - numPoints - number of points to add to the player's score
 */
void Player::addPoints(int numPoints) {
	score += numPoints * diffMultiplier;
}

void Player::setSpeed(int newSpeed) {
	speed = newSpeed;
}

void Player::getBounds(RECT* rect)
{
	// Win32 function
	SetRect(rect, getPosX(), getPosY() + 5, getPosX()+ spriteWidth - 2, getPosY()+ spriteHeight - 9);
}