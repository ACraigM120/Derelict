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

// Header file for DisplayManager.cpp

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "FontEngine.h"
#include "Texture.h"
#include "Player.h"
#include "LevelManager.h"

class DisplayManager {
public:

	DisplayManager( Player* ship, LevelManager* mgr);
	~DisplayManager();
	HRESULT init();
	void display(int alpha = -1);
	void gameOver(int alpha = -1);
	void gameOverMenu();
	void setOffset( int offset );
	void setFlame( bool isOn );
	void printSettings( int lives, int gBombs, float difficulty );
	void printStory( int yOffset );
	void printInitials( char* initial1, char* initial2, char* initial3, int alpha = -1 );
	void printTopPlayers( char** topPlayers, int* topScores, int alpha = -1 );
	void setHighScore( int hScore );
	void printCredits( int yOffset, int alpha = -1 );
	void showInstructions();

private:
	bool flaming;
	int iconOffset;
	int highScore;

	FontEngine fontEngine;
	D3DXMATRIX scaleMatrix;
	Player* player;
	LevelManager* levelMgr;

	Texture icon;
	Texture flame;
	Texture health;
	Texture gBomb;
	Texture scoreBG;
	RECT iconRect;
	RECT flameRect;
	RECT healthRect;
	RECT gBombRect;
	RECT scoreBGRect;
	Texture instructions;
};

#endif