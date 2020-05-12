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

#ifndef BIN_H
#define BIN_H

#include "GameCore.h"
#include "Ship.h"
#include "Weapon.h"
#include <vector>

class Ship;
class Weapon;

struct Bin
{
	RECT myRect;
	POINT myPos;
	int binID;
	bool occupied;
	
	// vector of ships in me
	std::vector<Ship*> shipsInMe;
	std::vector<Weapon*> weaponsInMe;

};

#endif