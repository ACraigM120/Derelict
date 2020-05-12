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

class GameDX

{
public:
	//public functions to manipulate stuff
	HRESULT SetupD3D(HWND hWnd);
	LPDIRECT3DDEVICE9 & GetDeviceHandle();
	LPDIRECT3D9 & GetD3D();
	D3DRECT & GetClientSurfaceCoords();
	LPD3DXFONT & GetFont();


	//constructor & destructor
	GameDX();
	~GameDX();

private:
	//structures for init
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	//member pointers for D3D
	LPDIRECT3D9 m_pd3d;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPD3DXFONT m_pd3dFont;

};