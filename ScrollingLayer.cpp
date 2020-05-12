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

#include "ScrollingLayer.h"
#include "Collision.h"
#include <fstream>
#include <string.h>

/*
 * constructor
 *
 * param - filename - name of file containing info for this layer
 */
ScrollingLayer::ScrollingLayer(char *filename) {

	mapFilename = filename;

	startX = 0;
	xOffset = 0;
	tileMap = 0;
	tiles = 0;
	doneScrolling = false;
	loadMap(mapFilename);
}

/*
 * destructor
 */
ScrollingLayer::~ScrollingLayer() {
	if(tileMap) {
        GameError("Deleting tilemap...");
		
		for(int x = 0; x < worldWidth; ++x) {
			delete[] tileMap[x];
			tileMap[x] = 0;
		}

		delete[] tileMap;
		tileMap = 0;
	}

	if(tiles) {
		for(int i = 0; i < numTiles; ++i) {
			tiles[i].tileTexture.texture->Release();
			tiles[i].tileTexture.texture = 0;
			delete[] tiles[i].tileTexture.filename;
		}

		delete[] tiles;
	}
}

/*
 * initTile
 *
 * initializes a tile, loading all metadata associated with it (bounding poly, animation info, etc)
 *
 * arg - tile - pointer to tile structure where all data is to be loaded
 * arg - filename - name of file containg metadata for 'tile'
 */
void ScrollingLayer::initTile(Tile *tile, char *filename) {
	std::ifstream tileInput(filename);

	//first, get the texture info
	char buffer[256];
	tileInput >> buffer;
	tile->tileTexture.filename = new char[strlen(buffer) + 1];
	strcpy(tile->tileTexture.filename, buffer);
	tile->tileTexture.texture = 0;
	
	//then animation info
	tileInput >> tile->secsPerFrame;
	tile->secsLeft = tile->secsPerFrame;

	//then polygon armature info
	while(tileInput.peek() != -1) {
		POINT p;
		tileInput >> p.x;
		tileInput >> p.y;
		tile->boundingVertices.push_back(p);
	}
	
	//set up an initial source rect
	tile->sourceRect.left = tile->sourceRect.top = 0;
	tile->sourceRect.right = tile->sourceRect.bottom = tileSize;

	//close the input stream
	tileInput.close();
}

/*
 * loadMap
 *
 * map file should be an ascii file with the following format
 *
 * texture_filename tile_size
 * texture_width texture_height
 * scroll_speed infinite_scrolling fade (the last 2 on this line are binary flags)
 * world_width world_height
 * layout of tiles in the map...........
 * ...
 *
 * param - filename - name of the file that contains the map for this layer
 *
 * returns true if it loaded successfully, false otherwise
 */
bool ScrollingLayer::loadMap(char *filename) 
{
	bool success = true;

	if(tileMap) {
		delete tileMap;
		tileMap = 0;
	}

	std::ifstream mapInput(filename);

	//find out the width and height of each tile in pixels
	mapInput >> tileSize;

	//load all tile metadata
	mapInput >> numTiles;
	tiles = new Tile[numTiles];

	for(int i = 0; i < numTiles; ++i) {
		char tileFilename[250];
		mapInput >> tileFilename;
		initTile(&tiles[i], tileFilename);
	}
	
	//get the speed, whether this should scroll infinitely, and if it should fade in and out
	mapInput >> scrollSpeed;
	mapInput >> infiniteScroll;
	mapInput >> fadeSpeed;
	mapInput >> scaleFactor;

	//get dims of the world
	mapInput >> worldWidth;
	mapInput >> worldHeight;

	//create a 2d array the size of the world...
	tileMap = new int*[worldWidth];
	for ( int i = 0; i < worldWidth; ++i ) {
		tileMap[i] = new int[worldHeight];
	}

	//...then get the layout of the world
	for ( int y = 0; y < worldHeight; ++y ) {
		for ( int x = 0; x < worldWidth; ++x ) {
			if ( mapInput.peek() != -1 ) {
				mapInput >> tileMap[x][y];
			} 
			else 
			{
				GameError( "Error reading in map file: " );
				success = false;
			}
		}
	}

	//close the ifstream
	mapInput.close();

	//calculate the screen height in tiles (which is, of course, based on tile size)
	screenWidth = WINDOW_WIDTH / (int)(tileSize * scaleFactor) + 2;
	screenHeight = WINDOW_HEIGHT / (int)(tileSize * scaleFactor) + 1;

	//set up the scaling matrix
	D3DXMatrixScaling(&scaleMatrix, scaleFactor, scaleFactor, 1.0f);

	return success;
}

/*
 * getTileMap
 *
 * returns the ascii representation of this layer (each tile represented by an int)
 */
int **ScrollingLayer::getTileMap() {
	return tileMap;
}

/*
 * getXOffset
 *
 * returns how many pixels the leftmost tile should be drawn offscreen. this allows for smooth scrolling
 */
int ScrollingLayer::getXOffset() {
	return xOffset;
}

/*
 * getStartX
 *
 * returns the x coordinate of the spot in the map where the screen begins
 */
int ScrollingLayer::getStartX() {
	return startX;
}

/*
 * getEndX
 *
 * returns the x coordinate of the rightmost part of the screen to be drawn
 */
int ScrollingLayer::getEndX() {
	int endX = startX + screenWidth;

	if(infiniteScroll) {
		endX = endX % screenWidth;
	}

	return endX;
}

/*
 * scrollWorld
 *
 * updates world->screen coordinates, also does fade effect where applicable
 */
void ScrollingLayer::scrollWorld() {
	if(infiniteScroll) {
		
		// smooth scrolling implemented here.
		
		// offset in pixels
		xOffset += scrollSpeed;
		
		// if beyond one tile, change map lookup
		if(xOffset >= tileSize) {
			xOffset = xOffset%tileSize;
			++startX;
		}
	}
	else 
	{
		//if not scrolling infinitely and not at end of the world
		if(getEndX() !=  worldWidth) {
			xOffset += scrollSpeed;
			if(xOffset >= tileSize) {
				xOffset = xOffset % tileSize;
				++startX;
			}
		}else {
			doneScrolling = true;
		}
	}

}

/*
 * fadeLayer
 *
 * does a fade in/out effect on the layer
 */
void ScrollingLayer::fadeLayer() {
	for(int i = 0; i < numTiles; ++i) {
        if(fadingIn) {
			tiles[i].tileTexture.red = tiles[i].tileTexture.green = tiles[i].tileTexture.blue += fadeSpeed;
		}else {
			tiles[i].tileTexture.red = tiles[i].tileTexture.green = tiles[i].tileTexture.blue -= fadeSpeed;
		}

		if(tiles[i].tileTexture.red <= 50) {
			fadingIn = true;
			tiles[i].tileTexture.red = tiles[i].tileTexture.green = tiles[i].tileTexture.blue = 50;
		}else if(tiles[i].tileTexture.red >= 255) {
			fadingIn = false;
			tiles[i].tileTexture.red = tiles[i].tileTexture.green = tiles[i].tileTexture.blue = 255;
		}
	}
}

/*
 * getWorldWidth
 *
 * returns the width of this entire layer
 */
int ScrollingLayer::getWorldWidth() {
	return worldWidth;
}

/*
 * isScrollingInfinite
 *
 * returns true if this layer should repeat continuously, false otherwise
 */
bool ScrollingLayer::isScrollingInfinite() {
	return infiniteScroll;
}

/*
 * getScreenWidth
 *
 * returns width of the screen in tiles. i think this is made obsolete by a function of the same name in
 * LevelManager
 */
int ScrollingLayer::getScreenWidth() {
	return screenWidth;
}

/*
 * getScreenHeight
 *
 * returns height of the screen in tiles. i think this is made obsolete by a function of the same name in
 * LevelManager
 */ 
int ScrollingLayer::getScreenHeight() {
	return screenHeight;
}

/*
 * getTileSize
 *
 * returns the size of a side of an individual tile in this layer (in pixels)
 */
int ScrollingLayer::getTileSize() 
{
	return tileSize;
}

HRESULT ScrollingLayer::loadTextures() 
{
	HRESULT r;

	for(int i = 0; i < numTiles; ++i) {
		r = SpriteManager::Instance()->loadTexture(&tiles[i].tileTexture);

		if(FAILED(r)) {
			GameError("Error loading tile texture");
			return r;
		}
		//really bad hack that i might fix one day
		if(scaleFactor == 0.5) {
			tiles[i].tileTexture.red = tiles[i].tileTexture.green = tiles[i].tileTexture.blue = 125;
		}
	}

	return r;
}

HRESULT ScrollingLayer::draw(int alpha) {
	HRESULT r = S_OK;
	for(int x = 0; x < getScreenWidth(); ++x) 
	{
		for(int y = 0; y < getScreenHeight(); ++y) 
		{
			//helps infinitely scrolling screens wrap properly
			int xCoord = (x + getStartX()) % getWorldWidth();
			int tileId = tileMap[xCoord][y];
			
			if(tileId != -1) 
			{
				POINT pos, center;
				pos.x = x * getTileSize() - getXOffset();
				pos.y = y * getTileSize();
				center.x = 0;
				center.y = 0;
				r = SpriteManager::Instance()->drawSprite(&tiles[tileId].tileTexture,
														  &tiles[tileId].sourceRect,
														  pos,
														  &scaleMatrix,
														  center, alpha);

				if(FAILED(r)) {
					return r;
				}
			}
		}
	}

	return r;
}

/*
 * checkBoundingBoxes
 *
 * checks to see if the ship is colliding with anything in this layer
 *
 * arg - ship - ship to check collision for
 */
void ScrollingLayer::checkBoundingBoxes(Ship *ship) 
{
	//ship's world coordinates in pixels
	int shipWorldX = ship->getPosX() + (getStartX() * getTileSize()) + getXOffset();
	int shipWorldY = ship->getPosY();

	//dont check collision for dead ships or ships that are offscreen
	if(ship->isDead() || shipWorldY > WINDOW_WIDTH || (shipWorldY + ship->getSpriteHeight()) <= 0) {
		return;
	}
	
	int x, y; //world coordinates of a ship's given corner in tiles
	RECT boundingBox;
	ship->getBounds(&boundingBox);

	//check all 4 corners of the ship's bounding box to see if it may be colliding with a
	//tile

	//check what tile the top left corner is in
	x = (boundingBox.left / tileSize) + getStartX();
	y = boundingBox.top / tileSize;

	if(tileSize - (boundingBox.left % tileSize) < getXOffset()) {
		++x;
	}

	//if the bounding boxes collide, go one step further and compare poly armatures
	if(tileMap[x][y] != -1) 
	{
		//if there's a collision...
		if(checkPolyArmature(ship->getBoundingPoints(), shipWorldX, shipWorldY,
		   &tiles[tileMap[x][y]].boundingVertices, x*tileSize, y*tileSize)) 
		{
				//hide the ship
				ship->damage(20000.0);
				return;
		}
	}

	//check bottom left corner
	y = boundingBox.bottom / tileSize;

	//if the bounding boxes collide, go one step further and compare poly armatures
	if(y < screenHeight && tileMap[x][y] != -1) {
		//if there's a collision...
		if(checkPolyArmature(ship->getBoundingPoints(), shipWorldX, shipWorldY,
		   &tiles[tileMap[x][y]].boundingVertices, x*tileSize, y*tileSize)) {
				//hide the ship
				ship->damage(20000.0);
				return;
		   }
	}

	//check top right corner
	x = (boundingBox.right / tileSize) + getStartX();
	y = boundingBox.top / tileSize;

	if(tileSize - (boundingBox.right % tileSize) < getXOffset()) {
		++x;
	}

	//if the bounding boxes collide, go one step further and compare poly armatures
	if(tileMap[x][y] != -1) {
		//if there's a collision...
		if(checkPolyArmature(ship->getBoundingPoints(), shipWorldX, shipWorldY,
		   &tiles[tileMap[x][y]].boundingVertices, x*tileSize, y*tileSize)) {
				//hide the ship
				ship->damage(20000.0);
				return;
		   }
	}

	//check bottom right corner
	y = boundingBox.bottom / tileSize;

	//if the bounding boxes collide, go one step further and compare poly armatures
	if(y < screenHeight && tileMap[x][y] != -1) {
		//if there's a collision...
		if(checkPolyArmature(ship->getBoundingPoints(), shipWorldX, shipWorldY,
		   &tiles[tileMap[x][y]].boundingVertices, x*tileSize, y*tileSize)) {
				//hide the ship
				ship->damage(20000.0);
				return;
		   }
	}
}

/*
 * checkBoundingBoxes
 *
 * checks to see if the given rect is overlapping with the bounding box of a collideable tile
 *
 * arg - boundingBox - bounding box of object to check against this layer
 * arg - points - polygon armature of object to check against this layer
 * arg - x - x coordinate of the top left corner of 'boundingBox'
 * arg - y - y coordinate of the top left corner of 'boundingBox'
 *
 * returns true if 'boundingBox' overlaps with a collideable tile's bounding box, false otherwise
 */
bool ScrollingLayer::checkBoundingBoxes(RECT *boundingBox, std::vector<POINT> *points, int boxX, int boxY) {
	//disregard anything not on the screen
	if(boxY <= 0 - tileSize || boxY > WINDOW_HEIGHT) {
		return false;
	}

	int boxWorldX = boxX + (getStartX() * getTileSize()) + getXOffset();
	RECT boxToCheck;
	boxToCheck.left = boundingBox->left + boxWorldX;
	boxToCheck.right = boundingBox->right + boxWorldX;
	boxToCheck.top = boundingBox->top + boxY;
	boxToCheck.bottom = boundingBox->bottom + boxY;

	int cornerX, cornerY; //world coordinates of a ship's given corner in tiles
	//check all 4 corners of the ship's bounding box to see if it may be colliding with a
	//tile

	//check what tile the top left corner is in
	cornerX = ((boundingBox->left + boxX) / tileSize) + getStartX();
	cornerY = (boundingBox->top + boxY) / tileSize;

	if(tileSize - ((boundingBox->left + boxX) % tileSize) < getXOffset()) {
		++cornerX;
	}

	//if the bounding boxes collide, go one step further and compare poly armatures
	if(tileMap[cornerX][cornerY] != -1) {
		RECT tileBoundingBox, dummyBox;
		tileBoundingBox.left = cornerX * tileSize;
		tileBoundingBox.right = tileBoundingBox.left + tileSize;
		tileBoundingBox.top = cornerY * tileSize;
		tileBoundingBox.bottom = tileBoundingBox.top + tileSize;

		if(IntersectRect(&dummyBox, &tileBoundingBox, &boxToCheck) &&
		   checkPolyArmature(points, boxWorldX, boxY, &tiles[tileMap[cornerX][cornerY]].boundingVertices,
							 cornerX * tileSize, cornerY*tileSize)) {
			return true;
		}
	}

	//check bottom left corner
	cornerY = (boundingBox->bottom + boxY) / tileSize;

	//if the bounding boxes collide, go one step further and compare poly armatures
	if(tileMap[cornerX][cornerY] != -1) {
		RECT tileBoundingBox, dummyBox;
		tileBoundingBox.left = cornerX * tileSize;
		tileBoundingBox.right = tileBoundingBox.left + tileSize;
		tileBoundingBox.top = cornerY * tileSize;
		tileBoundingBox.bottom = tileBoundingBox.top + tileSize;

		if(IntersectRect(&dummyBox, &tileBoundingBox, &boxToCheck) &&
		   checkPolyArmature(points, boxWorldX, boxY, &tiles[tileMap[cornerX][cornerY]].boundingVertices,
							 cornerX * tileSize, cornerY*tileSize)) {
			return true;
		}
	}

	//check top right corner
	cornerX = ((boundingBox->right + boxX) / tileSize) + getStartX();
	cornerY = (boundingBox->top + boxY) / tileSize;

	if(tileSize - ((boundingBox->right + boxX) % tileSize) < getXOffset()) {
		++cornerX;
	}

	//if the bounding boxes collide, go one step further and compare poly armatures
	if(tileMap[cornerX][cornerY] != -1) {
		RECT tileBoundingBox, dummyBox;
		tileBoundingBox.left = cornerX * tileSize;
		tileBoundingBox.right = tileBoundingBox.left + tileSize;
		tileBoundingBox.top = cornerY * tileSize;
		tileBoundingBox.bottom = tileBoundingBox.top + tileSize;

		if(IntersectRect(&dummyBox, &tileBoundingBox, &boxToCheck) &&
		   checkPolyArmature(points, boxWorldX, boxY, &tiles[tileMap[cornerX][cornerY]].boundingVertices,
							 cornerX * tileSize, cornerY*tileSize)) {
			return true;
		}
	}

	//check bottom right corner
	cornerY = (boundingBox->bottom + boxY) / tileSize;

	//if the bounding boxes collide, go one step further and compare poly armatures
	if(tileMap[cornerX][cornerY] != -1) {
		RECT tileBoundingBox, dummyBox;
		tileBoundingBox.left = cornerX * tileSize;
		tileBoundingBox.right = tileBoundingBox.left + tileSize;
		tileBoundingBox.top = cornerY * tileSize;
		tileBoundingBox.bottom = tileBoundingBox.top + tileSize;

		if(IntersectRect(&dummyBox, &tileBoundingBox, &boxToCheck) &&
		   checkPolyArmature(points, boxWorldX, boxY, &tiles[tileMap[cornerX][cornerY]].boundingVertices,
							 cornerX * tileSize, cornerY*tileSize)) {
			return true;
		}
	}

	return false;
}


/*
 * update
 *
 * updates this layer (animates any animated tiles...)
 *
 * arg - dt - how much time has passed since the last call to update()
 */
void ScrollingLayer::update(float dt) {
	for(int i = 0; i < numTiles; ++i) {
		if(tiles[i].secsPerFrame != -1.0) {
			if((tiles[i].secsLeft -= dt) <= 0) {
				tiles[i].sourceRect.left = (tiles[i].sourceRect.left + tileSize) % 
											tiles[i].tileTexture.width;
				tiles[i].sourceRect.right = tiles[i].sourceRect.left + tileSize;
				tiles[i].secsLeft = tiles[i].secsPerFrame - tiles[i].secsLeft;
			}
		}
	}
}

/*
 * speedUp
 *
 * speeds up the scrolling by the specified amount
 *
 * arg - speed - how much to speed up this layer by
 */
void ScrollingLayer::speedUp(int speed) {
	scrollSpeed += speed;
}

/*
 * getScrollSpeed
 *
 * get the speed this layer is currently scrolling at
 *
 * returns the speed this layer is currently scrolling at (in pixels)
 */
int ScrollingLayer::getScrollSpeed() {
	return scrollSpeed;
}

/*
 * isDoneScrolling
 *
 * is this layer finished scrolling? (end of layer reached and not an infinitely scrolling layer)
 *
 * returns true of this layer is done scrolling, false otherwise
 */
bool ScrollingLayer::isDoneScrolling() {
	return doneScrolling;
}


/*
 * getEndScreenWorldX
 *
 * find out the world x coordinate of the rightmost edge of the screen
 *
 * returns the world x coordinate of the rightmost edge of the screen
 */
int ScrollingLayer::getEndScreenWorldX() {
	return (startX * tileSize) + xOffset + WINDOW_WIDTH;
}