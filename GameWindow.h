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

class GameWindow
{
private:
	int window_startX;
	int window_startY;
	int window_sizeX;
	int window_sizeY;

	HWND hWnd;
	HINSTANCE hInstance;

public:
	
	virtual void MyRegisterClass(HINSTANCE, char*);
	virtual BOOL InitInstance(HINSTANCE, int, char*, char*);
	virtual void SetWindowSize(int, int, int, int);
	virtual HWND & getHwnd();
	virtual HINSTANCE & get_hInstance();

	GameWindow();
	~GameWindow();
};
