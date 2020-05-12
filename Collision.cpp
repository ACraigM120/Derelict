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

#include "Collision.h"

/*
 * checkPolyArmature
 *
 * compares polygon armature of a ship and a tile to see if there is line intersection (collision)
 * line intersection equation stolen from andy's paper on collision detection. adds xn and yn to each
 * vertex to convert from sprite relative to world relative coordinates, then checks for collision
 *
 * arg - points1 - vertices of the 1st object's collision box
 * arg - x1 - world x offset of 1st object
 * arg - y1 - world y offset of 1st object
 * arg - points2 - vertices of the 2nd object's collision box
 * arg - x2 - world x offset of 2nd object
 * arg - y2 - world y offset of 2nd object

 * returns true if there is a collision, false o.w.
 */
bool checkPolyArmature(std::vector<POINT> *points1, int x1, int y1, std::vector<POINT> *points2, int x2, int y2) {
   //for each line segment in the first object
   for(int i = 0; i < (int)((*points1).size()); ++i) 
   {
		//get the 2 endpoints of the current line segment being checked
		POINT a = (*points1)[i];
		POINT b = (*points1)[(i+1) % (*points1).size()];

		//add the offset so the points are now in world coordinates
		a.x += x1;
		a.y += y1;
		b.x += x1;
		b.y += y1;

		//for each line segment in the second object
		for(int j = 0; j < (int)((*points2).size()); ++j) 
		{
			//get the 2 endpoints of the current line segment being checked
			POINT c = (*points2)[j];
			POINT d = (*points2)[(j+1) % (*points2).size()];

			//add the offset so the points are now in world coordinates
			c.x += x2;
			c.y += y2;
			d.x += x2;
			d.y += y2;

			//the equation is...
			//      (ay - cy)(dx - cx) - (ax - cx)(dy - cy)
			//  s = ---------------------------------------
			//      (bx - ax)(dy - cy) - (by - ay)(dx - cx)
			//
			//      (ay - cy)(bx - ax) - (ax - cx)(by - ay)
			//  r = ---------------------------------------
			//      (bx - ax)(dy - cy) - (by - ay)(dx - cx)
			//
			// if 0 < s < 1 AND 0 < r < 1, the lines intersect
			// NOTE: the denominator is the same in each, so it only has to be calc'd once
			
			int denominator = (b.x - a.x)*(d.y - c.y) - (b.y - a.y)*(d.x - c.x);
			int rNumerator = (a.y - c.y)*(d.x - c.x) - (a.x - c.x)*(d.y - c.y);
			int sNumerator = (a.y - c.y)*(b.x - a.x) - (a.x - c.x)*(b.y - a.y);

			float r = (float)rNumerator/(float)denominator;
			float s = (float)sNumerator/(float)denominator;

			if(s > 0.0 && s < 1.0 && r > 0.0 && r < 1.0) 
			{
				return true;
			}
		}
   }

   return false;
}
