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

#include "SpriteManager.h"

/*
 * constructor
 */
SpriteManager::SpriteManager() 
{
	//zero the pointers
	sprite = 0;
	d3ddHandle = 0;
	offsetX = 0;
	offsetY = 0;

	m_pModulateColor = D3DCOLOR_XRGB(255,255,255); //no effect by default
}

/*
 * Returns the instance of the SpriteManager object. 
 *
 * @returns - The one (and only) instance of SoundManager
 */
SpriteManager *SpriteManager::Instance() {
	//instead of 'new'ing 'this' and returning the pointer, just return the address. this way we dont
	//have to worry about explicitly calling the destructor of 'this'
	static SpriteManager spriteMgr;
	return &spriteMgr; 
}

/*
 * destructor
 */
SpriteManager::~SpriteManager() 
{
	if(sprite) 
	{
		sprite->Release();
		sprite = 0;
	}

}

/*
 * initSpriteManager
 *
 * initializes the LPD3DXSPRITE, which is used to draw textured sprites
 *
 * returns S_OK, error code otherwise
 */
HRESULT SpriteManager::initSpriteManager(LPDIRECT3DDEVICE9 deviceHandle) 
{
	d3ddHandle = deviceHandle;

	// D3DXCreateSprite takes a handle to a directx device and
	// a reference to a sprite object to hold the created sprite
	HRESULT r = D3DXCreateSprite(d3ddHandle, &sprite);

	if(FAILED(r)) 
	{
		GameError("SpriteManager: Unable to create sprite");
	}

	return r;
}

/*
 * loadTexture
 *
 * loads the given texture into memory. this function assumes the parameter being given
 * has a valid filename and contains the width and height of the texture being loaded into memory.
 *
 * param - newTexture - Texture struct containing filename, width, and height of texture to be loaded.
 *                      after successfully loading the texture into memory, a pointer to it is assigned to
 *                      'newTexture'->texture
 */
HRESULT SpriteManager::loadTexture(Texture *newTexture) 
{
	
	//reads texture from file
	
	//initialize modulation colors to default values
	newTexture->alpha = newTexture->red = newTexture->green = newTexture->blue = 255;

	// get the image info
	D3DXIMAGE_INFO d3dxImageInfo;
	HRESULT r = D3DXGetImageInfoFromFile(newTexture->filename, &d3dxImageInfo);
	if(FAILED(r))
	{
		GameError("failed to get image info");
	}
	
	//get the width and height of the texture
	newTexture->width = d3dxImageInfo.Width;
	newTexture->height = d3dxImageInfo.Height;

	// create texture, stored in newTexture->texture (see last parameter)
	r = D3DXCreateTextureFromFileEx(d3ddHandle,				//  handle to a direct3d device
								    newTexture->filename,	// filename of texture to load
								    newTexture->width,		// width of texture
								    newTexture->height,		// height of texture
									1,
									D3DPOOL_DEFAULT,		
									d3dxImageInfo.Format,
									D3DPOOL_MANAGED,
									D3DX_DEFAULT,
									D3DX_DEFAULT,
									0,						// colorkey info
									&d3dxImageInfo,			// where to store image info
									NULL,
									&newTexture->texture);	// where to store loaded texture

	if(FAILED(r)) 
	{
		GameError("could not load texture:");
		GameError(newTexture->filename);
	}

	return r;
}

/*
 * spriteBegin
 *
 * needs to be called once before drawing all sprites in a given frame
 *
 * returns S_OK if successful, error code otherwise
 */
HRESULT SpriteManager::spriteBegin() 
{
	return sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

/*
 * spriteEnd
 *
 * needs to be called once after drawing all sprites in a given frame
 */
HRESULT SpriteManager::spriteEnd() 
{
	return sprite->End();
}

/*
 * drawSprite
 *
 * draws a sprite, mapping the given texture to it
 *
 * param - texture - Texture struct of texture to be drawn
 *         sourceRect - specifies which area of the texture will be mapped to the sprite
 *         x - x coordinate of the screen where the sprite will be drawn
 *         y - y coordinate of the screen where the sprite will be drawn
 */
HRESULT SpriteManager::drawSprite(Texture *texture, RECT *sourceRect, float x, float y, int alpha) 
{
	int alphaVal = alpha;
	if(alpha == -1) {
		alphaVal = texture->alpha;
	}
	D3DXVECTOR3 position(x + offsetX, y + offsetY, 0.0);
	D3DXVECTOR3 center(0.0, 0.0, 0.0);
	
	HRESULT r = sprite->Draw(texture->texture,
		   		    	sourceRect,
						&center,
						&position,
						D3DCOLOR_ARGB(alphaVal, texture->red, texture->green, texture->blue));

	if(FAILED(r))
	{
		GameError("failed drawing sprite...");
		return r;
	}
	return r;
}


/*
 * drawSprite
 *
 * overloaded drawSprite that will take a transformation matrix
 *
 * params - (see above)
 *           &matrix -> a matrix specifying the sprite's transform
 *           &centerPos -> POINT describing the center position of the sprite.  this is required
 *                         for proper rotation.
 *
 */
HRESULT SpriteManager::drawSprite(Texture *texture, RECT *sourceRect, POINT pos, D3DXMATRIX *matrix, POINT centerPos, int alpha)
{
	int alphaVal = alpha;
	if(alpha == -1) {
		alphaVal = texture->alpha;
	}

	HRESULT r;
	D3DXVECTOR3 position((float)pos.x + offsetX, (float)pos.y + offsetY, 0.0f);
	D3DXVECTOR3 center((float)centerPos.x, (float)centerPos.y, 0.0f);

	// save the original transform
	D3DXMATRIX origTrans;
	r = sprite->GetTransform(&origTrans);
	if(FAILED(r))
	{
		GameError("failed saving original sprite transform");
		return r;
	}

	// set new transform
	r = sprite->SetTransform(matrix);
	if(FAILED(r))
	{
		GameError("failed setting new sprite transform");
		return r;
	}
	
	// draw sprite with new transform
	r = sprite->Draw(texture->texture, sourceRect, &center, &position, D3DCOLOR_ARGB(alphaVal, texture->red, texture->green, texture->blue));

	if(FAILED(r))
	{
		GameError("failed drawing sprite with a new transform");
		return r;
	}

	// restore original transform
	r = sprite->SetTransform(&origTrans);
	if(FAILED(r))
	{
		GameError("failed resetting transform");
	}

	return r;


}

void SpriteManager::setOffsetX(int newOffsetX) {
	offsetX = newOffsetX;
}

void SpriteManager::setOffsetY(int newOffsetY) {
	offsetY = newOffsetY;
}