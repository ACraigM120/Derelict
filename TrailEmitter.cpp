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

#include "TrailEmitter.h"

// constructor
TrailEmitter::TrailEmitter(char* particleTexture, POINT pos)
{
	// initialize
	age = 0.0f;
	alive = true;
	distBetweenNodes = 1;

	// load texture
	texture.filename = particleTexture;
	texture.texture = 0;        // zero the texture pointer
	
	// load texture
	HRESULT r = loadTexture();
	if(FAILED(r))
	{
		GameError("failed loading emitter texture...");
	}

	pos.y -= 4;

	setPos(pos);
	addParticles(pos);
	currTrailPos = pos;
	lastTrailPos = pos;

	GameError("TrailEmitter constructor called");

}

TrailEmitter::TrailEmitter()
{
}

// desctructor
TrailEmitter::~TrailEmitter()
{
	GameError("TrailEmitter destructor called");
	
	// if a texture object has been assigned, release it
	if(texture.texture) 
	{
		char buf[250];
		sprintf(buf, "Releasing texture: %s", texture.filename);
		GameError(buf);
		texture.texture->Release();
		texture.texture = 0;
	}

	// delete my particle vector
	std::vector<TrailParticle*>::iterator iter2 = particles.begin();
	while(iter2 != particles.end())
	{
		delete (*iter2);
		(*iter2) = 0;
		particles.erase(iter2);
	}

	// clear out states
	std::vector<bool>::iterator iter = particleStates.begin();
	while(iter != particleStates.end())
	{
		particleStates.erase(iter);
	}

}

void TrailEmitter::createEmitter(POINT pos)
{

}

// 

void TrailEmitter::setPos(POINT pos)
{
	emitterPos = pos;
}


void TrailEmitter::update(float dt, POINT newPos)
{
	bool oneAlive;
	
	newPos.y -= 4;
	
	// clear out states
	std::vector<bool>::iterator iter = particleStates.begin();
	while(iter != particleStates.end())
	{
		particleStates.erase(iter);
	}
	
	// populate states
	checkStates();

	oneAlive = false;

	// loop through the states...
	std::vector<bool>::iterator iter2 = particleStates.begin();
	while(iter2 != particleStates.end())
	{
		if((*iter2))
		{
			oneAlive = true;
		}

		++iter2;
	}

	// if at least one particle is alive, emitter is still active
	if(oneAlive)
	{
		// set current trail position
		currTrailPos = newPos;

		// if distance from last trail particle is great enough, add another
		if(getDistance(currTrailPos, lastTrailPos) >= distBetweenNodes)
		{
			// add particles to trail 
			addParticles(newPos);

			// update positions
			lastTrailPos = currTrailPos;
		}

		// update all particles
		updateParticles(dt);
	}
	
	// if all particles are dead, kill emitter
	else
	{
		alive = false;
		return;
	}

}


HRESULT TrailEmitter::loadTexture()
{
	HRESULT r;

	r = SpriteManager::Instance()->loadTexture(&texture);
	if(FAILED(r))
	{
		GameError("failed loading particle texture");
		return r;
	}

	return r;
	
}


HRESULT TrailEmitter::draw()
{
	HRESULT r = S_OK;

	// iterate through active particles, drawing them if alive
	std::vector<TrailParticle*>::iterator iter = particles.begin();
	while(iter != particles.end())
	{
		if((*iter)->getState() == 1)
		{
			r = (*iter)->draw();
			if(FAILED(r))
			{
				GameError("failed drawing a particle");
			}
		}

		++iter;
	}

	if(FAILED(r))
	{
		GameError("failed drawing particles");
	}

	return r;
}


bool TrailEmitter::isAlive()
{
	return alive;
}


void TrailEmitter::updateParticles(float dt)
{
	// iterate through particles, updating if alive
	std::vector<TrailParticle*>::iterator iter = particles.begin();
	while(iter != particles.end())
	{
		(*iter)->update(dt);

		++iter;
	}
}


void TrailEmitter::checkStates()
{
	// iterate through particles
	
	std::vector<TrailParticle*>::iterator iter = particles.begin();
	while(iter != particles.end())
	{
		// if the particle is alive or waiting, state is true
		if(((*iter)->getState() == 1) || ((*iter)->getState() == 2))
		{
			particleStates.push_back(true);
		}
	
		// node is dead, state is false
		else if((*iter)->getState() == 0)
		{
			particleStates.push_back(false);
		}

		++iter;
	}
}

void TrailEmitter::addParticles(POINT newPos)
{
	TrailParticle* particle;
	
	// just add one particle to the given position
	particle = new TrailParticle(&texture);
	newPos.x -= 20;
	particle->setPos(newPos);
	particle->spawn(newPos);
	particles.push_back(particle);
	

}


float TrailEmitter::getDistance(POINT p1, POINT p2)
{
	return sqrt((float)(((p2.x - p1.x)*(p2.x - p1.x)) + ((p2.y-p1.y)*(p2.y-p1.y))));
}