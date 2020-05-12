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

#ifndef DIMouse_H
#define DIMouse_H

class DIMouse
{
public:
	
	HRESULT Poll();
	void ParseMouseData();

	DIMouse();
	~DIMouse();
protected:
	HRESULT SetupDIMouse(LPDIRECTINPUT8 &m_pDI, HWND &hWnd);
private:
	LPDIRECTINPUTDEVICE8 m_pMouseDev;
	DIMOUSESTATE m_MouseData;
};

#endif