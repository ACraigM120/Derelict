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

#ifndef TEXTURE_H
#define TEXTURE_H

#include "GameCore.h"

typedef struct 
{
	LPDIRECT3DTEXTURE9 texture;
	int width, height;		//width and height of the sprite in pixels
	char* filename;			//path of image used for sprite
	int alpha, red, blue, green;   //modulation colors
	int spriteSize;         //the size one sprite from this texture should be (in pixels)
} Texture;
#endif
