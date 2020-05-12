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

#ifndef Timing_H
#define Timing_H
#pragma once

//#include <windows.h>
//#include <d3dx8.h>

class Timing
{
public:
	
	int GetFrameRate();
	int GetPauseTime();
	int GetTimeDeviance();
	HRESULT SetupTiming();
	void SetStartTime();
	void Pause(int);
	float GetNumTicksPerMs();
	float CalcFrameCount();
	
	//DOES NOT BELONG HERE -------
	HRESULT DisplayFrameRate(LPD3DXFONT &m_pd3dFont, int, int);

	Timing();
	~Timing();
private:
	int time_PauseTime;
	int time_FrameRate;
	INT64 time_Frequency;
	INT64 time_StartTime;
	int time_FrameCount;
	float time_Deviance;
	float time_MaxFrameRate;
};

#endif