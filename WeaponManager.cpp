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

#include "WeaponManager.h"
#include "Collision.h"
#include <stdio.h>

/*
 * Constructor
 * Creates a new instance of the WeaponManager
 *
 * @returns - None
 */
WeaponManager::WeaponManager()
{
   // Load the weapon array with all of the available weapons
	weaponArray[0] = new DefaultGun("graphics/defaultBullet.dds");
	weaponArray[1] = new WaveGun("graphics/waveBullet.dds");
	weaponArray[2] = new SpreadGun("graphics/spreadBullet.dds");
    weaponArray[3] = new Laser("graphics/laser.dds");
	weaponArray[4] = new Missile("graphics/Missile.dds");

   selectedSpecialWeapon = 0; // Initialized to 0
}

/*
 * Destructor
 * Performs cleanup
 *
 * @returns - None
 */
WeaponManager::~WeaponManager()
{
   // Clear out the weapon Array
	for(int i = 0; i < NUM_WEAPONS; ++i) {
		delete weaponArray[i];
		weaponArray[i] = 0;
	}
}

/*
 * reset
 *
 * recreates all of the weaposn
 */
void WeaponManager::reset() {
	for(int i = 0; i < NUM_WEAPONS; ++i) {
		weaponArray[i]->reset();
	}
}

/*
 * fireWeapon
 *
 * takes care of firing the currently selected weapon
 *
 * params -> weaponNum - index in weapon array (selected weapon)
 *			 startPos - position to start firing from
 *
 */
void WeaponManager::fireWeapon(POINT startPos)
{
	weaponArray[selectedWeapon]->fire(startPos);

   // Only fire the special weapon if the user currently has
   // one in their possession.
   //
   if ( selectedSpecialWeapon != 0 )
   {
      weaponArray[selectedSpecialWeapon]->fire(startPos);
   }
}


/*
 * updateWeapons
 *
 * calls the update() function for all currently active weapons
 *
 * args  dt  how much time passed since the last frame (in seconds). useful for animation of bullets
 *
 */
void WeaponManager::updateWeapons(float dt)
{
	for(int i = 0; i < NUM_WEAPONS; i++)
	{
		weaponArray[i]->update(dt);
	}
}

/*
 * selectWeapon
 *
 * selects a weapon to use
 *
 * params -> index - index into the weapon array
 *
 */
void WeaponManager::selectWeapon(int index)
{
	selectedWeapon = index;
}

/*
 * selectSpecialWeapon
 *
 * selects a special weapon to use
 *
 * params -> index - index into the weapon array
 *
 */
void WeaponManager::selectSpecialWeapon(int index)
{
	selectedSpecialWeapon = index;
}

/*
 * getWeaponPos
 *
 * returns the screen position of a weapon
 *
 * params -> index - index into the weapon array
 *
 * returns -> an array of POINTS representing positions of all instances
 *				of this weapon(IE, bullets).  The array will be of
 *				length 1 if the weapon is unique.
 *
 */
POINT WeaponManager::getWeaponPos(int index)
{
	return weaponArray[index]->getPos(0);
}

/*
 * Returns the array of weapons.
 *
 *
 * @returns - The array of weapons
 */
Weapon** WeaponManager::getWeapons()
{
	return weaponArray;
}


/*
 * Loads all of the textures for the weapons
 *
 * @returns - Result code
 */
HRESULT WeaponManager::loadTextures() {
	HRESULT r;
	for(int i = 0; i < NUM_WEAPONS; ++i) {
		r = weaponArray[i]->loadTexture();
		
		if(FAILED(r)) {
			return r;
		}
	}

	return r;
}


/*
 * Draw all of the weapons
 *
 * @returns - Result code
 */
HRESULT WeaponManager::draw() 
{
	HRESULT r;
	
	for(int i = 0; i < NUM_WEAPONS; ++i) 
	{
		r = weaponArray[i]->draw();
	
		if(FAILED(r)) {
			return r;
		}
	}

	return r;
}

/*
 * Checks for collision of all bullets with enemy ships
 *
 * @param - enemyShip - An Enemy object to check for collision with
 *
 * @returns - True if hit ship/false otherwise
 */
bool WeaponManager::checkForCollision(Enemy *enemyShip) {
	for(int i = 0; i < NUM_WEAPONS; i++)
	{
		std::vector<BulletData> *ammoPos = weaponArray[i]->getAmmoPos();
		std::vector<BulletData>::iterator iter = ammoPos->begin();

		RECT enemyBoundingRect;
		enemyShip->getBounds(&enemyBoundingRect);

		while(iter != ammoPos->end()) {
			RECT ammoRect, overlapRect;
			//first set the rect for the ammo

			// Get the size of the texture used to represent the 
			// ammo for the weapon.  Then get the offset used
			// for checking intersection.
			RECT ammoTempRect = (*iter).srcRect;
			int bottomOffset = ammoTempRect.bottom;
			int rightOffset = ammoTempRect.right;

			SetRect(&ammoRect, (*iter).position.x, (*iter).position.y, (*iter).position.x + bottomOffset, (*iter).position.y + rightOffset);
			
			if(IntersectRect(&overlapRect, &enemyBoundingRect, &ammoRect)) {
				//if rects intersect, check polyarm
				if(checkPolyArmature(weaponArray[i]->getBoundingPoints(), (*iter).position.x, (*iter).position.y,
					enemyShip->getBoundingPoints(), enemyShip->getPosX(), enemyShip->getPosY())) {
					// Remove the bullet that hit the enemy from the screen
					// only if it is not a "laser" bullet.
					//
					if ( (*iter).type == NORMAL )
					{
						ammoPos->erase(iter);
					}
					// Cause damage.
					enemyShip->damage(weaponArray[i]->getWeaponStrength());
					return true;
				}
			} 

			++iter;
		}
	}

	return false;
}

/*
 * Return the currently selected weapon
 *
 * @returns - the current Weapon
 */
Weapon *WeaponManager::getCurrentWeapon() {
	return weaponArray[selectedWeapon];
}