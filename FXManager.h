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

#ifndef FX_MANAGER_H
#define FX_MANAGER_H

#include "GameCore.h"
#include "Emitter.h"
#include <vector>

class FXManager
{
public:

	// this will be a singleton class
	static FXManager *Instance();

	// FX functions
	void createExplosion(POINT pos);
	void flashScreen();
	void update(float dt);
	void draw();
	void reset();
	void loadTextures();
	bool isScreenWhite();

protected:
	
	FXManager();
	~FXManager();

private:

	std::vector<Emitter*> emitters;	// vector of emitters
	Texture whiteScreen;
	bool showWhite;
};
#endif