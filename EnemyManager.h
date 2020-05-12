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

#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "GameCore.h"
#include "Enemy.h"
#include "Pattern.h"
#include "EnemyTrigger.h"
#include "Texture.h"
#include "SpriteManager.h"
#include "FXManager.h"
#include "EnemyGun.h"
#include "ClamFighterGun.h"
#include "GroundHogGun.h"
#include "BossLaser.h"
#include "BossSpreadGun.h"

#define NUM_ENEMIES 10
#define NUM_ENEMY_WEAPONS 4

#define LASER_ID 5
#define BOSS_SPREAD_A_ID 6
#define BOSS_SPREAD_B_ID 7
#define CORE_ID 8
#define SPAWN_ID 9

#define SPAWNER_TIMER 1.5f

class EnemyManager {
public:
   // Returns a singleton instance of the SoundManager class
   static EnemyManager* Instance();

	int update(float dt, int worldX, int playerY, bool gravityBomb);
	HRESULT drawEnemies(int alpha = -1);
	HRESULT drawWeapons(int alpha = -1);
	HRESULT loadTextures();
    void initEnemyManager(char *filename);
	void loadPatterns(char *filename);
	std::vector<Enemy*> *getActiveEnemies();
	void reset();
	void setDx(int newDx);  //changes delta x val for enemies matching world's scrolling screen
	void setDifficulty( float difficulty );
    Weapon** getWeapons();				   // return the array of weapon objects
   void killAllEnemies();
   bool isCoreDying();

protected:
	//ctor/dtor
   EnemyManager();
	~EnemyManager();

private:
	void loadTriggers(char *filename);
	void spawnEnemy(EnemyTrigger trigger, int worldX);

	std::vector<Enemy*> activeEnemies;   //all the enemies that are stil alive & onscreen
	std::vector<EnemyTrigger> triggers;  //what kind of enemy should spawn & where it should spawn

	Texture enemyTextures[NUM_ENEMIES];
	Pattern *enemyPatterns[NUM_ENEMIES];
	std::vector<POINT> polyArmatures[NUM_ENEMIES];

	char *triggerFilename;
	int dx;
	float diffMultiplier;
	bool bossOnScreen;
	bool coreDying;
	float spawnTimer;
	bool upgradeBoss;
	Weapon *weaponArray[NUM_ENEMY_WEAPONS];   // An array of all the available weapons for the enemies
};
#endif