#include "GameCore.h"

void GameError (char* Error)
{
	OutputDebugString("*** ");
	OutputDebugString(Error);
	OutputDebugString(" ***");
	OutputDebugString( "\n" );
}

void GameError (const char* Error)
{
	OutputDebugString("*** ");
	OutputDebugString(Error);
	OutputDebugString(" ***");
	OutputDebugString( "\n" );
}