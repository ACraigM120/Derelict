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

#include "Enemy.h"

// constructor
Enemy::Enemy(Texture newTexture, Pattern *enemyPattern, int spriteWidth, int spriteHeight, int pointVal,
			 int enemyType) : Ship(spriteWidth, spriteHeight)
{
	// initialize position
	posX = 700;
	posY = 200;
	life = 4;	
	points = pointVal;
	texture = newTexture;
	pattern = enemyPattern;
	dx = 0;
	gravityBombed = false;
	gravityVal = 0;
	gravityTimer = 0.0f;
	weaponFireTimer = 0.0f;
	weaponType = -1;
	weaponFireRate = 0.0f;
	gravityBombable = true;
	type = enemyType;
	animationTime = -1.0f;
	animationCountdown = -1.0f;
	bossElement = false;
}

// destructor
Enemy::~Enemy() 
{

}

void Enemy::setPosX(int x) 
{
	posX = x;
}

void Enemy::setPosY(int y) 
{
	posY = y;
}

/*
 * update
 *
 * updates the state of the player since the last frame
 *
 * args  dt  how much time passed since the last frame (in seconds). useful for things such as animation
 */
void Enemy::update(float dt, int playerY, bool gravityBomb)
{
	if(hitTimer > 0.0f) {
		hitTimer -= dt;
	}

	if(!gravityBombed && gravityBomb && isGravityBombable()) {
		gravityBombed = true;
		gravityVal = 1;
	}

	if(pattern->matchWorldSpeed) {
		setRelX(0 - dx);
	}else {
		setRelX((int)pattern->deltaX);
	}

	//setting of relative Y coordinates lead to 3 diff cases:
	//
	// 1. if this enemy was hit with a gravity bomb, pseudo-accelerate to the ground
	// 2. if this enemy is tracking the player, try to sync y pos with player's y pos
	// 3. o.w. set y pos according to the current node in this enemy's pattern
	if(gravityBombed) {
		setRelY(gravityVal);
		gravityTimer += dt;
		if(gravityTimer >= 0.1f && gravityVal < 32) {
			gravityVal = gravityVal << 2;
			gravityTimer = 0.0f;
			if(gravityVal == 64) {
				gravityVal = 60;
			}
		}
	}else if(pattern->followPlayer) {
		if(getPosY() + pattern->deltaY * 2< playerY) {
			setRelY((int)(pattern->deltaY/2));
		}else if(getPosY() - pattern->deltaY * 2 > playerY) {
			setRelY((int)(0 - pattern->deltaY/2));
		}
	}else {
		setRelY((int)pattern->deltaY);
	}
	pattern = pattern->next;

	//animate this enemy! (if it's animate-able)
	if(animationTime != -1.0f && !gravityBomb) {
		if((animationCountdown -= dt) < 0.0f) {
			sourceRect.left = (sourceRect.left + spriteWidth) % texture.width;
			sourceRect.right = sourceRect.left + spriteWidth;
			animationCountdown = animationTime;
		}
			
	}
}

/*
 * getPoints
 *
 * get how many points a player receives for killing 'this'
 *
 * returns how many points a player receives for killing 'this'
 */
int Enemy::getPoints() {
	return points;
}

/*
 * isOffscreen
 *
 * tells whether or not this enemy is offscreen
 *
 * returns true if this enemy is offscreen, false otherwise
 */
bool Enemy::isOffscreen() {
	return (getPosX() > WINDOW_WIDTH || getPosX() < 0 - spriteWidth ||
		    getPosY() > WINDOW_HEIGHT || getPosY() < 0 - spriteHeight);
}

/*
 * draw
 *
 * draws the enemy ship on screen
 */
HRESULT Enemy::draw(int alpha) 
{
   HRESULT r = SpriteManager::Instance()->drawSprite(&texture,
												 &sourceRect,
												 (float)posX,
												 (float)posY,
												 alpha);

	if(hitTimer < 0.0f) {
		texture.green = texture.blue = texture.red = 255;
	}

	return r;

}

/*
 * damage
 *
 * indicates the specified amount of damage has been inflicted upon 'this'
 * 
 * args - strength - the strength of the damage inflicted upon this
 */
void Enemy::damage(float strength) {
	if((life -= strength - ( diffMultiplier / 40 ) ) <= 0) {
		dead = true;
	}
	texture.red = 150;
	texture.green = texture.blue = 0;
	
	hitTimer = FLASH_TIME;
}

/*
 * setPolygonArmature
 *
 * sets the polygon Armature for this enemy
 */
void Enemy::setPolygonArmature(std::vector<POINT> points) {
	boundingPoints = points;
}

// Get the current timer for weapon firing
float Enemy::getTimer()
{
   return weaponFireTimer;
}

// Update the timer
void Enemy::updateTimer( float newTime )
{
   weaponFireTimer  += newTime;
}

// Reset the timer after the weapon is fired
void Enemy::resetTimer()
{
   weaponFireTimer = 0.0f;
}

// Get the weapon type
int Enemy::getWeaponType()
{
   return weaponType;
}

// Set the weapon type
void Enemy::setWeaponType( int weapon )
{
   weaponType = weapon;
}

// Get the weapon firing rate
float Enemy::getWeaponFireRate()
{
   return weaponFireRate;
}

// Set the weapon firing rate
void Enemy::setWeaponFireRate( float rate )
{
   weaponFireRate = rate;
}

void Enemy::setDx(int newDx) {
	dx = newDx;
}

Pattern *Enemy::getPattern() {
	return pattern;
}

/*
 * Sets the difficulty multiplier effecting enemy strength.
 *
 * @args difficulty - the difficulty multiplier
 */
void Enemy::setDifficulty( float difficulty ) {
	diffMultiplier = difficulty;
}

/*
 * setGravityBombable
 *
 * set whether or not this enemy can be affected by gravity bombs
 *
 * arg - bombable - whether or not enemy can be attacked by gbombs
 */
void Enemy::setGravityBombable(int bombable) {
   gravityBombable = bombable;
}

/*
 * isGravityBombable
 *
 * can this enemy  be attacked by g bombs?
 *
 * returns true if enemy can be attacked by g bombs, false otherwise
 */
bool Enemy::isGravityBombable() {
   return gravityBombable;
}

/*
 * getEnemyType
 *
 * returns the id number of this enemy
 */
int Enemy::getEnemyType() {
   return type;
}

void Enemy::setAnimationTime(float time) {
	animationTime = time;
	animationCountdown = time;
}

void Enemy::setBossElement(bool isElement) {
	bossElement = isElement;
}

bool Enemy::isBossElement() {
	return bossElement;
}

bool Enemy::isGravityBombed() {
	return gravityBombed;
}