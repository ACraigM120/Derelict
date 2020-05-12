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
#include "GameDXSetup.h"

//constructor
GameDX::GameDX()
{
	//initialize all member pointers to zero
	m_pd3dFont = 0;
	m_pd3dDevice = 0;
	m_pd3d = 0;
}

GameDX::~GameDX() 
{
	//release everything and return all pointers to zero
	if (m_pd3dFont)
	{
		GameError("Killing D3D Font");
		m_pd3dFont->Release();
		m_pd3dFont = 0;
	}
	if (m_pd3dDevice)
	{
		GameError("Killing D3D Device");
		m_pd3dDevice->Release();
		m_pd3dDevice = 0;
	}
	if (m_pd3d )
	{
		GameError("Killing D3D COM Object");
		m_pd3d->Release();
		m_pd3d = 0;
	}
}

HRESULT GameDX::SetupD3D(HWND hWnd)
{
	bool bWindowed = true;
	
	// create a handle to the Direct3D library
	// everything done with Direct3D will go through this handle
	m_pd3d = Direct3DCreate9( D3D_SDK_VERSION );
	
	if (m_pd3d == 0)
	{
		GameError("COULD NOT CREATE DX9 OBJECT");
		return E_FAIL;
	}

	//find out about current display
	HRESULT r = 0;			// error-holding stuff
	D3DDISPLAYMODE d3ddm;	// structure to be filled in later
	
	// get the display mode information and store it in the 
	// d3ddm structure declared above                       ***
	// need this for windowed mode to use the current settings
	// of the desktop
	r = m_pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	
	if (FAILED(r))
	{
		GameError("Could Not Get Display Adapter Information");
		return E_FAIL;
	}

	if(MessageBox(hWnd, "Would You Like to Run in Fullscreen Mode?", "", MB_YESNO) == IDYES)
	{
		bWindowed = false;
		ShowCursor(false);
	}

	//create display parameters for device "creation"
	// used for fullscreen mode
    D3DFORMAT FullScreenFormat = D3DFMT_A8R8G8B8;

	// setup options for the device before getting a handle to it
	// d3dpp is defined in the header file
	// see p.48 in book

	// present parameters is the structure that needs to be filled out
	// to get a handle to the device
	
	// clear out the structure
	ZeroMemory( &d3dpp, sizeof( D3DPRESENT_PARAMETERS ));
	
	//set to non-full screen
	d3dpp.Windowed = bWindowed;
	
	//tell DirectX that it is allowed to mess with the back buffer
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	//set the back buffer to the same format as the primary surface
	d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	
	// one backbuffer
	d3dpp.BackBufferCount = 1;
	
	//set target window to render to
	d3dpp.hDeviceWindow = hWnd;
	
	//turn on the depth buffer
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; //16bit depth buffer
    
	//set the refresh rate - not used if windowed (0 = autodetect)
	d3dpp.FullScreen_RefreshRateInHz = bWindowed ? 0 : 60;
    
	//set the presentation interval - MUST be 0 if windowed
	//d3dpp.FullScreen_PresentationInterval = WINDOWED ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	
	//allow access to the back buffer directly for 2D work (slow...)
	// lockable
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	//get a handle to the device (watch out if you have two...)
	// store the device handle in m_pd3dDevice (declared in header file)
	r = m_pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice);

    if (FAILED(r))
	{
		GameError("Could not create device...");
		return E_FAIL;
	}

	//clear the back buffer and then present
	
	// fill the buffer with a color
	m_pd3dDevice->Clear( 0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 25, 50), 0.0f, 0);	

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pd3dDevice->SetSamplerState( 0,  D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	
	// tells the card we're done drawing and can flip
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	//  DIRECTX8 FONT (much simpler)
	//Get a font to draw with as neccessary
	//r = D3DXCreateFont(
	//	m_pd3dDevice,
	//	(HFONT)GetStockObject(DEVICE_DEFAULT_FONT),
	//	&m_pd3dFont);
	

    r = D3DXCreateFont( m_pd3dDevice,
    12, //pixel height of font
    0, //calc width from height
    FW_NORMAL, //weight
    1, //num mip levels (0 creates full set)
    false, //italic yes/no
    DEFAULT_CHARSET, //dword charset
    OUT_DEFAULT_PRECIS, //dword precisioin
    DEFAULT_QUALITY, //dword quality
    DEFAULT_PITCH|FF_DONTCARE, //pitch and family
    "", //name of particular font
    &m_pd3dFont
    );



    if (FAILED(r))
	{
		GameError("Error Creating Font");
		return r;
	}

	return S_OK;
}

LPDIRECT3DDEVICE9 & GameDX::GetDeviceHandle()
{
	return m_pd3dDevice;
}

LPDIRECT3D9 & GameDX::GetD3D()
{
	return m_pd3d;
}

D3DRECT & GameDX::GetClientSurfaceCoords()
{
	static D3DRECT rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	return rect;
}

LPD3DXFONT & GameDX::GetFont()
{
	return m_pd3dFont;
}

