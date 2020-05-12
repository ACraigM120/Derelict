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
#include "timing.h"
#include <stdio.h>
Timing::Timing()
{
	time_Frequency = 0;
	time_StartTime = 0;
	time_FrameCount = 0;
	time_Deviance = 0;
	time_PauseTime = 0;
	time_MaxFrameRate = MAX_FRAME_RATE;
	time_FrameRate = 0;
}

Timing::~Timing()
{
	time_Frequency = 0;
	time_StartTime = 0;
	time_FrameCount = 0;
	time_Deviance = 0;
	time_PauseTime = 0;
	time_MaxFrameRate = MAX_FRAME_RATE;
	time_FrameRate = 0;
}

HRESULT Timing::SetupTiming()
{
	QueryPerformanceFrequency( (LARGE_INTEGER*)&time_Frequency );
	if (time_Frequency == 0) 
	{
		return E_FAIL;
	}

	//float mod = GetNumTicksPerMs();
	//time_PauseTime = (int)(mod / time_MaxFrameRate );


	float frameRateInMs =  (float)time_MaxFrameRate;
	float mod = GetNumTicksPerMs() ;
	float finalVal = (mod / frameRateInMs);
	time_PauseTime = int(finalVal);

	return S_OK;

}

void Timing::SetStartTime()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&time_StartTime );
}

float Timing::GetNumTicksPerMs()
{
	return((float)time_Frequency / 1000.0f );
}

void Timing::Pause(int Milliseconds)
{
	INT64 SecondsDelay = (INT64)Milliseconds * 1000;
	INT64 CurrentTime = 0;

	while(TRUE)
	{
		QueryPerformanceCounter( (LARGE_INTEGER*) &CurrentTime );
		if ( (CurrentTime - time_StartTime) > SecondsDelay)
			break;
	}

}

//calculates how many ticks the last frame took (used in figuring out fps)
//additionally, returns how many seconds the last frame took
float Timing::CalcFrameCount()
{
	INT64 NewCount = 0;
	static INT64 LastFrameCount = 0; //number of ticks at the last frame
	static INT64 LastSecCount = 0;   //number of ticks at the last second
	INT64 FrameDifference = 0;
	INT64 SecDifference =0;

	QueryPerformanceCounter( (LARGE_INTEGER*) &NewCount );

	if(LastFrameCount == 0) {
		LastFrameCount = NewCount;
		LastSecCount = NewCount;
	}

	if (NewCount == 0)
	{
		//game error no HPC
	}

	time_FrameCount++;

	SecDifference = NewCount - LastSecCount;      //number of ticks since the last second
	FrameDifference = NewCount - LastFrameCount;  //number of ticks since the last frame

	//length of this frame (s) = (ticks)/(ticks/second)
	//reminder:time_Frequency is the number of ticks it takes for 1 second to pass
	float dt = ((float) FrameDifference)/time_Frequency; //how long this frame lasted

	//if according to the number of ticks we've seen since the last second, another second has passed
	if (SecDifference >= time_Frequency)
	{		
		time_FrameRate = time_FrameCount;  //fps seen last second
		time_FrameCount = 0;  //reset number of frames seen (for the next second)
		
		LastSecCount = NewCount;
	}
	
	LastFrameCount = NewCount;

	time_Deviance = (float)time_FrameRate / time_MaxFrameRate;
	//SetDeviance(time_Deviance);

	return dt;
}

HRESULT Timing::DisplayFrameRate(LPD3DXFONT &m_pd3dFont, int x, int y)
{
	RECT rect =  {x, y, 200, 200};
	RECT rect2 = {x, y+20, 0, 0};

	char framerate[256];
	char deviance[256];

	sprintf(framerate, "Frame Rate: %d", time_FrameRate);
	sprintf(deviance,  "Deviance: %f", time_Deviance);
    
	m_pd3dFont->DrawText(NULL, framerate, -1, &rect, DT_NOCLIP, D3DCOLOR_XRGB(255,255,255));
	m_pd3dFont->DrawText(NULL, deviance, -1, &rect2, DT_NOCLIP, D3DCOLOR_XRGB(255,255,255));

	return S_OK;
}

int Timing::GetFrameRate()
{
	return time_FrameRate;
}

int Timing::GetPauseTime()
{
	return time_PauseTime;
}

int Timing::GetTimeDeviance()
{
	return (int)time_Deviance;
}