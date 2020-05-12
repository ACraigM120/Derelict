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

#ifndef TRAIL_EMITTER_H
#define TRAIL_EMITTER_H

#include "GameCore.h"
#include "Texture.h"
#include "TrailParticle.h"
#include <math.h>
#include <vector>

class TrailEmitter
{

public:
	
	// C D
	TrailEmitter(char* particleTexture, POINT pos);
	TrailEmitter();
	~TrailEmitter();

	// A M
	void setPos(POINT pos);


	// others
	void createEmitter(POINT pos);
	void update(float dt, POINT newPos);			// takes the updated position of the missile
	void updateParticles(float dt);
	bool isAlive();
	void checkStates();
	
	HRESULT loadTexture();
	HRESULT draw();
	
	void addParticles(POINT newPos);				// adds particles to the emitter to create a 
													// trail from the last known position to a new position
	float getDistance(POINT p1, POINT p2);

private:

	POINT emitterPos;						// position of the emitter

	std::vector<TrailParticle*> particles;	// this emitter's particles
	std::vector<bool> particleStates;		// state of each particle (alive or dead)
	
	bool alive;								// is this emitter active?
	Texture texture;						// texture for this emitter's particles
	
	float age;								// age of the emitter
	int distBetweenNodes;					// distance between each particle when spawned

	POINT currTrailPos;
	POINT lastTrailPos;


};


#endif