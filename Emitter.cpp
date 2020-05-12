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

#include "Emitter.h"

// constructor
Emitter::Emitter(char* particleTexture, POINT pos)
{
	// initialize
	index = 0;
	age = 0.0f;
	alive = false;
	treeBuilt = false;
	distBetweenNodes = 50;

	particleIndex = 0;

	// load texture
	texture.filename = particleTexture;
	texture.texture = 0;        // zero the texture pointer

	//GameError("emitter constructor called");

	alive = true;
	
	// load texture
	HRESULT r = loadTexture();
	if(FAILED(r))
	{
		GameError("failed loading emitter texture...");
	}
	
	setPos(pos);

	// build a particle tree
	baseParticle = Particle(&texture);
	baseParticle.depth = 1;
	baseParticle.position = pos;
	buildPositionTree(&baseParticle);
	treeBuilt = true;

}

// desctructor
Emitter::~Emitter()
{
	//GameError("emitter destructor called");
	
	// if a texture object has been assigned, release it
	if(texture.texture) 
	{
		char buf[250];
		sprintf(buf, "Releasing texture: %s", texture.filename);
		GameError(buf);
		texture.texture->Release();
		texture.texture = 0;
	}

	// delete my position tree
	if(treeBuilt)
	{
		clearTree(&baseParticle);
	}

	// clear out states
	std::vector<bool>::iterator iter = particleStates.begin();
	while(iter != particleStates.end())
	{
		particleStates.erase(iter);
	}

}

// 

void Emitter::setPos(POINT pos)
{
	emitterPos = pos;
}

void Emitter::update(float dt)
{
	bool oneAlive;
	
	// clear out states
	std::vector<bool>::iterator iter = particleStates.begin();
	while(iter != particleStates.end())
	{
		particleStates.erase(iter);
	}
	
	// populate states
	checkStates(&baseParticle);

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
		updateParticles(&baseParticle, dt);
	}
	
	// if all particles are dead, kill emitter
	else
	{
		alive = false;
		return;
	}

}


HRESULT Emitter::loadTexture()
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


HRESULT Emitter::draw()
{
	HRESULT r = S_OK;

	r = drawParticles(&baseParticle);
	if(FAILED(r))
	{
		GameError("failed drawing particles");
	}

	return r;
}

HRESULT Emitter::drawParticles(Particle* particle)
{
	HRESULT r = S_OK;
	
	if(particle->depth != NUMTREELEVELS)
	{
		drawParticles(particle->childOne);
		drawParticles(particle->childTwo);
	}

	if(particle->getState() == 1)
	{
		r = particle->draw();
	}

	return r;
}


void Emitter::buildPositionTree(Particle* baseParticle)
{	
	// leaf nodes of this base node
	Particle* tempNode1 = new Particle(&texture);
	Particle* tempNode2 = new Particle(&texture);

	// set depths
	tempNode1->depth = tempNode2->depth = baseParticle->depth+1;
	
	// set positions
	
	for(int i = 0; i < 2; i++)
	{
		
		// random degree turn from parent node
		degree = rand()%360;

		// convert degree to angle
		angle = (float)(degree*degrad);

		// get some sin and cos values
		sine = LookUp::Instance()->getSin(degree);
		cosine = LookUp::Instance()->getCos(degree);
	
		// get points based on parent's position
		pos[i].x = (long)(baseParticle->position.x + (cosine*distBetweenNodes));
		pos[i].y = (long)(baseParticle->position.y + (sine*distBetweenNodes));
	}

	tempNode1->position = pos[0];
	tempNode2->position = pos[1];

	// add me to tree
	baseParticle->childOne = tempNode1;
	baseParticle->childTwo = tempNode2;

	if(tempNode1->depth < NUMTREELEVELS)
	{
		// hurray for recursion
		buildPositionTree(tempNode1);
		buildPositionTree(tempNode2);
	}
	else
	{
		// leaf nodes are null
		tempNode1->childOne = tempNode2->childOne = tempNode1->childTwo = tempNode2->childTwo = 0;
	}

	baseParticle->setSpawnTime(.1f*baseParticle->depth);

}

void Emitter::clearTree(Particle* node)
{
	if(node)
	{
		clearTree(node->childOne);
		clearTree(node->childTwo);

		if(node->depth != 1)
		{
			delete node;
			node = 0;
		}
	}
}


bool Emitter::isAlive()
{
	return alive;
}


void Emitter::updateParticles(Particle* particle, float dt)
{
	if(particle->depth != NUMTREELEVELS)
	{
		updateParticles(particle->childOne, dt);
		updateParticles(particle->childTwo, dt);
	}

	particle->update(dt);
}


/*
 * checkStates
 * 
 * recursive function that will check to see if at least one
 * particle is alive.  If they are all dead, the emitter
 * will be destructed.
 *
 * parameters -> baseNode - basenode in the particle tree
 *
 */
void Emitter::checkStates(Particle* node)
{
	//char buffer[250];
//	bool state;

	// recursion
	if(node->depth != NUMTREELEVELS)
	{
		checkStates(node->childOne);
		checkStates(node->childTwo);
	}

	// if the node is alive or waiting, state is true
	if((node->getState() == 1) || (node->getState() == 2))
	{
		particleStates.push_back(true);
	}
	
	// node is dead, state is false
	else if(node->getState() == 0)
	{
		particleStates.push_back(false);
	}
}	