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

#ifndef SCROLLING_SCREEN_H
#define SCROLLING_SCREEN_H

#include "GameCore.h"
#include "Texture.h"
#include "SpriteManager.h"
#include "Player.h"
#include "Tile.h"
#include "FXManager.h"

class ScrollingLayer {
public:
	//ctor/dtor*
	ScrollingLayer(char *filename);
	~ScrollingLayer();

	int **getTileMap();
	int getXOffset();
	int getStartX();
	int getEndX();
	int getEndScreenWorldX();
	void scrollWorld();
	int getWorldWidth();
	int getScreenWidth();
	int getScreenHeight();
	int getScrollSpeed();
	int getTileSize();
	bool isScrollingInfinite();
	bool isFading();
	bool isDoneScrolling();
	void fadeLayer();
	void update(float dt);
	HRESULT loadTextures();
	HRESULT draw(int alpha = -1);
	void checkCollision(Ship *ship);
	void checkBoundingBoxes(Ship *ship);
	bool checkBoundingBoxes(RECT *boundingBox, std::vector<POINT> *points, int boxX, int boxY);
	void speedUp(int speed);
	
private:
	//helper functions
	void initTile(Tile *tile, char *filename);
	bool loadMap(char *filename);

	char *mapFilename;
	int tileSize;
	int numTiles;
	Tile *tiles;
	int startX;
	int xOffset;
	int **tileMap;
	int worldWidth, worldHeight;   //dims of the world  (in tiles)
	int screenWidth, screenHeight; //dims of the screen (in tiles)
	int scrollSpeed;   //how fast the screen will scroll (in pixels/frame)
	bool infiniteScroll;     //the part that goes off the left side of the screen will be drawn to the right side
	bool fadingIn;      //is the layer currently fading in?
	int fadeSpeed;      //how fast this layer will fade in/out (0 for no fade)
	float scaleFactor;
	bool doneScrolling;
	D3DXMATRIX scaleMatrix;
};
#endif