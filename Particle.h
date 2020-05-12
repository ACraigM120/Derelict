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

#ifndef PARTICLE_H
#define PARTICLE_H

#include "GameCore.h"
#include "Texture.h"
#include "SpriteManager.h"

#define PARTICLESIZE 64
#define ANIMSPEED 1.0f/15.0f

class Particle
{
public:

	enum State
	{
		dead = 0,
		alive = 1,
		waiting = 2
	};
	
	// C D
	Particle(Texture* texture);
	Particle();
	~Particle();


	// A M
	RECT* getSrcRect();
	float getRotation();
	float getScale();
	float getAge();
	POINT getSpeed();
	int getState();

	void setSpeed(int x, int y);
	void setSpeed(POINT newSpeed);
	void setState(State state);
	void setSpawnTime(float time);



	// other stuff
	void update(float dt);
	void rotateMe(float degrees);
	void scale(float scaler);
	
	void spawn(POINT position);
	void spawnChildren();
	void die();
	
	HRESULT draw();

	// public members.... here as a substitute for PositionNode
	// perhaps make them private in the future.....
	Particle* childOne;
	Particle* childTwo;

	POINT position;
	int depth;



private:

	RECT srcRect;
	POINT speed;
	POINT myPos;

	float myAge;
	float animTimer;
	float myRotation;
	float myScaleAmount;
	float mySpawnTime;
	
	State myState;

	D3DXMATRIX myTransform;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX scaleMatrix;
	D3DXMATRIX translationMatrix;

	Texture* myTexture;

	int currFrame;
	int numFrames;
};




#endif