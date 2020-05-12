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

//-----[ Globals ]--------------------------------------------

#ifndef GameConstants_H
#define GameConstants_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <d3dx9math.h>
#include <dxerr9.h>


#define degrad 3.1416 / 180.00

#define WINDOW_WIDTH 800 //window surface width in pixels
#define WINDOW_HEIGHT 600 //window surface height in pixels
#define MAX_FRAME_RATE 60.0f //framerate target 
#define FULL_SCREEN_REFRESH_RATE 60 //refresh rate... make sure supported !

#define SHOW_FRAME_COUNT true //display framerate and deviance info on startup
#define LOCK_FRAME_RATE false //lock framerate near refresh rate
#define APP_TITLE "AppTitle" //internal application title
#define WINDOW_TITLE "Derelict The Game" //title bar of the game window

#define BINSIZE 100
#define NUMBINS_W WINDOW_WIDTH/BINSIZE
#define NUMBINS_H WINDOW_HEIGHT/BINSIZE
#define NUMBINS NUMBINS_W*NUMBINS_H

//-----[ More Junk ]------------------------------------------
void GameError (char* );		//definition of error reporting class
void GameError (const char* );	//may be used for other things besides errors...

int GetWindowHasFocus();
void SetWindowHasFocus(int);
int GetDrawCustomMouse();
void SetDrawCustomMouse(int);
POINT GetMouseLoc();
void SetMouseLoc(POINT);
void SetStillRendering(int);
int GetStillRendering();

int GetGamePaused();
void SetGamePaused(int);

float GetDeviance();
void SetDeviance(float);

#endif