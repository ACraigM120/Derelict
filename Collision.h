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

#ifndef COLLISION_H
#define COLLISION_H

#include "GameCore.h"
#include <vector>

bool checkPolyArmature(std::vector<POINT> *points1, int x1, int y1,
						   std::vector<POINT> *points2, int x2, int y2);

#endif