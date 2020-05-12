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
#include "GameWindow.h"

//Global WndProc (Can't be placed inside class :<  )
LRESULT CALLBACK GlobalWndProc(HWND hWnd, UINT message, 
							   WPARAM wParam, LPARAM lParam )
{
	HDC hdc;
	PAINTSTRUCT ps; //structure used by Windows to paint things
	switch(message)
	{
	case WM_CREATE:
		
		return DefWindowProc( hWnd, message, wParam, lParam );
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//do any windows drawing here
		EndPaint(hWnd, &ps);
		break;

	case WM_SETFOCUS:
		SetWindowHasFocus(1);
		GameError("Received FOCUS");
		return DefWindowProc( hWnd, message, wParam, lParam );
		break;

	case WM_KILLFOCUS:
		SetWindowHasFocus(0);
		//paused = true;
		GameError("Lost FOCUS");
		return DefWindowProc( hWnd, message, wParam, lParam );
		break;

	case WM_DESTROY:
		GameError("Posting Destroy");
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
		break;

	//case WM_SETCURSOR:
	//	SetCursor(NULL);
	//	return true;
		
	default:
		return DefWindowProc(hWnd, message,wParam,lParam);

	}

	return 0;
}

GameWindow::GameWindow()
{
	window_startX = 0;
	window_startY = 0;
	window_sizeX = WINDOW_WIDTH;
	window_sizeY = WINDOW_HEIGHT;
	GameError("Created Game Window Object");
}

GameWindow::~GameWindow()
{
	CoUninitialize(); //to clean up windows a little better. not required.
	GameError("Terminating Game Window Object");
	//nothing we are killing this structure...
}

void GameWindow::MyRegisterClass(HINSTANCE hInstance, char* ATitle)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)GlobalWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION );
	wcex.hIconSm = LoadIcon(NULL, IDI_HAND);
	wcex.hCursor = LoadCursor( NULL, IDC_CROSS);
	//wcex.hCursor = 
	wcex.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = ATitle;
	

	RegisterClassEx(&wcex);
}

BOOL GameWindow::InitInstance(HINSTANCE hInst, int nCmdShow, char* ATitle, char* WTitle)
{
	
	hInstance = hInst;

	// setup some flags for the window
	//DWORD ws = WS_POPUP | WS_CAPTION | WS_SYSMENU | 
	//	WS_MINIMIZEBOX | WS_VISIBLE;
	DWORD ws = WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE;

	// create a rect structure that will be the window's client area
	RECT rc;
	rc.left = window_startX;
	rc.top = window_startY;
	rc.right = window_sizeX;
	rc.bottom = window_sizeY;
	
	// the rect must be adjusted so that the size is the *client area*
	// rather than the whole window size
	AdjustWindowRect(&rc, ws, FALSE);

	// CreateWindow will return 0 if unsuccessful
	// otherwise the handle to the created window is returned
	hWnd = CreateWindow(
		ATitle,
		WTitle,
		ws,
		window_startX,
		window_startY,
		rc.right - rc.left + window_startX,
		rc.bottom - rc.top + window_startY,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(!hWnd) 
	{
		// debug output
		GameError("GOT A FALSE WINDOW - BAD NEWS");
		
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);	// make the window visible
	UpdateWindow(hWnd);			// initially update the window

	return TRUE;
}

void GameWindow::SetWindowSize(int a, int b, int c, int d)
{
	window_startX = a;
	window_startY = b;
	window_sizeX = c;
	window_sizeY = d;
}

HWND & GameWindow::getHwnd()
{
	return hWnd;
}

HINSTANCE & GameWindow::get_hInstance()
{
	return hInstance;
}