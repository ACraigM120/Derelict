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

#include "PowerUp.h"

/*
 * constructor
 */
PowerUp::PowerUp(Texture myTexture, int powerupId, int y, int spriteWidth, int spriteHeight) 
		  : Ship(spriteWidth, spriteHeight)
{
	texture = myTexture;
	
	initialY = y;
	maxY = y+20;
	minY = y-20;
	goingUp = true;
	id = powerupId;
	collected = false;
	dx = -1;

}

/*
 * destructor
 */
PowerUp::~PowerUp()
{

}

void PowerUp::setPosX(int x) 
{
	posX = x;
}

void PowerUp::setPosY(int y) 
{
	posY = y;
}

void PowerUp::update(float dt)
{
	// move across screen every frame
	setPosX(posX-dx);

	int newPosY;
	
	// "float" up and down
	if(goingUp)
	{
		newPosY = posY-1;
		if(newPosY >= minY)
		{
			setPosY(newPosY);
		}
		else
		{
			goingUp = false;
		}
	}
	else
	{
		newPosY = posY+1;
		if(newPosY <= maxY)
		{
			setPosY(newPosY);
		}
		else
		{
			goingUp = true;
		}
	}

}


/*
 * isOffscreen
 *
 * returns whether the power-up is offscreen or not
 */
bool PowerUp::isOffscreen()
{
	return (getPosX() > WINDOW_WIDTH || getPosX() < 0 - spriteWidth ||
		    getPosY() > WINDOW_HEIGHT || getPosY() < 0 - spriteHeight);
}

/*
 * isCollected
 *
 * returns whether this power-up has been collected
 */
bool PowerUp::isCollected()
{
	return collected;
}


HRESULT PowerUp::draw(int alpha)
{
	return SpriteManager::Instance()->drawSprite(&texture, &sourceRect, (float)posX, (float)posY, alpha);
}

/*
 * setPolygonArmature
 *
 * sets the polygon Armature for this enemy
 */
void PowerUp::setPolygonArmature(std::vector<POINT> points) 
{
	boundingPoints = points;
}


void PowerUp::damage(float strength)
{

}

void PowerUp::collectMe()
{
	collected = true;
}

/*
 * getID
 *
 * gets the numerical ID of this powerup
 *
 * returns numerical ID of this powerup
 */
int PowerUp::getID() {
	return id;
}

/*
 * setDx
 *
 * changes the rate at which the powerups scroll across the screen
 *
 * arg - new dx - new delta x (how fast powerup should move across the screen in pixels)
 */
void PowerUp::setDx(int newDx) {
	dx = newDx;
}