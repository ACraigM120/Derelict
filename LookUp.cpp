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

#include "LookUp.h"

LookUp::LookUp()
{
	
}

LookUp::~LookUp()
{

}

LookUp *LookUp::Instance() 
{
	//instead of 'new'ing 'this' and returning the pointer, just return the address. this way we dont
	//have to worry about explicitly calling the destructor of 'this'
	static LookUp lookup;
	return &lookup; 
}

void LookUp::buildSinTable()
{
	for(int i = 0; i < 360; i++)
	{
		sinTable[i] = (float)(sin((double)i));
	}
}

void LookUp::buildCosTable()
{
	for(int i = 0; i < 360; i++)
	{
		cosTable[i] = (float)(cos((double)i));
	}
}

float LookUp::getSin(int degree)
{
	return sinTable[degree];
}

float LookUp::getCos(int degree)
{
	return cosTable[degree];
}