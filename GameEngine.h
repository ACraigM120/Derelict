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

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameCore.h"
#include "GameWindow.h"
#include "GameDXSetup.h"
#include "Timing.h"
#include "SpriteManager.h"
#include "Ship.h"
#include "Player.h"
#include "DIInput.h"
#include "LevelManager.h"
#include "IntroManager.h"
#include "SoundManager.h"
#include "EnemyManager.h"
#include "PowerUpManager.h"
#include "FontEngine.h"
#include "Emitter.h"
#include "Particle.h"
#include "FXManager.h"
#include "DisplayManager.h"
#include "LookUp.h"
#include "BinManager.h"
#include "Menu.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define EXPLOSION_TIMER_MAX 0.3f
#define CORE_EXPLODING_TIME 10.0f

class GameEngine {
public:
	enum GameState {
		intro = 0,
		menuscreen = 1,
		playing = 2,
		paused = 3,
		gameover = 4,
		gravityBombing = 5,
		title = 6,
		options = 7,
		story = 8,
		initialsScreen = 9,
		scoreScreen = 10,
		coreDying = 11,
		victory = 12,
		instructions = 13
	};
	
	//ctor/dtor
	GameEngine();
	~GameEngine();

	//initialization functions
	HRESULT initD3D();
	HRESULT initTiming();
	HRESULT initWindow(HINSTANCE hInstance, int nShowCmd);
	HRESULT initInput();
	HRESULT initSpriteMgr();
	HRESULT initGameElements();

	//do all calculations required before drawing next frame
	void nextFrame();

private:
	//check player input
	void checkInput();
	void checkCollision();

	//draw the current frame
	void drawFrame();

	// build some game objects
	GameWindow gameWindow;
	GameDX gameDX;
	Timing timer;
	DIInput input;
	LevelManager levelMgr;
	Player player;
	GameState state;
	GameState lastState;

	Menu* titleMenu;
	char** titleMenuOptions;
	Menu* optionMenu;
	char** optionMenuOptions;
	Menu* initialEntry;
	char** initials;

	int startingLives;
	int maxSLives;
	int startingGBombs;
	int maxSGBombs;
	float difficulty;
	int diffMultiplier;

	int storyScroll;

	char* initial1;
	char* initial2;
	char* initial3;
	int position;

	char** topPlayers;
	int* topScores;
	int scoreTimer;

	//singletons
	SpriteManager* spriteMgr;
	SoundManager* soundMgr;
	DisplayManager* display;
	EnemyManager* enemyMgr;

	//so as to not waste memory, we'll 'new' this as soon as the prog begins, and terminate it as soon
	//as the intro finishes
	IntroManager *introMgr;

	float gameOverTimer;
	float coreExplosionTimer;
	float coreDyingTimer;

	bool showBins;

	int alpha;
};

#endif