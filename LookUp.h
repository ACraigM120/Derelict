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

#ifndef LOOKUP_H
#define LOOKUP_H

#include <math.h>

class LookUp
{


public:

	// values are rounded rather than truncated, I fudged it after 90 (using what I know about the sin curve)



	// ctor/dtor
	LookUp();
	~LookUp();

	static LookUp *Instance();
	
	// lookup functions
	float getSin(int degree);
	float getCos(int degree);

	// build tables
	void buildSinTable();
	void buildCosTable();

private:
	
	// sin/cos tables
	float sinTable[360];
	float cosTable[360];



};
#endif