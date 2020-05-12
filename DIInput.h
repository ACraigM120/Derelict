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

#ifndef DIInput_H
#define DIInput_H

#include <dinput.h>
#include "DIKeyboard.h"
#include "DIMouse.h"
#include "GameCore.h"

class DIInput : public DIKeyboard, public DIMouse
{
public:
	HRESULT SetupInput(HINSTANCE &hInstance);
	HRESULT SetupKeyboard(HWND &hWnd);
	HRESULT SetupMouse(HWND &hWnd);

	DIInput();
	~DIInput();
private:
	LPDIRECTINPUT8 m_pDI;
};

#endif