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

#include "Particle.h"

// constructor
Particle::Particle(Texture* texture)
{
	//GameError("particle constructor called");
	
	// initialize
	myState = waiting;
	myScaleAmount = 1.0f;
	myRotation = 0.0f;
	myAge = 0.0f;
	animTimer = 0.0f;
	currFrame = 1;
	numFrames = 15;
	
	// originally held offscreen
	myPos.x = -300;

	myTexture = texture;
	myTexture->red = 255;
	myTexture->blue = 255;
	myTexture->green = 255;

	srcRect.top = 0;
	srcRect.bottom = PARTICLESIZE;
	srcRect.left = 0;
	srcRect.right = PARTICLESIZE;


}

Particle::Particle()
{
}

Particle::~Particle()
{


}

// **** Accessors and Mutators *** //

int Particle::getState()
{
	return myState;
}

RECT* Particle::getSrcRect()
{
	return &srcRect;
}

float Particle::getRotation()
{
	return myRotation;
}

float Particle::getScale()
{
	return myScaleAmount;
}

float Particle::getAge()
{
	return myAge;
}

POINT Particle::getSpeed()
{
	return speed;
}

void Particle::setSpeed(POINT newSpeed)
{
	speed = newSpeed;
}

void Particle::setSpeed(int x, int y)
{
	speed.x = x;
	speed.y = y;
}

void Particle::setState(State state)
{
	myState = state;
}

void Particle::setSpawnTime(float time)
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
void Particle::update(float dt)
{
	//HRESULT r;
	//char buffer[250];
	
	// increment age timer
	myAge += dt;

	// spawn if its time
	if((myAge >= mySpawnTime) && (myState == waiting))
	{
		spawn(position);
	}
	else if(myState == alive)
	{
		animTimer += dt;
		
		//if its time to animate, increment source rect
		if(animTimer >= ANIMSPEED)
		{
			currFrame++;
			if(currFrame >= numFrames)
			{
				die();
				return;
			}
			else
			{
				srcRect.left = (srcRect.left + PARTICLESIZE) % myTexture->width;
				srcRect.right = (srcRect.left + PARTICLESIZE);

				// reset timer
				animTimer = 0.0f;
			}
		}

		// rotation is a function of time
		myRotation += myAge*myAge*myAge*myAge;

		// scale is also a function of time
		myScaleAmount += myAge/100;

		// update position
		myPos.x += speed.x;
		myPos.y += speed.y;

		// set rotation matrix
		//D3DXMatrixRotationZ(&rotationMatrix, (myRotation * float(degrad)));
		D3DXMatrixRotationZ(&rotationMatrix, 0);

		// set scale matrix
		D3DXMatrixScaling(&scaleMatrix, myScaleAmount, myScaleAmount, 1.0f);

		// set translation matrix (may not need this for dx9)
		D3DXMatrixTranslation(&translationMatrix, (float)myPos.x, (float)myPos.y, 0.0f);

		// concatenate matrices
		D3DXMatrixMultiply(&myTransform, &rotationMatrix, &scaleMatrix); 
		D3DXMatrixMultiply(&myTransform, &myTransform, &translationMatrix);

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
void Particle::spawn(POINT position)
{
	myPos = position;
	myScaleAmount = (float)((1.0f/depth)+.5);
	speed.x = -2;
	speed.y = 0;
	
	myState = alive;
}

/*
 * die
 *
 * sets particle's state to dead, places it offscreen
 *
 */
void Particle::die()
{
	myPos.x = -300;
	srcRect.left = 0;
	srcRect.right = PARTICLESIZE;

	myState = dead;
}

/*
 * draw
 * 
 * takes care of drawing this particle, using the transform matrix
 * calculated in update()
 *
 */
HRESULT Particle::draw()
{
	HRESULT r;
	POINT center;
	center.x = myPos.x+(PARTICLESIZE/2);
	center.y = myPos.y+(PARTICLESIZE/2);

	r = SpriteManager::Instance()->drawSprite(myTexture, &srcRect, myPos, &myTransform, center);

	return r;

}
