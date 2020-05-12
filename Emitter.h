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

#ifndef EMITTER_H
#define EMITTER_H

#include "GameCore.h"
#include "Texture.h"
#include "Particle.h"
#include "LookUp.h"
#include <vector>

#define NUMTREELEVELS 4
#define NUMPARTICLES (1 << NUMTREELEVELS) - 1

class Emitter
{

public:
	
	// C D
	Emitter(char* particleTexture, POINT pos);
	~Emitter();

	// A M
	void setPos(POINT pos);


	// others
	void update(float dt);
	bool isAlive();
	
	HRESULT loadTexture();
	HRESULT draw();
	HRESULT drawParticles(Particle* particle);

	void buildPositionTree(Particle* baseNode);
	void clearTree(Particle* baseNode);
	
	void checkStates(Particle* baseNode);

	void updateParticles(Particle* particle, float dt);

private:

	POINT emitterPos;					// position of the emitter
	int numParticles;					// number of particles in this emitter

	Particle baseParticle;
	
	bool alive;							// is this emitter active?
	bool treeBuilt;						// has the tree been built?
	Texture texture;					// texture for this emitter's particles
	
	int index;							// index for spawning particles
	float age;
	int distBetweenNodes;

	int degree;
	float angle;
	float sine;
	float cosine;
	POINT pos[2];

	int particleIndex;
	
	std::vector<bool> particleStates;


};


#endif