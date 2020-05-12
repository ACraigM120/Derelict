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

#ifndef BULLET_DATA_H
#define BULLET_DATA_H

// This is used so that if an enemy is hit by the laser
// The "bullet" does not get deleted.
//
enum BulletType {
	NORMAL = 0,
	LASER = 1,
};

struct BulletData
{
   POINT position;
   POINT lastPosition;
   POINT target;
   RECT animateRect;
   RECT srcRect;
   int direction;
   int distance;
   float velocityX;
   float velocityY;
   float animateTimer;
   BulletType type;
   D3DXMATRIX transform;
   float rotation;

   // Initilize to default values.
   BulletData()
   {
     // GameError("creating bullet data");
	  position.x = position.y = 0;
	  lastPosition.x = lastPosition.y = 0;
      target.x = target.y = 0;
      animateRect.bottom = animateRect.left = animateRect.right = animateRect.top = 0;
      srcRect.bottom = srcRect.left = srcRect.right = srcRect.top = 0;
      direction = 0;
      distance = 0;
      velocityX = 0.0;
      velocityY = 0.0;
      animateTimer = 0.0;
      type = NORMAL;
      rotation = 0.0;
   }

   ~BulletData()
   {
	//	GameError("BulletData destructor called");
   }
};

#endif