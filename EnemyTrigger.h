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

#ifndef ENEMY_TRIGGER_H
#define ENEMY_TRIGGER_H

#include "GameCore.h"

typedef struct {
	int startX;         //in world coordinates
	int startY;         //in world coordinates
	int enemyId;        //which type of enemy should spawn
} EnemyTrigger;

#endif