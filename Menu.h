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

// Header file for Menu.cpp

#ifndef MENU_H
#define MENU_H

#include "SpriteManager.h"
#include "Texture.h"
#include "FontEngine.h"


/* This class takes care of menus, as the name implies.
 * It is the responsibility of the owner of the menu object to give the options meaning.
 */

class Menu {
public:
	Menu( int rows, int cols, int minX, int minY, int spacingX, int spacingY, char** optionNames, char* background );
	~Menu();
	HRESULT init();
	void resetPos();
	void move( int direction );
	char* getSelection();
	int getRow();
	int getCol();
	void draw(int alpha = -1);
	void setPos( int x, int y );

private:
	FontEngine fontEngine;
	D3DXMATRIX scaleMatrix;
	Texture icon;
	RECT iconRect;
	Texture bg;
	RECT bgRect;
	int row;
	int col;

	int numRows;
	int numCols;
	int leftmostX;
	int topmostY;
	char** options;
	int deltaX;
	int deltaY;
};
#endif