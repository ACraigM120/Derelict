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

#include "PowerUpManager.h"
#include <fstream>

/*
 * returns an instance of PowerUpManager
 */
PowerUpManager* PowerUpManager::Instance()
{
	static PowerUpManager mgr;
	return &mgr;
}

/*
 * constructor
 */
PowerUpManager::PowerUpManager()
{

}


/* 
 * destructor
 */
PowerUpManager::~PowerUpManager()
{
	// delete any power-ups still onscreen
	for(int i = 0; i < (int)activePowerUps.size(); i++)
	{
		delete activePowerUps[i];
		activePowerUps[i] = 0;
	}

	// release all textures
	for(int i = 0; i < NUM_TYPES; i++)
	{
		if(textures[i].texture)
		{
			textures[i].texture->Release();
			textures[i].texture = 0;
		}
	}
}


/*
 * reset -- puts manager in initial state.... used when a new game is started
 */
void PowerUpManager::reset()
{
	// delete any power-ups still onscreen
	std::vector<PowerUp*>::iterator iter = activePowerUps.begin();
	while(iter != activePowerUps.end())
	{
		delete (*iter);
		activePowerUps.erase(iter);
	}

	// remove triggers
	std::vector<PowerUpTrigger>::iterator trigIter = triggers.begin();
	while(trigIter != triggers.end())
	{
		triggers.erase(trigIter);
	}

	// reload triggers
	loadTriggers(triggerFileName);

}


/*
 * initialization of PowerUpManager instance
 *
 * params -- filename that contains triggers for all power-ups
 *
 */
void PowerUpManager::initPowerUpManager(char* filename)
{
	// setup triggers
	triggerFileName = filename;
	loadTriggers(triggerFileName);

	// setup textures
	// hard coded....?
	textures[0].filename = "graphics/powerups/wavegunpowerup.dds";
	textures[0].texture = 0;
	textures[1].filename = "graphics/powerups/spreadgunpowerup.dds";
	textures[1].texture = 0;
	textures[2].filename = "graphics/powerups/laserpowerup.dds";
	textures[2].texture = 0;
	textures[3].filename = "graphics/powerups/rocketpowerup.dds";
	textures[3].texture = 0;
	textures[4].filename = "graphics/powerups/gravitybombpowerup.dds";
	textures[4].texture = 0;
	textures[5].filename = "graphics/powerups/1uppowerup.dds";
	textures[5].texture = 0;
	// .... needs to be done for all types of power-ups

	
	// setup polygon armatures....?
	// all are the same shape, so populate the armatures array with the same values
	POINT p;

	for(int i = 0; i < NUM_TYPES; i++)
	{
		p.x = 5; p.y = 9;
		polyArmatures[i].push_back(p);
		p.x = 27;
		polyArmatures[i].push_back(p);
		p.x = 32; p.y = 14;
		polyArmatures[i].push_back(p);
		p.x = 32; p.y = 18;
		polyArmatures[i].push_back(p);
		p.x = 29; p.y = 22;
		polyArmatures[i].push_back(p);
		p.x = 4;
		polyArmatures[i].push_back(p);
		p.x = 0; p.y = 19;
		polyArmatures[i].push_back(p);
		p.y = 15;
		polyArmatures[i].push_back(p);
	}

}


/*
 * update
 *
 * params -- dt - time from last frame
 *			 worldX - current X position of the level, used to tell when to spawn
 * 		     a power-up
 *			 playerY - Y position of player
 */
void PowerUpManager::update(float dt, int worldX, int playerY)
{
	//char buffer[250];
	// check to see if any power-ups have been collected or
	// gone offscreen
	std::vector<PowerUp*>::iterator iter = activePowerUps.begin();
	while(iter != activePowerUps.end())
	{
		if((*iter)->isCollected() || (*iter)->isOffscreen())
		{
			bool collected = (*iter)->isCollected();
			bool off = (*iter)->isOffscreen();
			
			delete (*iter);
			(*iter) = 0;
			activePowerUps.erase(iter);
		}
		else
		{
			++iter;
		}

	}

	// update any power-ups that haven't died
	for(int i = 0; i < (int)activePowerUps.size(); ++i)
	{
		activePowerUps[i]->update(dt);
	}

	// check to see if any power-ups should be spawned
	bool done = false;

	// don't spawn if the game is over (playerY = -1)
	if(playerY == -1)
	{
		done = true;
	}

   // Spawn the powerups
	//check to see if enemies should be spawned...
   std::vector<PowerUpTrigger>::iterator triggerIter = triggers.begin();

	while(triggerIter != triggers.end() ) 
	{
      if( ((*triggerIter).startX) <= worldX) 
		{
			spawnPowerUp((*triggerIter), worldX);
         triggers.erase((triggerIter));
		}
		else 
		{
         ++triggerIter;
			done = true;
		}
	}
}

/*
 * spawnPowerUP
 *
 * creates a power-up and puts it onscreen 
 *
 */
void PowerUpManager::spawnPowerUp(PowerUpTrigger trigger, int worldX)
{
	PowerUp* powerUp = new PowerUp(textures[trigger.powerUpID], trigger.powerUpID, trigger.startY, 32, 32);
  	powerUp->setPosX(trigger.startX - (worldX - WINDOW_WIDTH));
	powerUp->setPosY(trigger.startY);
	powerUp->setPolygonArmature(*(&polyArmatures[0]));
	powerUp->setDx(dx);
	activePowerUps.push_back(powerUp);
}

/*
 * loadTriggers
 *
 * loads all triggers from the specified file
 */
void PowerUpManager::loadTriggers(char* fileName)
{
	//open the input stream
	std::ifstream triggerFile(fileName);

	//load each trigger
	while(triggerFile.peek() != -1) 
	{
		PowerUpTrigger trigger;
		triggerFile >> trigger.powerUpID;
		triggerFile >> trigger.startX;
		triggerFile >> trigger.startY;
		triggers.push_back(trigger);
	}

	//close the input stream
	triggerFile.close();
}

HRESULT PowerUpManager::loadTextures()
{
	HRESULT r;

	for(int i = 0; i < NUM_TYPES; i++)
	{
		r = SpriteManager::Instance()->loadTexture(&textures[i]);
		if(FAILED(r))
		{
			return r;
		}
	}

	return r;
}

HRESULT PowerUpManager::draw(int alpha)
{
	HRESULT r = S_OK;

	// draw all active power-ups
	for(int i = 0; i < (int)activePowerUps.size(); i++)
	{
		//GameError("drawing powerup");
		r = activePowerUps[i]->draw(alpha);
		if(FAILED(r))
		{
			GameError("failed drawing powerup");
			return r;
		}
	}

	return r;
}

/*
 * getActivePowerUps
 *
 * returns the vector of active power-ups
 */
std::vector<PowerUp*> *PowerUpManager::getActivePowerUps()
{
	return &activePowerUps;
}

/*
 * setDx
 *
 * changes the rate at which the powerups scroll across the screen
 *
 * arg - new dx - new delta x (how fast powerup should move across the screen in pixels)
 */
void PowerUpManager::setDx(int newDx) {
	dx = newDx;

	std::vector<PowerUp*>::iterator iter = activePowerUps.begin();
	while(iter != activePowerUps.end())	{
		(*iter++)->setDx(newDx);
	}
}