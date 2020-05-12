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

#ifndef POWERUP_MANAGER_H
#define POWERUP_MANAGER_H

#include "GameCore.h"
#include "PowerUp.h"
#include "PowerUpTrigger.h"
#include "Texture.h"
#include "SpriteManager.h"
#include "FXManager.h"

#define NUM_TYPES 6

class PowerUpManager
{
public:

	// singleton class
	static PowerUpManager* Instance();

	// a&m



	// funcs

	void update(float dt, int worldX, int playerY);
	HRESULT draw(int alpha = -1);
	HRESULT loadTextures();
    void initPowerUpManager(char *filename);
	std::vector<PowerUp*> *getActivePowerUps();
	void reset();
	void setDx(int newDx);

protected:

	// ctor/dtor
	PowerUpManager();
	~PowerUpManager();

private:

	void loadTriggers(char* fileName);
	void spawnPowerUp(PowerUpTrigger trigger, int worldX);

	std::vector<PowerUp*> activePowerUps;			// power-ups that are active and onscreen
	std::vector<PowerUpTrigger> triggers;			// where and what type to spawn for each power-up

	Texture textures[NUM_TYPES];					// texture for each power-up type
	std::vector<POINT> polyArmatures[NUM_TYPES];	// polygon armatures for each type

	char* triggerFileName;
    int dx;
};

#endif