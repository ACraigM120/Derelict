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
#include <windows.h>
#include "DIMouse.h"
#include "GameCore.h"

DIMouse::DIMouse()
{
	//constructor
	m_pMouseDev = 0;
}

DIMouse::~DIMouse()
{
	if (m_pMouseDev)
	{
		m_pMouseDev->Unacquire();
		m_pMouseDev->Release();
		m_pMouseDev = 0;
	}
}

HRESULT DIMouse::SetupDIMouse(LPDIRECTINPUT8 &m_pDI, HWND &hWnd)
{
	HRESULT r;
	//------------------------[ Blatant attempt at MOUSE land ]-----------
	r = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouseDev, NULL);
	if ( FAILED( r ) )
	{
		return r;
	} 

	r = m_pMouseDev->SetDataFormat( &c_dfDIMouse );
	if (FAILED( r ) )
	{
		return r;
	}

	//Set Coop Level
	r = m_pMouseDev->SetCooperativeLevel( hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if ( FAILED( r ) )
	{
		return r;
	}
	
	r = m_pMouseDev->Acquire();
	if (FAILED( r ) )
	{
		return r;
	}

	return S_OK;

}

HRESULT DIMouse::Poll()
{
	HRESULT r = 0;

	r = m_pMouseDev->GetDeviceState( sizeof( DIMOUSESTATE ), &m_MouseData );
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
				r = m_pMouseDev->Acquire();

				if(SUCCEEDED( r ) )
				{
					m_pMouseDev->GetDeviceState( sizeof( DIMOUSESTATE ), &m_MouseData );
					//MessageBox(mainWindow.getHwnd(),"GOT IT BACK", "error", MB_ICONERROR);
					//break;
					return S_OK;
				}
				else
					//MessageBox(mainWindow.getHwnd(),"LOST INPUT", "error", MB_ICONERROR);
					return E_FAIL;
			}
		}
		else if (r == DIERR_NOTACQUIRED)
		{
			if (GetWindowHasFocus())
			{
				r = m_pMouseDev->Acquire();
				if (SUCCEEDED( r ) )
				{
					m_pMouseDev->GetDeviceState( sizeof( DIMOUSESTATE ), &m_MouseData );
					return S_OK;
					//MessageBox(mainWindow.getHwnd(),"GOT IT BACK 2", "error", MB_ICONERROR);
				}
			}
		}
		else
			//some other keyboard error..otherwise return false
			//MessageBox(mainWindow.getHwnd(),DXGetErrorString8(r), "error", MB_ICONERROR);
			return E_FAIL;
	}
	return S_OK;
}

void DIMouse::ParseMouseData()
{
	int x = GetMouseLoc().x + m_MouseData.lX;
	int y = GetMouseLoc().y + m_MouseData.lY;

	POINT pt = {x, y};
	SetMouseLoc(pt);

}