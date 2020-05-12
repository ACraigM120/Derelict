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

#ifndef WEAPON_MANAGER_H
#define WEAPON_MANAGER_H

#include "GameCore.h"
#include "Enemy.h"
#include "Texture.h"
#include "Weapon.h"
#include "BulletData.h"
#include "DefaultGun.h"
#include "WaveGun.h"
#include "SpreadGun.h"
#include "Missile.h"
#include "Laser.h"

#define NUM_WEAPONS 5   // The current number of weapons available in the game

class WeaponManager
{
public:

	// constructor & destructor
	WeaponManager();
	~WeaponManager();

	// member functions
	void fireWeapon(POINT startPos);                // Fires the weapons
	void updateWeapons(float dt);	                  // called each frame to update all weapons being managed
	void selectWeapon(int index);							// selects a weapon for use
   void selectSpecialWeapon(int index);            // select the special weapon
	POINT getWeaponPos(int index);						// get position of weapon (index)
	Weapon** getWeapons();									// return the array of weapon objects
	Weapon* getCurrentWeapon();                     // returns the currently selected weapon
	bool checkForCollision(Enemy* enemyShip);       // checks for bullet/enemy collision
	void reset();                                   // resets the weapons.

	HRESULT loadTextures();                         // Loads the textures for the weapons
	HRESULT draw();                                 // Draws the weapons
	
private:
	Weapon *weaponArray[NUM_WEAPONS];	// array of weapons this manager manages
	int selectedWeapon;                 // The current basic weapon
	int selectedSpecialWeapon;          // The current special weapon
};

#endif