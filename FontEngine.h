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

// Header file for FontEngine.cpp

#ifndef FONT_ENGINE_H
#define FONT_ENGINE_H

#include "SpriteManager.h"
#include <stdlib.h>

class FontEngine
{
public:
	FontEngine(char *fontPathname, int charWidth, int charHeight);
	~FontEngine();

	HRESULT LoadAlphabet();
	HRESULT PrintString(int x, int y, char* String, int alpha = -1);
	HRESULT PrintRevString(int x, int y, char* String, int alpha = -1);

private:
	Texture fontTexture;
	HRESULT PrintChar(int x, int y, char Character, int alpha = -1);

	int alphabetLettersPerRow;
	int alphabetLetterWidth;
	int alphabetLetterHeight;
};
#endif