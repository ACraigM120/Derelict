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

#ifndef DIKeyboard_H
#define DIKeyboard_H
#pragma once

class DIKeyboard
{
public:
	BOOL IsKeyDown( int );
	bool wasKeyDown(int);
	bool keyJustPressed(int);
	void refreshOldBuffer();
	DIKeyboard();
	~DIKeyboard();

protected:
	HRESULT SetupDIKeyboard(LPDIRECTINPUT8 &m_pDI, HWND &hWnd);
private:
	//Direct Input Constructs
	
	LPDIRECTINPUTDEVICE8 m_pKeyDev;
	char m_KeyBuffer[256];
	char tempBuffer[256];
	char oldKeyBuffer[256];
	bool checked;  //have we checked for any key presses since the last call to refresh()?
};

#endif