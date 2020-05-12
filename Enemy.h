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

// Enemy.h -- header file for an enemy class
#ifndef ENEMY_H
#define ENEMY_H

#include "GameCore.h"
#include "Texture.h"
#include "Ship.h"
#include "Pattern.h"

// player class is derived from Ship
class Enemy : public Ship
{
public:
	//ctor/dtor
	Enemy(Texture newTexture, Pattern *enemyPattern, int spriteWidth, int spriteHeight, int pointVal, int enemyType);
	~Enemy();

	void setPosX(int x);
	void setPosY(int y);

	// additional player-specific functions
	void update(float dt, int playerY, bool gravityBomb);
	bool isOffscreen();
	int getPoints();
	void damage(float strength);
	void setPolygonArmature(std::vector<POINT> points);
	void setDx(int newDx);  //change delta x for enemies matching the world's scroll speed
	void setGravityBombable(int bombable);
	bool isGravityBombable();
	bool isGravityBombed();
	void setBossElement(bool isElement);
	bool isBossElement();
	int getEnemyType();
	Pattern *getPattern();
	void setAnimationTime(float time);

   // Weapon firing specific methods
   float getTimer();
   void updateTimer( float newTime );
   void resetTimer();

   // Weapon method
   int getWeaponType();
   void setWeaponType( int weapon );
   float getWeaponFireRate();
   void setWeaponFireRate( float fireRate );

	HRESULT draw(int alpha = -1);
	void setDifficulty( float difficulty );

private:
	int points;  //how many points player will receive for destroying 'this'
	Pattern *pattern;
	bool gravityBombed;
	int gravityVal;
   bool gravityBombable;
	float gravityTimer;
	int dx;
	float diffMultiplier;
   int type;
   // Weapon specifics
   float weaponFireTimer;  // Timer for weapon firing.
   int weaponType;         // The type of weapon this enemy has.
   float weaponFireRate;   // The rate of fire for the weapon.
   float animationTime;
   float animationCountdown;
   bool bossElement;
};
#endif