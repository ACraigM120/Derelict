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

#include "Menu.h"

//Constructor
Menu::Menu( int rows, int cols, int minX, int minY, int spacingX, int spacingY, char** optionNames, char* background ):
fontEngine( "graphics/font.dds", 16, 32 ) 
{
	numRows = rows;
	numCols = cols;
	leftmostX = minX;
	topmostY = minY;
	options = optionNames;
	deltaX = spacingX;
	deltaY = spacingY;
	
	bg.filename = background;
	bg.texture = 0;
	bgRect.bottom = 600;
	bgRect.top = 0;
	bgRect.right = 800;
	bgRect.left = 0;

	icon.filename = "graphics/PlayerShip.dds";
	icon.texture = 0;
	iconRect.bottom = 64;
	iconRect.top = 0;
	iconRect.right = 64;
	iconRect.left = 0;
	D3DXMatrixScaling( &scaleMatrix, 0.5f, 0.5f, 1.0f );

	row = 0;
	col = 0;
}

// destructor
Menu::~Menu() {
	if( bg.texture ) {
		bg.texture->Release();
		bg.texture = 0;
	}
	if( icon.texture ) {
		icon.texture->Release();
		icon.texture = 0;
	}
}

// contains things we don't want to do in the constructor since they might go boom
HRESULT Menu::init() {

	// load the graphics

	HRESULT r = fontEngine.LoadAlphabet();
	if(FAILED(r)) {
		GameError("Couldn't load font");
		return r;
	}

	r = SpriteManager::Instance()->loadTexture( &bg );
	if( FAILED(r)) {
		GameError("failed loading background image");
	}

	r = SpriteManager::Instance()->loadTexture( &icon );
	if(FAILED(r))
	{
		GameError("failed loading icon texture");
		return r;
	}

	return r;
}

void Menu::resetPos() {
	row = 0;
	col = 0;
}

// moves the "cursor" either up, down, left, or right
// 0 = up
// 1 = right
// 2 = down
// 3 = left
void Menu::move( int direction ) {
	switch( direction ) {
		case( 0 ):
			do {
				row = (row + numRows - 1) % numRows;
			} while( getSelection() == "" );
			break;
		case( 1 ):
			do {
				col = (col + 1) % numCols;
			} while( getSelection() == "" );
			break;
		case( 2 ):
			do {
				row = (row + 1) % numRows;
			} while( getSelection() == "" );
			break;
		case( 3 ):
			do {
				col = (col + numCols - 1) % numCols;
			} while( getSelection() == "" );
			break;
		default:
			GameError( "Shazam!  Didn't go up down left or right!" );
			break;
	}
}

char* Menu::getSelection() {
	return options[ (row * numCols) + col ];
}
int Menu::getRow() {
	return row;
}

int Menu::getCol() {
	return col;
}

void Menu::draw(int alpha) {
	SpriteManager::Instance()->drawSprite( &bg, &bgRect, 0, 0, alpha );

	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			fontEngine.PrintString( ( leftmostX + ( j * deltaX ) ), ( topmostY + ( i * deltaY ) ),
				options[ ( i * numCols ) + j ], alpha );
		}
	}
	POINT position;
	position.x = (leftmostX * 2) - 48 + (col * 2 * deltaX);
	position.y = (topmostY * 2) + 32 + (row * 2 * deltaY);

	POINT center;
	center.x = 32;
	center.y = 32;

	HRESULT r = SpriteManager::Instance()->drawSprite( &icon, &iconRect, position, &scaleMatrix, center, alpha );
}

void Menu::setPos( int x, int y ) {
	col = x;
	row = y;
}