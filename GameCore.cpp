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

#include "GameCore.h"
#include "Timing.h"

static int WINDOW_HAS_FOCUS = false;
static int DRAW_CUSTOM_MOUSE = false;
static int StillRendering = true;

static POINT mouseLoc = {-99,-99};
static int PAUSED = false;
static float dev = 1.0f;

void GameError (char* Error)
{
	OutputDebugString( Error );
	OutputDebugString( "\n" );
}

void GameError (const char* Error)
{
	OutputDebugString( Error );
	OutputDebugString( "\n" );
}

int GetWindowHasFocus()
{
	return WINDOW_HAS_FOCUS;
}

void SetDrawCustomMouse(int x)
{
	DRAW_CUSTOM_MOUSE = x;
}

int GetDrawCustomMouse()
{
	return DRAW_CUSTOM_MOUSE;
}

void SetWindowHasFocus(int x)
{
	WINDOW_HAS_FOCUS = x;
}

POINT GetMouseLoc()
{
	return mouseLoc;
}

void SetMouseLoc(POINT pt)
{
	/*if (pt.x >= 0 && pt.x <= WINDOW_WIDTH &&
		pt.y >= 0 && pt.y <= WINDOW_HEIGHT)
		mouseLoc = pt;
	else
	{
		//mouseLoc.x = -99;
		//mouseLoc.y = -99;
		mouseLoc = pt;
	}*/
	mouseLoc = pt;
}

void SetStillRendering(int x)
{
	StillRendering = x;
}

int GetStillRendering()
{
	return StillRendering;
}

int GetGamePaused()
{
	return PAUSED;
}

void SetGamePaused(int x)
{
	PAUSED = x;
}

float GetDeviance()
{
	return dev;
}

void SetDeviance(float x)
{
	dev = x;
}