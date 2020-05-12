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

#include "FontEngine.h"

// This engine was lifted from the Zen book. I intend to play with it a bit later.
// Since it was lifted from the Zen book (except comments), I'm expecting it will work.

FontEngine::FontEngine(char *fontPathname, int charWidth, int charHeight) {
	fontTexture.filename = fontPathname;
	fontTexture.texture = 0;  //zero the texture pointer

	//size of each char in pixels
	alphabetLetterWidth = charWidth;
	alphabetLetterHeight = charHeight;
}

FontEngine::~FontEngine() {
	if(fontTexture.texture) {
		fontTexture.texture->Release();
		fontTexture.texture = 0;
	}
}

// Load the alphabet

HRESULT FontEngine::LoadAlphabet() {
	HRESULT r = SpriteManager::Instance()->loadTexture(&fontTexture);
	
	if(FAILED(r)) {
		GameError("Error loading font texture");
		return r;
	}

	alphabetLettersPerRow = fontTexture.width / alphabetLetterWidth;

	return r;
}

// Prints a character to a surface.

HRESULT FontEngine::PrintChar(int x, int y, char Character, int alpha) {
	HRESULT r = S_OK;
	div_t Result;
	int OffsetX = 0;
	int OffsetY = 0;
	RECT LetterRect = { 0, 0, 0, 0 };
	
	// find the character in the font
	Character -= 32;
	Result = div( Character, alphabetLettersPerRow );
	OffsetX = Result.rem * alphabetLetterWidth;
	OffsetY = Result.quot * alphabetLetterHeight;

	// draw the stupid thing
	SetRect( &LetterRect, OffsetX, OffsetY, OffsetX + alphabetLetterWidth, 
		OffsetY + alphabetLetterHeight );
	
	r = SpriteManager::Instance()->drawSprite(&fontTexture, &LetterRect, (float)x, (float)y, alpha);

	//return with error if we couldnt draw it
	if( FAILED( r ) ) {
		GameError( "Couldn't draw from font texture" );
		return r;
	}

	return r;
}

// prints a String from left to right
HRESULT FontEngine::PrintString( int x, int y, char* String, int alpha) {
	HRESULT r = S_OK;
	for( UINT i = 0; i < strlen( String ); i++ ) {
		r = PrintChar( x + (alphabetLetterWidth * i), y, String[i], alpha);

		if(FAILED(r)) {
			return r;
		}
	}

	return r;
}

// prints a String from right to left (useful for things like the score)
HRESULT FontEngine::PrintRevString( int x, int y, char* String, int alpha) {
	HRESULT r = S_OK;
	for( UINT i = 0; i < strlen( String ); i++ ) {
		r = PrintChar( x - (alphabetLetterWidth * i), y, String[strlen(String) - i - 1], alpha);

		if(FAILED(r)) {
			return r;
		}
	}

	return r;
}

