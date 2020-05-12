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

#include <dinput.h>

#include "DIKeyboard.h"
#include "GameCore.h"

DIKeyboard::DIKeyboard()
{
	//constructor
	for (int i=0; i<256; i++)
	{
		m_KeyBuffer[i] = 0;
		oldKeyBuffer[i] = 0;
		tempBuffer[i] = 0;
	}
	m_pKeyDev = 0;
	checked = false;
}

DIKeyboard::~DIKeyboard() 
{

	if (m_pKeyDev)
	{
		m_pKeyDev->Unacquire();
		m_pKeyDev->Release();
		m_pKeyDev = 0;
	}
}

HRESULT DIKeyboard::SetupDIKeyboard(LPDIRECTINPUT8 &m_pDI, HWND &hWnd) {
	
	HRESULT r;
	
	//make keyboard device
	r = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyDev, NULL);
	if ( FAILED( r ) )
	{
		GameError("crashed on key device creation");
		return r;
	}

	//Set Data Format
	r = m_pKeyDev->SetDataFormat( &c_dfDIKeyboard );
	if ( FAILED( r ) )
	{
		GameError("crashed on set data format");
		return r;
	}

	//Set Coop Level
	m_pKeyDev->SetCooperativeLevel( hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if ( FAILED( r ) )
	{
		GameError("crashed on set coop level");
		return r;
	}

	//Acquire
	r = m_pKeyDev->Acquire();
	if ( FAILED( r ) )
	{
		GameError("crashed on acquire keyboard");
		return r;
	} 

	GameError("INITIALIZED KEYBOARD SUPPORT");
	return S_OK;
}


BOOL DIKeyboard::IsKeyDown(int key)
{
	HRESULT r = 0;
	//Get the state of the keyboard in the buffer
	r = m_pKeyDev->GetDeviceState( sizeof( m_KeyBuffer ), &m_KeyBuffer );
	if ( FAILED( r ) )
	{
		if(r == DIERR_INPUTLOST)
		{
			//try to focus window...
			//HWND z = SetFocus(mainWindow.getHwnd());
			//MessageBox(z,"THIS GOT FOCUS ??", "error", MB_ICONERROR);
			
			while(r == DIERR_INPUTLOST)
			{
				//r = m_pKeyDev->Unacquire();
				r = m_pKeyDev->Acquire();

				if(SUCCEEDED( r ) )
				{
					m_pKeyDev->GetDeviceState( sizeof( m_KeyBuffer ), &m_KeyBuffer );
					//MessageBox(mainWindow.getHwnd(),"GOT IT BACK", "error", MB_ICONERROR);
					//break; 
				}
				else
					//MessageBox(mainWindow.getHwnd(),"LOST INPUT", "error", MB_ICONERROR);
					return FALSE;
			}
		}
		
		else if (r == DIERR_NOTACQUIRED)
		{
			if (GetWindowHasFocus())
			{
				r = m_pKeyDev->Acquire();
				if (SUCCEEDED( r ) )
				{
					m_pKeyDev->GetDeviceState( sizeof( m_KeyBuffer ), &m_KeyBuffer );
					//MessageBox(mainWindow.getHwnd(),"GOT IT BACK 2", "error", MB_ICONERROR);
				}
			}
			else
				return FALSE;
		}
		else
			//some other keyboard error..otherwise return false
			//MessageBox(mainWindow.getHwnd(),DXGetErrorString8(r), "error", MB_ICONERROR);
			return FALSE;
	}

	checked = true;
	bool retval = false;

	//check for the KEY !!
	if( m_KeyBuffer[key] & 0x80 ) {
		retval = true;
		tempBuffer[key] = -128;
	}

	return retval;
}

bool DIKeyboard::wasKeyDown(int key) {
	bool retval = false;

	if( oldKeyBuffer[key] & 0x80 ) {
		retval = true;
	}

	return retval;
}

bool DIKeyboard::keyJustPressed(int key) {
	return IsKeyDown(key) && !wasKeyDown(key);
}

void DIKeyboard::refreshOldBuffer() {
	if(checked) {
		memcpy(oldKeyBuffer, tempBuffer, 256);
		memset(tempBuffer, 0, 256);
		checked = false;
	}
}