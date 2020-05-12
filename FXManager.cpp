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

#include "FXManager.h"
#include "SpriteManager.h"
#include "SoundManager.h"

FXManager::FXManager()
{
	whiteScreen.filename = "graphics/whiteblock.dds";
	whiteScreen.texture = 0;
	showWhite = false;
}

FXManager::~FXManager()
{
	if(whiteScreen.texture) {
		whiteScreen.texture->Release();
		whiteScreen.texture = 0;
	}

	reset();
}

FXManager* FXManager::Instance()
{
	static FXManager manager;
	return &manager;
}

void FXManager::flashScreen() {
	showWhite = true;
	SoundManager::Instance()->playSoundEffect("audio/sfx/shutdown.wav");
}

void FXManager::loadTextures() {
	if(FAILED(SpriteManager::Instance()->loadTexture(&whiteScreen))) {
		GameError("Error loading white block in fxmgr");
	}else {
		whiteScreen.alpha = 0;
	}
}

void FXManager::reset() {
	std::vector<Emitter*>::iterator iter = emitters.begin();
	while(iter != emitters.end())
	{
		delete (*iter);
		emitters.erase(iter);
	}
}

void FXManager::update(float dt)
{
	std::vector<Emitter*>::iterator iter = emitters.begin();
	while(iter != emitters.end())
	{
		if((*iter)->isAlive())
		{
			(*iter)->update(dt);
			++iter;
		}
		else
		{
			delete (*iter);
			emitters.erase(iter);
		}
	}

	if(showWhite) {
		if(whiteScreen.alpha == 255) {
			showWhite = false;
			whiteScreen.alpha = 0;
		}else {
			if((whiteScreen.alpha += 3) > 255) {
				whiteScreen.alpha = 255;
			}
		}
	}
}

void FXManager::draw()
{
	HRESULT r;
	std::vector<Emitter*>::iterator iter = emitters.begin();
	while(iter != emitters.end())
	{
		if((*iter)->isAlive())
		{
			r = (*iter)->draw();
			if(FAILED(r))
			{
				GameError("failed drawing explosion from FXManager.draw()");
			}
		}
		++iter;
	}

	if(showWhite) {
		RECT sourceRect;
		sourceRect.left = sourceRect.top = 0;
		sourceRect.right = WINDOW_WIDTH;
		sourceRect.bottom = WINDOW_HEIGHT;

		HRESULT r = SpriteManager::Instance()->drawSprite(&whiteScreen, &sourceRect, 0, 0);
		if(FAILED(r)) {
			GameError("couldn't draw gravity bomb gfx effect");
		}
	}
}

void FXManager::createExplosion(POINT pos)
{
   // Randomly choose a explosion sound to play
   int explosionNum = ( rand()%4 );

   // Play the chosen sound
   switch( explosionNum )
   {
   case 0:
     	SoundManager::Instance()->playSoundEffect("audio/sfx/s_explosion_1.mp3");
      break;

   case 1:
     	SoundManager::Instance()->playSoundEffect("audio/sfx/s_explosion_2.mp3");
      break;

   case 2:
     	SoundManager::Instance()->playSoundEffect("audio/sfx/s_explosion_3.mp3");
      break;

   case 3:
     	SoundManager::Instance()->playSoundEffect("audio/sfx/s_explosion_4.mp3");
      break;
   }

	emitters.push_back(new Emitter("graphics/Explosion.dds", pos));
}

bool FXManager::isScreenWhite() {
	return showWhite;
}