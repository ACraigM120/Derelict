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

#include "TrailParticle.h"

// constructor
TrailParticle::TrailParticle(Texture* texture)
{
	//GameError("particle constructor called");
	
	// initialize
	myState = waiting;
	myScaleAmount = 1.0f;
	myRotation = 0.0f;
	myAge = 0.0f;
	animTimer = 0.0f;
	currFrame = 1;
	numFrames = 5;

	// trail particle spawn time is *always* 0.0
	mySpawnTime = 0.0f;
	
	// originally held offscreen
	myPos.x = -300;

	myTexture = texture;
	myTexture->red = 255;
	myTexture->blue = 255;
	myTexture->green = 255;

	srcRect.top = 0;
	srcRect.bottom = TRAIL_PARTICLESIZE;
	srcRect.left = 0;
	srcRect.right = TRAIL_PARTICLESIZE;


}

//TrailParticle::TrailParticle()
//{
//}

TrailParticle::~TrailParticle()
{


}

// **** Accessors and Mutators *** //

int TrailParticle::getState()
{
	return myState;
}

RECT* TrailParticle::getSrcRect()
{
	return &srcRect;
}

float TrailParticle::getRotation()
{
	return myRotation;
}

float TrailParticle::getScale()
{
	return myScaleAmount;
}

float TrailParticle::getAge()
{
	return myAge;
}

POINT TrailParticle::getSpeed()
{
	return speed;
}

POINT TrailParticle::getPos()
{
	return myPos;
}

void TrailParticle::setPos(POINT pos)
{
	myPos = pos;
}

void TrailParticle::setSpeed(POINT newSpeed)
{
	speed = newSpeed;
}

void TrailParticle::setSpeed(int x, int y)
{
	speed.x = x;
	speed.y = y;
}

void TrailParticle::setState(State state)
{
	myState = state;
}

void TrailParticle::setSpawnTime(float time)
{
	mySpawnTime = time;
}

// ***************************//

/*
 * update
 *
 * takes care of all transformations for this sprite.  animation,
 * scale, and rotation are all based on time.  This should not
 * be called if the particle is dead.
 *
 * params - dt -> time passed since last frame
 *
 */
void TrailParticle::update(float dt)
{
	// increment age timer
	myAge += dt;

	if(myState == alive)
	{
		animTimer += dt;
		
		//if its time to animate, increment source rect
		if(animTimer >= TRAIL_ANIMSPEED)
		{
			currFrame++;
			if(currFrame >= numFrames)
			{
				die();
				return;
			}
			else
			{
				srcRect.left = (srcRect.left + TRAIL_PARTICLESIZE) % myTexture->width;
				srcRect.right = (srcRect.left + TRAIL_PARTICLESIZE);

				// reset timer
				animTimer = 0.0f;
			}
		}

	}

}

/*
 * spawn
 *
 * sets this particle's state to alive, placing it at an emitter position
 *
 * params - pos -> position to start at
 *
 */
void TrailParticle::spawn(POINT position)
{
	myPos = position;
	speed.x = 0;
	speed.y = 0;
	
	myState = alive;
}

/*
 * die
 *
 * sets particle's state to dead, places it offscreen
 *
 */
void TrailParticle::die()
{
	myPos.x = -300;
	srcRect.left += TRAIL_PARTICLESIZE;
	srcRect.right = srcRect.left + TRAIL_PARTICLESIZE;

	myState = dead;
}

/*
 * draw
 * 
 * takes care of drawing this particle, using the transform matrix
 * calculated in update()
 *
 */
HRESULT TrailParticle::draw()
{
	HRESULT r;
	POINT center;
	center.x = myPos.x+(TRAIL_PARTICLESIZE/2);
	center.y = myPos.y+(TRAIL_PARTICLESIZE/2);

	r = SpriteManager::Instance()->drawSprite(myTexture, &srcRect, myPos.x, myPos.y, -1);

	return r;

}