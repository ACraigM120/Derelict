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

#include "DisplayManager.h"

//constructor
DisplayManager::DisplayManager( Player* ship, LevelManager* mgr ):
fontEngine("graphics/font.dds", 16, 32)
{
	flaming = false;
	iconOffset = 0;

	highScore = 0;

	player = ship;
	levelMgr = mgr;

	scoreBG.filename = "graphics/stardotstar.dds";
	scoreBG.texture = 0;

	scoreBGRect.top = 0;
	scoreBGRect.left = 0;
	scoreBGRect.bottom = 600;
	scoreBGRect.right = 800;

	icon.filename = "graphics/PlayerShip.dds";
	icon.texture = 0;

	iconRect.bottom = 64;
	iconRect.top = 0;
	iconRect.right = 64;
	iconRect.left = 0;

	flame.filename = "graphics/Flame.dds";
	flame.texture = 0;

	flameRect.top = 0;
	flameRect.bottom = 18;
	flameRect.left = 0;
	flameRect.right = 32;

	health.filename = "graphics/shieldStatus.dds";
	health.texture = 0;

	healthRect.top = 0;
	healthRect.bottom = 32;
	healthRect.left = 0;
	healthRect.right = 160;

	gBomb.filename = "graphics/powerups/gravitybombpowerup.dds";
	gBomb.texture = 0;

	instructions.filename = "graphics/overview.dds";
	instructions.texture = 0;

	gBombRect.top = 0;
	gBombRect.bottom = 32;
	gBombRect.left = 0;
	gBombRect.right = 32;

	D3DXMatrixScaling(&scaleMatrix, 0.5f, 0.5f, 1.0f);
}

//destructor
DisplayManager::~DisplayManager() {

	if(icon.texture)
	{
		icon.texture->Release();
		icon.texture = 0;
	}
	
	if( flame.texture ) {
		flame.texture->Release();
		flame.texture = 0;
	}

	if( health.texture ) {
		health.texture->Release();
		health.texture = 0;
	}

	if( gBomb.texture ) {
		gBomb.texture->Release();
		gBomb.texture = 0;
	}

	if( scoreBG.texture ) {
		scoreBG.texture->Release();
		scoreBG.texture = 0;
	}
}

HRESULT DisplayManager::init() {

	//load the font
	HRESULT r = fontEngine.LoadAlphabet();
	
	if(FAILED(r)) {
		GameError("Couldn't load font");
		return r;
	}

	r = SpriteManager::Instance()->loadTexture( &icon );
	if(FAILED(r))
	{
		GameError("failed loading icon texture");
		return r;
	}

	r = SpriteManager::Instance()->loadTexture( &flame );
	if( FAILED(r)) {
		GameError( "failed loading icon flame texture thingie" );
		return r;
	}

	r = SpriteManager::Instance()->loadTexture( &health );
	if( FAILED(r)) {
		GameError( "failed loading healthbar image, grr");
	}

	r = SpriteManager::Instance()->loadTexture( &gBomb );
	if( FAILED(r)) {
		GameError( "failed loading gravity bomb icon" );
	}

	r = SpriteManager::Instance()->loadTexture( &scoreBG );
	if( FAILED(r) ) {
		GameError( "scoreBG poof, damn, display.init() failure" );
	}

	r = SpriteManager::Instance()->loadTexture( &instructions );
	if( FAILED(r) ) {
		GameError( "scoreBG poof, damn, display.init() failure" );
	}

	return r;
}

void DisplayManager::display(int alpha) {
	if( player->getLife() >= (10.0 / 3.0)) {
		//change to 'healthy'
		iconRect.bottom = 64;
		iconRect.top = 0;
	}else if( player->getLife() >= (5.0 / 3.0)) {
		//change to 'medium' health
		iconRect.top = 64;
		iconRect.bottom = 128;
	}else {
		//ready to die
		iconRect.top = 128;
		iconRect.bottom = 192;
	}
	iconRect.right = 64 + iconOffset;
	iconRect.left = 0 + iconOffset;

	
	POINT position;
	position.x = 128;
	position.y = 32;
	POINT center;
	center.x = 32;
	center.y = 32;

	HRESULT r = SpriteManager::Instance()->drawSprite( &icon, &iconRect, position, &scaleMatrix, center, alpha );
	if(FAILED(r))
	{
		GameError("failed drawing icon");
	}

	if( flaming ) {
		POINT flamePos;
		flamePos.x = 104;
		flamePos.y = 56;
		POINT flameCenter;
		flameCenter.x = 32;
		flameCenter.y = 32;
		HRESULT r = SpriteManager::Instance()->drawSprite( &flame, &flameRect, flamePos, &scaleMatrix, flameCenter, alpha );
		if( FAILED(r) ) {
			GameError( "failed drawing flame in icon" );
		}
		flamePos.x = 108;
		flamePos.y = 64;
		HRESULT s = SpriteManager::Instance()->drawSprite( &flame, &flameRect, flamePos, &scaleMatrix, flameCenter, alpha );
		if( FAILED(s) ) {
			GameError( "failed drawing flame in icon" );
		}
	}

	char buf[250];
	sprintf(buf, " x %d", player->getLives());
	fontEngine.PrintString(80, 0, buf, alpha);
	fontEngine.PrintString(48, 32, "LEVEL", alpha);
	memset(buf, '\0', 250 );
	sprintf(buf, "%d", (levelMgr->getCurrLevelNum() + 1) );
	fontEngine.PrintRevString(160, 32, buf, alpha);
	fontEngine.PrintString(216, 0, "ENERGY", alpha);
	healthRect.right = (32 * player->getLife());
	HRESULT s = SpriteManager::Instance()->drawSprite( &health, &healthRect, 320, -4, alpha );
	if( FAILED(s) ) {
		GameError( "healthbar didn't draw... curses");
	}
	fontEngine.PrintString(448, 32, "HI-SCORE", alpha);
	fontEngine.PrintString(496, 0, "SCORE", alpha);

	memset(buf, '\0', 250);
	sprintf(buf, "%d", player->getScore());
	fontEngine.PrintRevString(752, 0, buf, alpha);
	if( highScore >= player->getScore() ) {
		memset( buf, '\0', 250 );
		sprintf( buf, "%d", highScore );
	}
	fontEngine.PrintRevString(752, 32, buf, alpha);

	HRESULT t = SpriteManager::Instance()->drawSprite( &gBomb, &gBombRect, 40, 566, alpha );
	if( FAILED(t) ) {
		GameError( "gbomb icon didn't draw... curses");
	}

	memset(buf, '\0', 250);
	sprintf(buf, " x %d", player->getGravityBombs() );
	fontEngine.PrintString( 64, 568, buf, alpha );
}

void DisplayManager::setFlame( bool isOn ) {
	flaming = isOn;
}

void DisplayManager::setOffset( int offset ) {
	iconOffset = offset;
}

void DisplayManager::gameOver(int alpha) {
	fontEngine.PrintString(328, 252, "GAME OVER", alpha);
}

void DisplayManager::gameOverMenu() {
	fontEngine.PrintString(104, 420, "press enter to return to title screen");
	fontEngine.PrintString(264, 452, "press esc to quit");
}

void DisplayManager::printSettings( int lives, int gBombs, float difficulty ) {
	char buf[ 64 ];
	sprintf( buf, "%d", lives );
	fontEngine.PrintString( 500, 170, buf );
	memset( buf, '\0', 64 );
	sprintf( buf, "%d", gBombs );
	fontEngine.PrintString( 500, 234, buf );
	if( difficulty == 0.5 ) {
		fontEngine.PrintString( 500, 300, "EASY" );
	} else if ( difficulty == 1.0 ) {
		fontEngine.PrintString( 500, 300, "NORMAL" );
	} else if ( difficulty == 2.0 ) {
		fontEngine.PrintString( 500, 300, "HARD" );
	} else if ( difficulty == 4.0 ) {
		fontEngine.PrintString( 500, 300, "$#%&" );
	}
}

// Prints the story sequence before the first level.  I suppose if I had more time I'd write an algorithm to take
// an arbitrary string for this...
//
// The argument is the offset on the y-axis.  By changing it over time the story can scroll.

void DisplayManager::printStory( int yOffset ) {
	fontEngine.PrintString( 32, yOffset + 0, "The year is 3228. Earth has become more than a" );
	fontEngine.PrintString( 32, yOffset + 32, "planet--it is  now a tyrannical empire bent on" );
	fontEngine.PrintString( 32, yOffset + 64, "cosmic domination,  obliterating all those who" );
	fontEngine.PrintString( 32, yOffset + 96, "would dare to stand against them.  Over 90% of" );
	fontEngine.PrintString( 32, yOffset + 128, "the  known  universe  has  fallen  into  their" );
	fontEngine.PrintString( 32, yOffset + 160, "grasp." );
	fontEngine.PrintString( 32, yOffset + 224, "The  reason  for Earth's  military  success is" );
	fontEngine.PrintString( 32, yOffset + 256, "two-fold:" );
	fontEngine.PrintString( 32, yOffset + 320, "I:  Earth's solar system was uninhabited (with" );
	fontEngine.PrintString( 32, yOffset + 352, "the exception of Earth itself) when the humans" );
	fontEngine.PrintString( 32, yOffset + 384, "began to seriously explore space.  This oddity" );
	fontEngine.PrintString( 32, yOffset + 416, "allowed  them  access  to  a superior quantity" );
	fontEngine.PrintString( 32, yOffset + 448, "of  resources   in   relation  to  neighboring" );
	fontEngine.PrintString( 32, yOffset + 480, "societies." );
	fontEngine.PrintString( 32, yOffset + 544, "II:  Around the time the  humans began to make" );
	fontEngine.PrintString( 32, yOffset + 576, "contact  with their neighbors,  they succeeded" );
	fontEngine.PrintString( 32, yOffset + 608, "in    perfecting    an   advanced   artificial" );
	fontEngine.PrintString( 32, yOffset + 640, "intelligence that allowed their fighters to go" );
	fontEngine.PrintString( 32, yOffset + 672, "unmanned.   Having no  need to  deploy  living" );
	fontEngine.PrintString( 32, yOffset + 704, "beings to the  military front,  they were able" );
	fontEngine.PrintString( 32, yOffset + 736, "to  fully   utilize   their   population   for" );
	fontEngine.PrintString( 32, yOffset + 768, "industrial purposes." );
	fontEngine.PrintString( 32, yOffset + 832, "Today,  Earth's military  production is buried" );
	fontEngine.PrintString( 32, yOffset + 864, "so deep in their territory that any attempt to" );
	fontEngine.PrintString( 32, yOffset + 896, "halt it is a dream at best.  Conversely, their" );
	fontEngine.PrintString( 32, yOffset + 928, "enemies are daily  slaughtered by the unmanned" );
	fontEngine.PrintString( 32, yOffset + 960, "fighters." );
	fontEngine.PrintString( 32, yOffset + 1024, "You  are  the  newest   prototype  in  Earth's" );
	fontEngine.PrintString( 32, yOffset + 1056, "soulless  armory,  codenamed Derelict.   Today" );
	fontEngine.PrintString( 32, yOffset + 1088, "your performance will  finally be tested in an" );
	fontEngine.PrintString( 32, yOffset + 1120, "actual  combat  situation:   an  attack  on an" );
	fontEngine.PrintString( 32, yOffset + 1152, "outlying enemy starbase.  Your objective is to" );
	fontEngine.PrintString( 32, yOffset + 1184, "penetrate its defenses, navigate into the main" );
	fontEngine.PrintString( 32, yOffset + 1216, "fusion reactor,  and reduce it  to space dust." );
	fontEngine.PrintString( 32, yOffset + 1280, "Kill  as  many  of  the   enemy  as  you  can." );
	fontEngine.PrintString( 32, yOffset + 1312, "Remember,  your creators have little tolerance" );
	fontEngine.PrintString( 32, yOffset + 1344, "for inefficiency..." );
}

void DisplayManager::printInitials( char* initial1, char* initial2, char* initial3, int alpha ) {
	fontEngine.PrintString( 84, 50, "CONGRATULATIONS!  YOU GOT A HIGH SCORE!", alpha );
	fontEngine.PrintString( 376, 125, initial1, alpha );
	fontEngine.PrintString( 392, 125, initial2, alpha );
	fontEngine.PrintString( 408, 125, initial3, alpha );
}

void DisplayManager::printTopPlayers( char** topPlayers, int* topScores, int alpha ) {
	SpriteManager::Instance()->drawSprite( &scoreBG, &scoreBGRect, 0, 0, alpha );
	fontEngine.PrintString( 280, 75, "TOP 5 DERELICTS", alpha );
	char buf[25];
	for( int i = 0; i < 5; i++ ) {
		memset( buf, '\0', 25 );
		sprintf( buf, "%d", topScores[i] );
		fontEngine.PrintString( 100, ( 75 * i + 150 ), topPlayers[i], alpha );
		fontEngine.PrintRevString( 700, ( 75 * i + 150 ), buf, alpha );
	}
}

void DisplayManager::setHighScore( int hScore ) {
	highScore = hScore;
}

void DisplayManager::printCredits( int yOffset, int alpha ) {
	fontEngine.PrintString( 32, yOffset + 0, "The mission is complete, but the demo is over.", alpha );
	fontEngine.PrintString( 243, yOffset + 64, "To be continued...?", alpha );
	fontEngine.PrintString( 344, yOffset + 600, "CREDITS" );
	fontEngine.PrintString( 380, yOffset + 704, "ART" );
	fontEngine.PrintString( 32, yOffset + 768, "Art Director / Lead Artist:  Brian J. Weis" );
	fontEngine.PrintString( 32, yOffset + 800, "Font Artist:  Andrew C. McCartney" );
	fontEngine.PrintString( 32, yOffset + 832, "Some Art:  Jason A. Gowan" );
	fontEngine.PrintString( 32, yOffset + 864, "Some Art:  Brian O. Adeloye" );
	fontEngine.PrintString( 32, yOffset + 896, "More Art:  Brian J. Weis" );
	fontEngine.PrintString( 32, yOffset + 928, "Even More Art:  Brian J. Weis" );
	fontEngine.PrintString( 32, yOffset + 960, "Almost All the Art:  Brian J. Weis" );
	fontEngine.PrintString( 360, yOffset + 1056, "MUSIC" );
	fontEngine.PrintString( 32, yOffset + 1120, "Composer:  Jason A. Gowan" );
	fontEngine.PrintString( 272, yOffset + 1214, "DOCUMENTATION" );
	fontEngine.PrintString( 32, yOffset + 1280, "Pissed as Hell:  Jason A. Gowan" );
	fontEngine.PrintString( 32, yOffset + 1312, "Vexed:  Brian O. Adeloye" );
	fontEngine.PrintString( 32, yOffset + 1344, "Annoyed:  Andrew C. McCartney" );
	fontEngine.PrintString( 32, yOffset + 1376, "Bored:  Mark S. Hazlewood" );
	fontEngine.PrintString( 288, yOffset + 1472, "ENEMY DESIGN" );
	fontEngine.PrintString( 32, yOffset + 1536, "Artist:  Brian J. Weis" );
	fontEngine.PrintString( 32, yOffset + 1568, "Flight Paths:  Andrew C. McCartney" );
	fontEngine.PrintString( 32, yOffset + 1600, "Flight Paths:  Brian O. Adeloye" );
	fontEngine.PrintString( 32, yOffset + 1632, "Boss Design:  Everyone" );
	fontEngine.PrintString( 32, yOffset + 1664, "Boss Logic:  Mark S. Hazlewood" );
	fontEngine.PrintString( 32, yOffset + 1696, "Boss Logic:  Jason A. Gowan" );
	fontEngine.PrintString( 32, yOffset + 1728, "Boss Logic:  Brian O. Adeloye" );
	fontEngine.PrintString( 272, yOffset + 1824, "CHARACTER DESIGN" );
	fontEngine.PrintString( 32, yOffset + 1888, "99%:  Brian J. Weis" );
	fontEngine.PrintString( 288, yOffset + 1984, "LEVEL DESIGN" );
	fontEngine.PrintString( 32, yOffset + 2048, "See Above:  Everyone" );
	fontEngine.PrintString( 32, yOffset + 2080, "But it Was Mostly:  Brian J. Weis" );
	fontEngine.PrintString( 280, yOffset + 2176, "WEAPON DESIGN" );
	fontEngine.PrintString( 32, yOffset + 2240, "Stuff:  Everyone" );
	fontEngine.PrintString( 308, yOffset + 2336, "STORY" );
	fontEngine.PrintString( 32, yOffset + 2400, "Author:  Andrew C. McCartney" );
	fontEngine.PrintString( 48, yOffset + 2496, "THINGS THAT WILL BE PLACED UNDER PROGRAMMING" );
	fontEngine.PrintString( 32, yOffset + 2560, "Anywhere You See the Font:  Andrew C. McCartney" );
	fontEngine.PrintString( 32, yOffset + 2592, "Collision Detection:  Brian O. Adeloye" );
	fontEngine.PrintString( 32, yOffset + 2624, "Collision Detection:  Brian J. Weis" );
	fontEngine.PrintString( 32, yOffset + 2656, "Collision Detection:  Jason A. Gowan" );
	fontEngine.PrintString( 32, yOffset + 2688, "Collision Detection:  Mark S. Hazlewood" );
	fontEngine.PrintString( 32, yOffset + 2720, "UnMappy:  Jason A. Gowan" );
	fontEngine.PrintString( 32, yOffset + 2752, "Cool Ship Icon in the HUD:  Andrew C. McCartney" );
	fontEngine.PrintString( 32, yOffset + 2784, "Bugging:  Everyone" );
	fontEngine.PrintString( 32, yOffset + 2816, "DeBugging:  Everyone" );
	fontEngine.PrintString( 32, yOffset + 2848, "ReBugging:  Everyone" );
	fontEngine.PrintString( 32, yOffset + 2880, "You Get the Idea:  Everyone" );
	fontEngine.PrintString( 32, yOffset + 2912, "Weapons:  Jason A. Gowan" );
	fontEngine.PrintString( 32, yOffset + 2944, "Weapons:  Mark S. Hazlewood" );
	fontEngine.PrintString( 32, yOffset + 2976, "Sweet Fades:  Brian O. Adeloye" );
	fontEngine.PrintString( 32, yOffset + 3008, "Cool Explosions:  Mark S. Hazlewood" );
	fontEngine.PrintString( 32, yOffset + 3040, "Engine:  Brian O. Adeloye" );
	fontEngine.PrintString( 32, yOffset + 3072, "Engine Tweaking:  Everyone" );
	fontEngine.PrintString( 312, yOffset + 3168, "OTHER STUFF" );
	fontEngine.PrintString( 32, yOffset + 3232, "Likes Stew:  Mark S. Hazlewood" );
	fontEngine.PrintString( 32, yOffset + 3264, "Sleepy:  Brian O. Adeloye" );
	fontEngine.PrintString( 32, yOffset + 3296, "Green Skunk:  Brian J. Weis" );
	fontEngine.PrintString( 32, yOffset + 3328, "Scares the Above:  Andrew C. McCartney" );
	fontEngine.PrintString( 32, yOffset + 3360, "Has a Kid:  Jason A. Gowan" );
	fontEngine.PrintString( 32, yOffset + 3392, "Current President:  George W. Bush" );
	fontEngine.PrintString( 32, yOffset + 3424, "Next President:  Some Guy" );
	fontEngine.PrintString( 344, yOffset + 3520, "TESTING" );
	fontEngine.PrintString( 32, yOffset + 3584, "Play Testing:  Everyone" );
	fontEngine.PrintString( 32, yOffset + 3616, "Unit Testing:  Everyone" );
	fontEngine.PrintString( 32, yOffset + 3648, "Integration Testing:  Someone" );
	fontEngine.PrintString( 64, yOffset + 3744, "SPECIAL THANKS AND LEGAL RIGHTS TO" );
	fontEngine.PrintString( 32, yOffset + 3808, "Original Explosion Art:  Professor Phelps" );
	fontEngine.PrintString( 32, yOffset + 3840, "Baby Picture Supplier:  Anonymous" );
	fontEngine.PrintString( 32, yOffset + 3872, "Some Sound Effects:  Flashkit" );
	fontEngine.PrintString( 32, yOffset + 3906, "Player:  You (Duh!)" );
	fontEngine.PrintString( 32, yOffset + 3938, "Read the Credits:  No One, Right?" );
	fontEngine.PrintString( 32, yOffset + 3970, "Facilities:  RIT, IT Department, ET Lab" );
	fontEngine.PrintString( 32, yOffset + 4002, "Facilities:  RIT, CS Department, Break Out Rooms" );
	fontEngine.PrintString( 32, yOffset + 4038, "Concept Taken From:  Countless 2D Shooters" );
	fontEngine.PrintString( 32, yOffset + 4070, "Map Editor/Life Saver: Mappy (www.tilemap.co.uk)");
	fontEngine.PrintString( 32, yOffset + 4464, "Derelict produced, developed, programmed," );
	fontEngine.PrintString( 32, yOffset + 4496, "composed, smoked, sniffed, snorted, inhaled," );
	fontEngine.PrintString( 32, yOffset + 4528, "tasted, thrashed, owned, reduced, slapped," );
	fontEngine.PrintString( 32, yOffset + 4560, "spangled, spackled, nailed, blown up, inflated," );
	fontEngine.PrintString( 32, yOffset + 4592, "prodded, aliased, managed, paid, laid, made," );
	fontEngine.PrintString( 32, yOffset + 4624, "aged, sanctified, defiled, operated on," );
	fontEngine.PrintString( 32, yOffset + 4656, "discovered, spun around, and any other random" );
	fontEngine.PrintString( 32, yOffset + 4688, "verb you can think of by..." );
	fontEngine.PrintString( 388, yOffset + 4786, "*.*" );
}

void DisplayManager::showInstructions() {
	HRESULT r = SpriteManager::Instance()->drawSprite( &instructions, 0, 0, 0 );
	if( FAILED(r) ) {
		GameError( "instructions didn't draw... curses");
	}
}