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

#include "DIInput.h"
#pragma comment(lib, "dinput8.lib")

DIInput::DIInput()
{
	m_pDI = 0;
}

DIInput::~DIInput()
{
	if (m_pDI)
	{
		m_pDI->Release();
		m_pDI = 0;
	}
}

HRESULT DIInput::SetupInput(HINSTANCE &hInstance)
{
	
	HRESULT r;
	//make new DI object
	r = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&m_pDI, NULL);
	if ( FAILED( r ) )
	{
		GameError("Failed on DI Object Creation");
		return r;
	}
	
	return S_OK;
}

HRESULT DIInput::SetupKeyboard(HWND &hWnd)
{
	HRESULT r;
	r = SetupDIKeyboard(m_pDI,hWnd);
	if ( FAILED( r ) )
	{
		GameError("Failed on Keyboard Setup");
		return r;
	}
	
	return S_OK;
}

HRESULT DIInput::SetupMouse(HWND &hWnd)
{
	HRESULT r;
	r = SetupDIMouse(m_pDI,hWnd);
	if ( FAILED( r ) )
	{
		GameError("Failed on Mouse Setup");
		return r;
	}
	
	return S_OK;
}