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

#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "GameCore.h"
#include "Texture.h"

class SpriteManager 
{
public:
	//returns a singleton instance of the SpriteManager
	static SpriteManager *Instance();

	HRESULT initSpriteManager(LPDIRECT3DDEVICE9 deviceHandle);

	//newly created texture put in 'texture'
	HRESULT loadTexture(Texture *newTexture);	// loads populates the .texture portion of a texture struct
	HRESULT spriteBegin();					    //needs to be called once and only once before drawSprite()
	HRESULT spriteEnd();					    //needs to be called once and only once after drawSprite()
	
	// draws the portion of a sprite indicated by sourceRect to the
	// screen at x, y coordinates
	HRESULT drawSprite(Texture *texture, RECT *sourceRect, float x, float y, int alpha = -1);
	HRESULT drawSprite(Texture *texture, RECT *sourceRect, POINT pos, D3DXMATRIX *matrix, POINT centerPos, int alpha = -1);
	void setOffsetX(int newOffsetX);
	void setOffsetY(int newOffsetY);
protected:
	//ctor/dtor
	SpriteManager();
	~SpriteManager();
private:
	LPDIRECT3DDEVICE9 d3ddHandle;	// handle to a direct3d device (needed to create a sprite)
	LPD3DXSPRITE sprite;			// handle to a sprite
	D3DCOLOR m_pModulateColor;
	int offsetX;
	int offsetY;

};

#endif