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

#include "EnemyManager.h"
#include "SoundManager.h"
#include <fstream>

/*
 * Returns the instance of the EnemyManager object. 
 *
 */
EnemyManager* EnemyManager::Instance()
{
   //instead of 'new'ing 'this' and returning the pointer, just return the address. this way we dont
   //have to worry about explicitly calling the destructor of 'this'
   static EnemyManager enemyMgr;
   return &enemyMgr; 
}

/*
 * constructor
 *
 * args - filename - name of file containing enemy patterns and enemy spawn positions
 */
EnemyManager::EnemyManager() {
   bossOnScreen = false;
   coreDying = false;
   upgradeBoss = false;
   spawnTimer = 0.0f;

   weaponArray[0] = new EnemyGun("graphics/enemyDefaultGun.dds");
   weaponArray[1] = new GroundHogGun("graphics/enemyDefaultGun.dds");
   weaponArray[2] = new BossLaser("graphics/BossLaser.dds");
   weaponArray[3] = new BossSpreadGun("graphics/bossSpreadBullet.dds");
}

/*
 * destructor
 */
EnemyManager::~EnemyManager() {
	//delete any enemies still on the screen
	for(int i = 0; i < (int)activeEnemies.size(); ++i) {
		delete activeEnemies[i];
		activeEnemies[i] = 0;
	}

	//release all enemy textures
	for(int i = 0; i < NUM_ENEMIES; ++i) {
		if(enemyTextures[i].texture) {
			enemyTextures[i].texture->Release();
			enemyTextures[i].texture = 0;
		}
	}

	//delete all enemy patterns
	for(int i = 0; i < NUM_ENEMIES; ++i) {
		Pattern *current = enemyPatterns[i];
		do {
			Pattern *next = current->next;
			delete current;
			current = 0;
			current = next;
		}while(current != enemyPatterns[i]);
	}

   // Delete all of the weapons
	for(int i = 0; i < NUM_ENEMY_WEAPONS; ++i) {
		delete weaponArray[i];
		weaponArray[i] = 0;
	}

}

/*
 * reset
 *
 * puts the enemymgr in the initial state. basically, used every time a new game is started
 */
void EnemyManager::reset() {
	//reset all flags and data members
	bossOnScreen = false;
	coreDying = false;
	upgradeBoss = false;
	spawnTimer = 0.0f;
	weaponArray[3]->unUpgrade();

	//delete any enemies still on the screen
	std::vector<Enemy*>::iterator iter = activeEnemies.begin();
	while(iter != activeEnemies.end()) {
		delete (*iter);
		activeEnemies.erase(iter);
	}

	//remove all enemy triggers
	std::vector<EnemyTrigger>::iterator trigIter = triggers.begin();
	while(trigIter != triggers.end()) {
		triggers.erase(trigIter);
	}

	//reload all enemy triggers
	loadTriggers(triggerFilename);

   // Reset the weapons
	for(int i = 0; i < NUM_ENEMY_WEAPONS; ++i) {
	   weaponArray[i]->reset();
	}

}

/*
 * Initializes the EnemyManager instance
 *
 * @param - filename - The file that contains the list of triggers for 
 *                     When enemies will appear on the screen.
 * @returns - None
 */
void EnemyManager::initEnemyManager(char *filename) {
	triggerFilename = filename;
	loadTriggers(triggerFilename);
	loadPatterns("map/level1/enemypatterns.txt");

	enemyTextures[0].filename = "graphics/EnemyX.dds";
	enemyTextures[0].texture = 0;
	enemyTextures[1].filename = "graphics/clamfighter.dds";
	enemyTextures[1].texture = 0;
	enemyTextures[2].filename = "graphics/GroundHog.dds";
	enemyTextures[2].texture = 0;
	enemyTextures[3].filename = "graphics/SpaceRacer.dds";
	enemyTextures[3].texture = 0;
	enemyTextures[4].filename = "graphics/Brick.dds";
	enemyTextures[4].texture = 0;
	enemyTextures[5].filename = "graphics/B_Laser.dds";
	enemyTextures[5].texture = 0;
	enemyTextures[6].filename = "graphics/B_SpreadA.dds";
	enemyTextures[6].texture = 0;
	enemyTextures[7].filename = "graphics/B_SpreadB.dds";
	enemyTextures[7].texture = 0;
	enemyTextures[8].filename = "graphics/B_CoreB.dds";
	enemyTextures[8].texture = 0;
	enemyTextures[9].filename = "graphics/B_Spawn.dds";
	enemyTextures[9].texture = 0;

	POINT p;

	//Poly forEnemyX
	p.x = 4; p.y = 4;
	polyArmatures[0].push_back(p);
	p.x = 61;
	polyArmatures[0].push_back(p);
	p.y = 58;
	polyArmatures[0].push_back(p);
	p.x = 4;
	polyArmatures[0].push_back(p);
	
	//Poly for Clam
	p.x = 0; p.y = 0;
	polyArmatures[1].push_back(p);
	p.x = 56; p.y = 16;
	polyArmatures[1].push_back(p);
	p.y = 48;
	polyArmatures[1].push_back(p);
	p.x = 0; p.y = 64;
	polyArmatures[1].push_back(p);
	p.x = 38, p.y = 32;
	polyArmatures[1].push_back(p);
	
	//Poly for GroundHog
	p.x = 9; p.y = 23;
	polyArmatures[2].push_back(p);
	p.x = 18; p.y = 18;
	polyArmatures[2].push_back(p);
	p.x = 64; p.y = 48;
	polyArmatures[2].push_back(p);
	p.x = 8; p.y = 64;
	polyArmatures[2].push_back(p);

	//Poly for SpaceRacer
	p.x = 2; p.y = 32;
	polyArmatures[3].push_back(p);
	p.x = 30; p.y = 2;
	polyArmatures[3].push_back(p);
	p.x = 58; p.y = 5;
	polyArmatures[3].push_back(p);
	p.y = 60;
	polyArmatures[3].push_back(p);
	p.x = 18, p.y = 58;
	polyArmatures[3].push_back(p);

	//Poly for Brick
	p.x = 0; p.y = 0;
	polyArmatures[4].push_back(p);
	p.x = 64;
	polyArmatures[4].push_back(p);
	p.y = 64;
	polyArmatures[4].push_back(p);
	p.x = 0;
	polyArmatures[4].push_back(p);

	//Poly for enemy laser
	p.x = 16; p.y = 0;
	polyArmatures[5].push_back(p);
	p.x = 31;
	polyArmatures[5].push_back(p);
	p.y = 192;
	polyArmatures[5].push_back(p);
	p.x = 16;
	polyArmatures[5].push_back(p);
	p.x = 7; p.y = 155;
	polyArmatures[5].push_back(p);
	p.x = 1; p.y = 119;
	polyArmatures[5].push_back(p);
	p.y = 72;
	polyArmatures[5].push_back(p);
	p.x = 7; p.y = 35;
	polyArmatures[5].push_back(p);


	//Poly for enemy spread A
	p.x = 64; p.y = 0;
	polyArmatures[6].push_back(p);
	p.y = 64;
	polyArmatures[6].push_back(p);
	p.x = 15;
	polyArmatures[6].push_back(p);
	p.x = 20; p.y = 51;
	polyArmatures[6].push_back(p);
	p.x = 46; p.y = 32;
	polyArmatures[6].push_back(p);
	p.x = 0;
	polyArmatures[6].push_back(p);
	p.y = 15;
	polyArmatures[6].push_back(p);
	p.x = 47;
	polyArmatures[6].push_back(p);
	p.x = 52; p.y = 4;
	polyArmatures[6].push_back(p);

	//Poly for enemy spread B
	p.x = 15; p.y = 0;
	polyArmatures[7].push_back(p);
	p.x = 64;
	polyArmatures[7].push_back(p);
	p.y = 64;
	polyArmatures[7].push_back(p);
	p.x = 50; p.y = 56;
	polyArmatures[7].push_back(p);
	p.x = 47; p.y = 48;
	polyArmatures[7].push_back(p);
	p.x = 0;
	polyArmatures[7].push_back(p);
	p.y = 31;
	polyArmatures[7].push_back(p);
	p.x = 47;
	polyArmatures[7].push_back(p);
	p.x = 20; p.y = 11;
	polyArmatures[7].push_back(p);

	//Poly for enemy core
	p.x = 16; p.y = 0;
	polyArmatures[8].push_back(p);
	p.x = 64;
	polyArmatures[8].push_back(p);
	p.y = 64;
	polyArmatures[8].push_back(p);
	p.x = 16;
	polyArmatures[8].push_back(p);
	p.x = 19; p.y = 44;
	polyArmatures[8].push_back(p);
	p.x = 15; p.y = 37;
	polyArmatures[8].push_back(p);
	p.x = 17; p.y = 22;
	polyArmatures[8].push_back(p);
	p.x = 21; p.y = 18;
	polyArmatures[8].push_back(p);

	//Poly for enemy spawner
	p.x = 0; p.y = 0;
	polyArmatures[9].push_back(p);
	p.x = 64;
	polyArmatures[9].push_back(p);
	p.y = 64;
	polyArmatures[9].push_back(p);
	p.x = 0;
	polyArmatures[9].push_back(p);
}

/*
 * update
 *
 * based on the world x coordinate, figures out of a new enemy should spawn
 *
 * args - worldX - x coordinate of the mainground
 *
 * returns - number of points the player should receive from killing any enemies
 */
int EnemyManager::update(float dt, int worldX, int playerY, bool gravityBombed) 
{
	int numPoints = 0;

	if(!gravityBombed) {   // Update any active shots
		for(int i = 0; i < NUM_ENEMY_WEAPONS; ++i) {
			weaponArray[i]->update( dt );
		}
	}

	if(playerY == -1) {
		bossOnScreen = true;
	}

   //first, check to see if any enemies have died since last frame
	std::vector<Enemy*>::iterator iter = activeEnemies.begin();
	
	while(iter != activeEnemies.end()) 
	{
		if((*iter)->isDead() || (*iter)->isOffscreen()) 
		{
			if((*iter)->isDead())
			{
				numPoints += (*iter)->getPoints();
				POINT pos;
				pos.x = (*iter)->getPosX();
				pos.y = (*iter)->getPosY();
				FXManager::Instance()->createExplosion(pos);

				if((*iter)->getEnemyType() == CORE_ID) {
					killAllEnemies();
					coreDying = true;
				}
				if(!(*iter)->isGravityBombable()) {
					upgradeBoss = true;
				}
			}
			
			delete (*iter);
			(*iter) = 0;
			activeEnemies.erase(iter);
		}
		else 
		{
			//only update timer if currently not gbombing
			if(!gravityBombed) {
				(*iter)->updateTimer( dt );
			}

			//if the boss needs to be upgraded and this is a spread...upgrade
			if(upgradeBoss && ((*iter)->getEnemyType() == BOSS_SPREAD_A_ID || (*iter)->getEnemyType() == BOSS_SPREAD_B_ID)) {
				weaponArray[3]->upgrade();
			}

			++iter;
		}
	}

	//next...update any enemies that haven't died
	for(int i = 0; i < (int)activeEnemies.size(); ++i) 
	{
		activeEnemies[i]->update(dt, playerY, gravityBombed);
      if(activeEnemies[i]->getEnemyType() == SPAWN_ID && !gravityBombed)
      {		 
         activeEnemies[i]->updateTimer(dt);
         if(activeEnemies[i]->getTimer() > SPAWNER_TIMER)
         {
            EnemyTrigger trigger;
            trigger.enemyId = 1;
            trigger.startX = activeEnemies[i]->getPosX() + (worldX - WINDOW_WIDTH);
            trigger.startY = activeEnemies[i]->getPosY();

            spawnEnemy(trigger, worldX);
            ++i;

            activeEnemies[i]->resetTimer();
			activeEnemies[0]->setBossElement(true);
         }
         
      }

		POINT startPos;
		startPos.x = activeEnemies[i]->getPosX()+32;
		startPos.y = activeEnemies[i]->getPosY()+32;

		// If the enemy has a weapon
		if ( activeEnemies[i]->getWeaponType() != -1 )
		{
			// Fire the weapon for this enemy
			if ( activeEnemies[i]->getTimer() > activeEnemies[i]->getWeaponFireRate() )
			{
				int enemyWeaponType = activeEnemies[i]->getWeaponType();
				weaponArray[enemyWeaponType]->fire(startPos);
				activeEnemies[i]->resetTimer();

				//randomize the boss laser's firing rate
				if(activeEnemies[i]->getEnemyType() == LASER_ID) {
					activeEnemies[i]->setWeaponFireRate((float)(rand() % 4) + 2.0f);
				}
			}
		}
	}
	//next...check to see if any enemies should be spawned
	bool done = false;

	//don't spawn any enemies if it's game over (playerY = -1 when it's gameover)
	if(playerY == -1) {
		done = true;
	}

	//check to see if enemies should be spawned...
   std::vector<EnemyTrigger>::iterator triggerIter = triggers.begin();

	while(triggerIter != triggers.end() ) 
	{
      if( (*triggerIter).startX <= worldX) 
		{
			spawnEnemy((*triggerIter), worldX);
         triggers.erase((triggerIter));
		}
		else 
		{
         ++triggerIter;
			done = true;
		}
	}

	return numPoints;
}	

/*
 * spawnEnemy
 *
 * throws an enemy into combat
 *
 * args - trigger - EnemyTrigger (what kind of enemy to spawn and where)
 * arg - worldX - world x coordinate of the end of the screen
 */
void EnemyManager::spawnEnemy(EnemyTrigger trigger, int worldX) 
{
	Enemy *enemy;

	switch(trigger.enemyId) 
	{
		//Enemy X
		case 0:
			enemy = new Enemy(enemyTextures[0], enemyPatterns[4], 64, 64, 100, trigger.enemyId);
			enemy->setLife(1.5f);
			enemy->setWeaponType(0);
			enemy->setWeaponFireRate(2.0f);
			break;
		//Clam Fighter
		case 1:
			enemy = new Enemy(enemyTextures[1], enemyPatterns[1], 64, 64, 200, trigger.enemyId);
			enemy->setLife(0.8f);
         break;
		//GroundHog
		case 2:
			enemy = new Enemy(enemyTextures[2], enemyPatterns[2], 64, 64, 200, trigger.enemyId);
			enemy->setLife(2.0f);
			enemy->setWeaponType(1);
			enemy->setWeaponFireRate(1.5f);
         break;
		//Space Racer
		case 3:
			enemy = new Enemy(enemyTextures[3], enemyPatterns[6], 64, 64, 200, trigger.enemyId);
			enemy->setLife(0.5f);
			enemy->setWeaponType(0);
			enemy->setWeaponFireRate(1.0f);
			break;
		//Brick
		case 4:
			enemy = new Enemy(enemyTextures[4], enemyPatterns[2], 64, 64, 200, trigger.enemyId);
			enemy->setLife(0.5f);
			break;
      // Boss Laser
		case 5:
			enemy = new Enemy(enemyTextures[5], enemyPatterns[2], 64, 192, 200, trigger.enemyId);
			enemy->setLife(40.0f);
			enemy->setWeaponType(2);
			enemy->setWeaponFireRate(3.0f);
			enemy->setGravityBombable(false);
			enemy->setBossElement(true);
			break;
      // Boss SpreadGun A
		case 6:
			if(!bossOnScreen) {
				bossOnScreen = true;
				SoundManager::Instance()->playBackgroundMusic("audio/music/s_boss_battle.wav", true);
			}
			enemy = new Enemy(enemyTextures[6], enemyPatterns[2], 64, 64, 200, trigger.enemyId);
			enemy->setLife(15.0f);
			enemy->setWeaponType(3);
			enemy->setWeaponFireRate(2.0f);
			enemy->setGravityBombable(false);
			enemy->setBossElement(true);
			break;
		// Boss SpreadGun B
		case 7:
			enemy = new Enemy(enemyTextures[7], enemyPatterns[2], 64, 64, 200, trigger.enemyId);
			enemy->setLife(15.0f);
			enemy->setWeaponType(3);
			enemy->setWeaponFireRate(2.0f);
			enemy->setGravityBombable(false);
			enemy->setBossElement(true);
			break;
      case 8:
			// core
			enemy = new Enemy(enemyTextures[8], enemyPatterns[2], 64, 64, 200, trigger.enemyId);
			enemy->setLife(15.0f);
	        enemy->setGravityBombable(false);
			enemy->setBossElement(true);
			enemy->setAnimationTime(0.05f);
			break;
      case 9:
			//enemy spawner
			enemy = new Enemy(enemyTextures[9], enemyPatterns[2], 64, 64, 200, trigger.enemyId);
			enemy->setLife(15.0f);
			enemy->setGravityBombable(false);
			enemy->setBossElement(true);
         break;

	}
	if(enemy->getPattern()->matchWorldSpeed) {
		enemy->setDx(dx);
	}
	enemy->setPolygonArmature(*(&polyArmatures[trigger.enemyId]));
	enemy->setPosX(trigger.startX - (worldX - WINDOW_WIDTH));
	enemy->setPosY(trigger.startY);
	enemy->setDifficulty( diffMultiplier );
   activeEnemies.insert(activeEnemies.begin(), enemy);
}

/*
 * loadTriggers
 *
 * reads all the triggers in from file
 *
 * args - filename - name of file containing all enemy triggers
 */
void EnemyManager::loadTriggers(char *filename) 
{
	//open the input stream
	std::ifstream triggerFile(filename);

	//load each trigger
	while(triggerFile.peek() != -1) 
	{
		EnemyTrigger trigger;
		triggerFile >> trigger.enemyId;
		triggerFile >> trigger.startX;
		triggerFile >> trigger.startY;
		triggers.push_back(trigger);
	}

	//close the input stream
	triggerFile.close();
}

/*
 * loadTextures
 *
 * loads textures of all enemies and weapons
 *
 * returns status code of the load (did it succeed or not)
 */
HRESULT EnemyManager::loadTextures() 
{
	HRESULT r;

   // Load the textures for the enemies
	for(int i = 0; i < NUM_ENEMIES; ++i) 
	{
		r = SpriteManager::Instance()->loadTexture(&enemyTextures[i]);

		if(FAILED(r)) 
		{
			return r;
		}
	}

   // Load the textures for the weapons
	for(int i = 0; i < NUM_ENEMY_WEAPONS; ++i) {
		r = weaponArray[i]->loadTexture();
		
		if(FAILED(r)) {
			return r;
		}
	}

   return r;
}

/*
 * loadPatterns
 *
 * loads patterns for each type of enemy
 *
 * args - filename - name of the file containing pattern data
 */
void EnemyManager::loadPatterns(char *filename) 
{
	//open the input stream
	std::ifstream patternFile(filename);

	//loads patterns for each type of enemy
	for(int i = 0; i < NUM_ENEMIES; ++i) 
	{
		int numNodes;
		patternFile >> numNodes;					//how many nodes are in the current enemy's pattern
		Pattern *current, *previous = 0;
		current = enemyPatterns[i] = new Pattern;

		//loads each node, creating a circular linked list (a pattern)
		for(int j = 0; j < numNodes; ++j) 
		{
			patternFile >> current->deltaX;
			patternFile >> current->deltaY;
			patternFile >> current->matchWorldSpeed;
			patternFile >> current->followPlayer ;
			
			if(previous) 
			{
				previous->next = current;
			}
			
			previous = current;
			current = 0;
			current = new Pattern;
		}

		delete current;
		current = 0;
		previous->next = enemyPatterns[i];
	}

	//close the input stream
	patternFile.close();

}

/*
 * drawEnemies
 *
 * draws all active enemies
 *
 * returns status code of the draw (did it succeed or not)
 */
HRESULT EnemyManager::drawEnemies(int alpha) 
{
	HRESULT r = S_OK;

	for(int i = 0; i < (int)activeEnemies.size(); ++i) 
	{
		r = activeEnemies[i]->draw(alpha);

		if(FAILED(r)) 
		{
			return r;
		}
	}

	return r;
}

/*
 * drawWeapons
 *
 * draws all active enemy weapons
 *
 * returns status code of the draw (did it succeed or not)
 */
HRESULT EnemyManager::drawWeapons(int alpha) 
{
	HRESULT r = S_OK;

   // Draw any bullets
  	for(int i = 0; i < NUM_ENEMY_WEAPONS; ++i)
   {
      weaponArray[i]->draw(alpha);
   }

	return r;
}

/*
 * getActiveEnemies
 *
 * gets all the enemies that are currently active and in combat
 *
 * returns a vector of all active enemies
 */
std::vector<Enemy*> *EnemyManager::getActiveEnemies() 
{
	return &activeEnemies;
}

/*
 * Returns the array of weapons
 *
 * @returns - The array of weapons
 */
Weapon** EnemyManager::getWeapons()
{
	return weaponArray;
}

void EnemyManager::setDx(int newDx) {
	dx = newDx;

	std::vector<Enemy*>::iterator iter = activeEnemies.begin();
	while(iter != activeEnemies.end())	{
		(*iter++)->setDx(newDx);
	}
}

/*
 * Sets the difficulty multiplier effecting enemy strength.
 *
 * @args difficulty - the difficulty multiplier
 */
void EnemyManager::setDifficulty( float difficulty ) {
	diffMultiplier = difficulty;
}

/*
 * killAllEnemies
 *
 * kills all enemies on screen
 */
void EnemyManager::killAllEnemies() {
   std::vector<Enemy*>::iterator iter = activeEnemies.begin();
	
	while(iter != activeEnemies.end()) 
	{
      (*iter++)->damage(1000.0f);
	}
}

bool EnemyManager::isCoreDying() {
   return coreDying;
}