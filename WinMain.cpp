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
#include "GameEngine.h"

//-----[ WinMain ]--------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, int nShowCmd)
{
	
	GameEngine gameEngine;

	gameEngine.initWindow(hInstance, nShowCmd);
	gameEngine.initD3D();
	gameEngine.initTiming();
	gameEngine.initInput();
	gameEngine.initSpriteMgr();
	gameEngine.initGameElements();

	//enter message loop and sit --------------
	// the actualy message pump is contained in the GameWindow class.... GlobalWndProc
	MSG msg;
	while(TRUE)
	{
		//check and see if there is a message
		if (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE))
		{
			//check and see if this is the quit message
			if (msg.message == WM_QUIT)
				//exit the message loop
				break;
			//else do what we would have before, namely translate and dispatch
			TranslateMessage( &msg );
			DispatchMessage( &msg );

		}
		else
		{
			//this is our game loop, the game engine figures out what the next frame
			//will look like, then draws it
			gameEngine.nextFrame();			

		}
	}
	return (int) msg.wParam;
}