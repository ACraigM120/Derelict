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

#ifndef TILE_H
#define TILE_H

#include "GameCore.h"
#include "Texture.h"

typedef struct
{
	Texture tileTexture;
	std::vector<POINT> boundingVertices; //for poly armature
	
	//for animation
	float secsPerFrame, secsLeft;
	RECT sourceRect;
} Tile;

#endif