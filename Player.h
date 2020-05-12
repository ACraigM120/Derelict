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

// Player.h -- header file for the Player class
#ifndef PLAYER_H
#define PLAYER_H

#include "GameCore.h"
#include "Texture.h"
#include "Ship.h"
#include "WeaponManager.h"
#include "Enemy.h"
#include "PowerUp.h"

#define RESPAWN_TIME 3.0f
#define MAX_PLAYER_LIFE 5.0f

// player class is derived from Ship
class Player : public Ship
{
public:
	// Constructor/Destructor
	Player(char* texFile, int spriteWidth, int spriteHeight );
	~Player();

	void setPosX(int x);             // Set the X position of the player
	void setPosY(int y);             // Set the Y position of the player
	int getSpeed();                  // Get the current speed of the player
	int getScore();                  // Get the current score
	void setSpeed(int newSpeed);
	void addPoints(int numPoints);   // Add points to the score.
	void getBounds(RECT* rect);		 // returns the bounding rect of a ship

	// additional player-specific functions
   // Update the player
	void update(float dt, std::vector<Enemy*> *enemyVector, std::vector<PowerUp*> *powerUpVector);
	void damage(float strength);     // Cause damage to the player
	int getLives();                  // Get the number of lives remaining
	int getGravityBombs();           // Get the number of gravity bombs remaining
	void reset();                    //puts the player in its initial state
	void setStartingValues( int startingLives, int startingGBombs, float difficulty );

	//collision detection
	bool checkForCollisionWith(RECT* boundingBox);	// checks bounding box collision
	bool checkForCollisionWith(Enemy *enemyShip);	// enemy ship collision
	bool checkForCollisionWith(PowerUp *powerUp);	// power-up collision check
    bool checkForCollisionWithBullets();           // Ship enemy bullet collisions
	bool checkQuad(int enemyQuad);					   // true if my quadrant matches the enemy's

	//weapon functions
	void fire();                              // Fire the ships weapon
	bool fireGravityBomb();                   // Fire a gravity bomb
	int getSelectedWeapon();	               // returns currently selected weapon
	WeaponManager* getWeaponManager();        // Get the weapon manager.
	void selectWeapon(int weaponNum);         // Get the selected weapon
   void selectSpecialWeapon(int weaponNum);  // Get the selected special weapon.

	//sprite functions
	HRESULT loadTextures();          // Load the textures for the player ship
	HRESULT draw(int alpha = -1);    // Draw the ship
    
private:
	Texture flameTexture;            // The Flame texture
	RECT flameSrcRect;               // The source RECT for the flame
	WeaponManager weaponMgr;         // The weapon manager
	int score;                       // The current score
	int speed;                       // The ships speed
	bool showFlame;                  // Whether the after-burner flame is visible
	int lives;                       // The current number of lives
	int startLives;					 // The number of lives you begin with
	bool respawning;                 // Whether the ship is respawning after being killed.
	float secsToRespawn;             // Respawn timer
	int gravityBombs;                // The number of gravity bombs
	int startGBombs;				 // The number of gravity bombs you begin with
	int selectedWeapon;              // The currently selected base weapon
    int selectedSpecialWeapon;       // The currently selected special weapon
	float diffMultiplier;			 // A multiplier that adjusts the game's difficulty
};
#endif