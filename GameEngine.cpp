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

#include "GameEngine.h"
#include "BulletData.h"

/*
 * constructor
 */
GameEngine::GameEngine():
player("graphics/PlayerShip.dds", 64, 64), levelMgr("data/levellist.txt"),
		state(intro)
{
	introMgr = 0;  //zero the pointer
	gameOverTimer = 0.0f;

	showBins = false;

	display = 0;
	alpha = 255;
}

/*
 * destructor
 */
GameEngine::~GameEngine() 
{
	if( topPlayers ) {
		delete[] topPlayers;
		topPlayers = 0;
	}

	if( topScores ) {
		delete[] topScores;
		topScores = 0;
	}

	if( initials ) {
		delete[] initials;
		initials = 0;
	}

	if( titleMenuOptions ) {
		delete[] titleMenuOptions;
		titleMenuOptions = 0;
	}

	if( optionMenuOptions ) {
		delete[] optionMenuOptions;
		optionMenuOptions = 0;
	}

	if( initialEntry ) {
		delete initialEntry;
		initialEntry = 0;
	}

	if( optionMenu ) {
		delete optionMenu;
		optionMenu = 0;
	}

	if( titleMenu ) {
		delete titleMenu;
		titleMenu = 0;
	}

	if(introMgr) {
		delete introMgr;
		introMgr = 0;
	}

	if(display)
	{
		delete display;
		display = 0;
	}
}

/*
 * initWindow
 *
 * setups up the game window
 *
 * hInstance - handle to the instance of this application
 * nShowCmd - determines how window will be displayed when application starts
 *
 * returns status code (was this operation successful or not)
 */
HRESULT GameEngine::initWindow(HINSTANCE hInstance, int nShowCmd) 
{
    gameWindow.SetWindowSize(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
	
	// register the window
	gameWindow.MyRegisterClass(hInstance, APP_TITLE);
	
	// create the window
	if (!gameWindow.InitInstance(hInstance, nShowCmd, APP_TITLE, WINDOW_TITLE))
	{
		return E_FAIL;
	}

	return S_OK;
}

/*
 * initD3D
 *
 * sets up direct3d
 *
 * returns status code (was this operation successful or not)
 */
HRESULT GameEngine::initD3D() 
{
	return gameDX.SetupD3D(gameWindow.getHwnd());;
}

/*
 * initInput
 *
 * sets up directinput
 *
 * returns status code (was this operation successful or not)
 */
HRESULT GameEngine::initInput() 
{
	HRESULT r;
	r = input.SetupInput(gameWindow.get_hInstance());

	if(FAILED(r)) {
		return r;
	}

	r = input.SetupKeyboard(gameWindow.getHwnd());

	if(FAILED(r)) 
	{
		return r;
	}

	return S_OK;
}

/*
 * initTiming
 *
 * sets up the timer
 *
 * returns status code (was this operation successful or not)
 */
HRESULT GameEngine::initTiming() 
{
	return timer.SetupTiming();
}

/*
 * initSpriteMgr
 *
 * sets up the sprite manager
 *
 * returns status code (was this operation successful or not)
 */
HRESULT GameEngine::initSpriteMgr() 
{
	spriteMgr = SpriteManager::Instance();
	return spriteMgr->initSpriteManager(gameDX.GetDeviceHandle());
}

/*
 * nextFrame
 *
 * does all the calculations and updates required before drawing the next frame
 */
void GameEngine::nextFrame() 
{
	static float lastFrameTime = 0.0; //how long the last frame was (in secs)
	
	//start the timer for this frame
	timer.SetStartTime();
	
	//do different things depending on the game state
	switch(state) 
	{
		case paused:
			//check to see if we're unpausing
			if(input.keyJustPressed(DIK_P)) 
			{
				// resume the audio
				SoundManager::Instance()->resumeMusic();
				state = lastState;
			}
			break;
		case gravityBombing:
			FXManager::Instance()->update(lastFrameTime);
			enemyMgr->update(lastFrameTime,levelMgr.getMainground()->getEndScreenWorldX(), player.getPosY(), true);
			checkCollision();
			
			//if gravity bomb is over...resume with game
			if(!FXManager::Instance()->isScreenWhite()) {
				state = playing;
			}

			break;

		case intro:
			//if the intro is finished
			//(due to either time elapsed or user bypassing it)
			if(!introMgr->update(lastFrameTime) ||
			   (input.keyJustPressed(DIK_RETURN) && !introMgr->nextScreen())) {
			
			    //intro's over...move to the playing state
				state = title;

				//we're done with the intro, so free up the mem it used
				delete introMgr;  
				introMgr = 0;

				
			}

			break;
      case coreDying:
         if((coreExplosionTimer -= lastFrameTime) < 0.0f) {
            POINT p;
            p.x = (rand() % 101) + 700;
            p.y = rand() % WINDOW_HEIGHT;
            FXManager::Instance()->createExplosion(p);
            coreExplosionTimer = EXPLOSION_TIMER_MAX;
         }

         if((coreDyingTimer -= lastFrameTime) < 0.0f) {
			//go to the you win screen and/or credits
			 soundMgr->playBackgroundMusic( "audio/music/s_credits.mp3", false );
			 alpha = 255;
			 state = victory;
         }
		case playing:
			//firstly check for pause
			if(input.keyJustPressed(DIK_P)) 
			{  
				// Pause the audio.
				SoundManager::Instance()->pauseMusic();
				state = paused;
				lastState = playing;
			}
			else 
			{
				//player's speed based on how fast the level is scrolling
				if(levelMgr.isSpedUp()) {
					player.setSpeed(4);
				}else {
					player.setSpeed(3);
				}

				checkInput();             //check player input
				
				// update bins
				std::vector<Bin>::iterator iter = BinManager::Instance()->getWorldBins()->begin();
				while(iter != BinManager::Instance()->getWorldBins()->end())
				{
					(*iter).occupied = false;
					++iter;
				}
				
				levelMgr.update(lastFrameTime);
				player.update(lastFrameTime, enemyMgr->getActiveEnemies(), PowerUpManager::Instance()->getActivePowerUps());
				//update player's score and all enemies in one fell swoop
				bool gbomb = (input.keyJustPressed(DIK_LMENU) || input.keyJustPressed(DIK_RMENU))
							 && player.fireGravityBomb();
				if(gbomb) {
					state = gravityBombing;
				}            
				player.addPoints(enemyMgr->update(lastFrameTime, levelMgr.getMainground()->getEndScreenWorldX(),
												  player.getPosY(), gbomb));
				
				// update power-ups
				PowerUpManager::Instance()->update(lastFrameTime, 
												   levelMgr.getMainground()->getTileSize() *
												   levelMgr.getMainground()->getEndX() +
												   levelMgr.getMainground()->getXOffset(), 
												   player.getPosY());

				FXManager::Instance()->update(lastFrameTime);
				checkCollision();


				//if the player's dead...it's game over
				if(player.getLives() == 0) {
					soundMgr->playSoundEffect( "audio/sfx/s_gameover.wav" );
					soundMgr->playBackgroundMusic( "audio/music/s_gameover.mp3", true );
					state = gameover;
				}

				//if the core is dying
				if(enemyMgr->isCoreDying()) {
					if(state == playing) {
						coreExplosionTimer = 0.0f;
						coreDyingTimer = CORE_EXPLODING_TIME;
						state = coreDying;
					}
				}
			}
			break;
		
		case gameover:
			gameOverTimer += lastFrameTime;
			enemyMgr->update(lastFrameTime, levelMgr.getMainground()->getEndScreenWorldX(), -1, 0);
			//update level mgr (animation, scroll world, fade world, etc
			levelMgr.update(lastFrameTime);
			PowerUpManager::Instance()->update(lastFrameTime, -1, -1);
			FXManager::Instance()->update(lastFrameTime);

			//restarting a game, so reset the states of everything that needs to be reset
			if(input.IsKeyDown(DIK_RETURN) || (gameOverTimer > 2.0) ) {
				scoreTimer = 0;
				gameOverTimer = 0.0f;
				if( player.getScore() > topScores[4] ) {
					state = initialsScreen;
				} else {
					state = scoreScreen;
					lastState = gameover;
				}
			}
			break;
		case title:
			if(input.keyJustPressed(DIK_RETURN)) {
				switch( titleMenu->getRow() ) {
				case( 0 ):
					state = story;
					// Play the story music
					soundMgr->playBackgroundMusic( "audio/music/s_story_music.mp3", false );
					break;
				case( 1 ):
					state = options;
					optionMenu->resetPos();
					break;
				}
			}
			checkInput();
			break;
		case options:
			if( input.keyJustPressed(DIK_RETURN) && ( optionMenu->getRow() == 3 ) ) {
				state = title;
				titleMenu->resetPos();
				player.setStartingValues( startingLives, startingGBombs, difficulty );
				player.reset();
				enemyMgr->setDifficulty( difficulty );
			}
			checkInput();
			break;
		case story:
			if( input.keyJustPressed( DIK_RETURN ) ) {
				storyScroll = 632;
				state = instructions;
			}
			storyScroll -= 1;
			timer.Pause( 100 );
			if( storyScroll <= -1400 ) {
				storyScroll = 632;
				state = instructions;
			}
			break;
		case initialsScreen:
			//crossfades whatever was on the screen when the player died with the hi score screen
			if( (alpha -= 2) <= 0) {
				alpha = 0;
			}

			scoreTimer += 1;

			if( ( ( input.keyJustPressed(DIK_RETURN) && ( initialEntry->getRow() == 10 ) && 
				( initialEntry->getCol() == 8 ) ) ) || ( scoreTimer > 1000 ) ) {
				position = -1;
				for( int i = 0; ( i < 5 ) && ( position == -1 ); i++ ) {
					if( player.getScore() > topScores[i] ) {
						if( i == 0 ) {
							display->setHighScore( player.getScore() );
						}
						position = i;
						break;
					}
				}

				char temp[4];
				strcpy( temp, initial1 );
				strcat( temp, initial2 );
				strcat( temp, initial3 );
	
				if( position == 4 ) {
					topScores[position] = player.getScore();
					strcpy(topPlayers[position], temp);
				} else if( position != -1 ) {
					for( int i = 4; i > position; i-- ) {
						topScores[i] = topScores[i - 1];
						strcpy( topPlayers[i], topPlayers[i - 1] );
					}
					topScores[position] = player.getScore();
					strcpy(topPlayers[position], temp);
				}

				

				fstream fileOutput( "data/HighScores.txt" );
				for( int i = 0; i < 5; i++ ) {
					fileOutput << topPlayers[i] << "\n" << topScores[i] << "\n";
				}
				fileOutput.close();

				scoreTimer = 0;
				state = scoreScreen;
				lastState = initialsScreen;
			} else if( input.keyJustPressed( DIK_RETURN ) && ( initialEntry->getRow() == 10 ) && ( initialEntry->getCol() == 0 ) ) {
				if( position > 0 ) {
					position--;
				}
				switch( position ) {
				case( 0 ):
					initial2 = " ";
				case( 1 ):
					initial3 = " ";
				case( 2 ):
					break;
				}
			} else if( input.keyJustPressed( DIK_RETURN ) && ( initialEntry->getRow() < 10 ) ) {
				if( position < 2 ) {
					position++;
				} else {
					initialEntry->setPos( 8, 10 );
				}
			} else if( initialEntry->getRow() < 10 ) {
				switch( position ) {
					case( 0 ):
						initial1 = initialEntry->getSelection();
						break;
					case( 1 ):
						initial2 = initialEntry->getSelection();
						break;
					case( 2 ):
						initial3 = initialEntry->getSelection();
						break;
				}
			}
			checkInput();
			break;
		case scoreScreen:
			//crossfades whatever was on the screen when the player died with the hi score screen
			if( (alpha -= 2) <= 0) {
				alpha = 0;
			}

			scoreTimer += 1;

			if( input.keyJustPressed( DIK_RETURN ) || ( scoreTimer > 500 ) ) {
				state = title;

				player.reset();
				levelMgr.reset();
				enemyMgr->reset();
				FXManager::Instance()->reset();
				PowerUpManager::Instance()->reset();

				alpha = 255;
				player.reset();
				initialEntry->resetPos();
				position = 0;
				initial1 = " ";
				initial2 = " ";
				initial3 = " ";
				scoreTimer = 0;

				// Play the intro music
				soundMgr->playBackgroundMusic( "audio/music/s_intro.mp3", true );
			}
			drawFrame();
			break;
		case victory:
			//crossfades whatever was on the screen before w/the credits
			if( (alpha -= 2) <= 0) {
				alpha = 0;
			}

			//update any remaining explosions
			FXManager::Instance()->update(lastFrameTime);

			if( input.keyJustPressed( DIK_RETURN ) || storyScroll <= -4900) {
				storyScroll = 632;
				alpha = 255;
				if( player.getScore() > topScores[4] ) {
					lastState = victory;
					state = initialsScreen;
				} else {
					state = scoreScreen;
					lastState = victory;
				}
			}
			storyScroll -= 1;
			timer.Pause(100);
			break;
		case instructions:
			if( input.keyJustPressed( DIK_RETURN ) ) {
				state = playing;
				// Start playing the audio for the first level
				soundMgr->playBackgroundMusic( "audio/music/s_level_one.mp3", true );
			}
			break;
	}
	
	drawFrame();
	input.refreshOldBuffer();
	
	//calc for next frame...
	lastFrameTime = timer.CalcFrameCount();
}

/*
 * checkCollision
 *
 * does all collision checks
 *
 */
void GameEngine::checkCollision() 
{
	//check to see if the player's colliding with the mainground
	levelMgr.getMainground()->checkBoundingBoxes(&player);

	// check to see if any enemies are colliding with the mainground
	std::vector<Enemy*> *enemies = enemyMgr->getActiveEnemies();
	std::vector<Enemy*>::iterator iter = enemies->begin();
	while(iter != enemies->end()) 
	{
		//dont check for collision with elements of the bos
		if(!(*iter)->isBossElement() || (*iter)->isGravityBombed()) {
			levelMgr.getMainground()->checkBoundingBoxes(*iter);
		}

		++iter;
	}

	//check for collision of player ammo against mainground
	Weapon **playerWeapons = player.getWeaponManager()->getWeapons();
	for(int i = 0; i < NUM_WEAPONS; ++i) {
		if(i == 3) {
			++i;
		}
		std::vector<BulletData> *ammo = playerWeapons[i]->getAmmoPos();
		std::vector<BulletData>::iterator iter = ammo->begin();

		while(iter != ammo->end()) {
         RECT *boundingBox = &((*iter).srcRect );

         //if there's a collision, remove this piece of ammo from the screen
			if(levelMgr.getMainground()->checkBoundingBoxes(boundingBox, playerWeapons[i]->getBoundingPoints(),
												            (*iter).position.x, (*iter).position.y)) {
				ammo->erase(iter);
			}else {
				++iter;
			}
		}
	}

	//do the same thing for the enemy's ammo
	Weapon **enemyWeapons = enemyMgr->getWeapons();
	for(int i = 0; i < NUM_ENEMY_WEAPONS; ++i) {
		std::vector<BulletData> *ammo = enemyWeapons[i]->getAmmoPos();
		std::vector<BulletData>::iterator iter = ammo->begin();

		while(iter != ammo->end()) {

         if ( (*iter).type == NORMAL )
         {
            RECT *boundingBox = &((*iter).srcRect);

            //if there's a collision, remove this piece of ammo from the screen
			   if(levelMgr.getMainground()->checkBoundingBoxes(boundingBox, enemyWeapons[i]->getBoundingPoints(),
				                                             (*iter).position.x, (*iter).position.y)) {
				   ammo->erase(iter);
			   } else {
				   ++iter;
			   }
         }
         else {
            ++iter;
         }
		}
	}
}

/*
 * checkInput
 *
 * checks the input of the user
 */
void GameEngine::checkInput() 
{
	int deltaX = 0;
	int deltaY = 0;
	switch( state ) {
		case playing:
			//check for player movement
			if(input.IsKeyDown( DIK_UP ) && input.IsKeyDown( DIK_DOWN ) ) {
				deltaY = 0;
				display->setOffset( 0 );
			} else if(input.IsKeyDown(DIK_UP))		
			{
				deltaY -= player.getSpeed();
				display->setOffset( 64 );
			} else if(input.IsKeyDown(DIK_DOWN)) 
			{
				deltaY += player.getSpeed();
				display->setOffset( 128 );
			} else {
				display->setOffset( 0 );
			}
			if( input.IsKeyDown( DIK_RIGHT ) && input.IsKeyDown( DIK_LEFT ) ) {
				deltaX = 0;
				display->setFlame( false );
			} else if(input.IsKeyDown(DIK_LEFT)) 
			{
				deltaX = 0 - player.getSpeed();
			} else if(input.IsKeyDown(DIK_RIGHT)) 
			{
				deltaX += player.getSpeed();
				display->setFlame( true );
			} else {
				display->setFlame( false );
			}
		   if(input.IsKeyDown(DIK_LCONTROL) || input.IsKeyDown(DIK_RCONTROL) )
			{
				player.fire();
			}
			// Update the player position
			player.setRelX(deltaX);
			player.setRelY(deltaY);
			break;
		case options:
			if( input.IsKeyDown( DIK_UP ) ) {
				optionMenu->move( 0 );
				timer.Pause( 500 );
			}
			if( input.IsKeyDown( DIK_RIGHT ) ) {
				switch( optionMenu->getRow() ) {
					case( 0 ):
						startingLives = ( startingLives + 1 ) % maxSLives;
						if( startingLives == 0 ) {
							startingLives = maxSLives;
						}
						break;
					case( 1 ):
						startingGBombs = ( startingGBombs + 1 ) % maxSGBombs;
						if( startingGBombs == 0 ) {
							startingGBombs = maxSGBombs;
						}
						break;
					case( 2 ):
						if( difficulty < 4.0 ) {
							difficulty = difficulty * diffMultiplier;
						} else if( difficulty == 4.0 ) {
							difficulty = 0.5f;
						}
						break;
				}
				timer.Pause( 500 );
			}
			if( input.IsKeyDown( DIK_DOWN ) ) {
				optionMenu->move( 2 );
				timer.Pause( 500 );
			}
			if( input.IsKeyDown( DIK_LEFT ) ) {
				switch( optionMenu->getRow() ) {
					case( 0 ):
						startingLives = ( startingLives + maxSLives - 1 ) % maxSLives;
						if( startingLives == 0 ) {
							startingLives = maxSLives;
						}
						break;
					case( 1 ):
						startingGBombs = ( startingGBombs + maxSGBombs - 1 ) % maxSGBombs;
						if( startingGBombs == 0 ) {
							startingGBombs = maxSGBombs;
						}
						break;
					case( 2 ):
						if( difficulty > 0.5 ) {
							difficulty = difficulty / diffMultiplier;
						} else if( difficulty == 0.5 ) {
							difficulty = 4.0f;
						}
						break;
				}
				timer.Pause( 500 );
			}
			break;
		case title:
			if( input.IsKeyDown( DIK_UP ) ) {
				titleMenu->move( 0 );
				timer.Pause( 500 );
			}
			if( input.IsKeyDown( DIK_RIGHT ) ) {
			}
			if( input.IsKeyDown( DIK_DOWN ) ) {
				titleMenu->move( 2 );
				timer.Pause( 500 );
			}
			if( input.IsKeyDown( DIK_LEFT ) ) {
			}
			break;
		case initialsScreen:
			if( input.IsKeyDown( DIK_UP ) ) {
				initialEntry->move( 0 );
				timer.Pause( 500 );
			}
			if( input.IsKeyDown( DIK_RIGHT ) ) {
				initialEntry->move( 1 );
				timer.Pause( 500 );
			}
			if( input.IsKeyDown( DIK_DOWN ) ) {
				initialEntry->move( 2 );
				timer.Pause( 500 );
			}
			if( input.IsKeyDown( DIK_LEFT ) ) {
				initialEntry->move( 3 );
				timer.Pause( 500 );
			}
			break;
   //    For weapon selection
   //if(input.IsKeyDown(DIK_1))
  // {
   //   player.selectWeapon(0);
  // }
  // if(input.IsKeyDown(DIK_2))
  // {
   //   player.selectWeapon(1);
  // }
  // if(input.IsKeyDown(DIK_3))
  // {
  //    player.selectWeapon(2);
  // }
  // if(input.IsKeyDown(DIK_4))
  // {
  //    player.selectWeapon(3);
   //}
   //if(input.IsKeyDown(DIK_5))
  // {
  //    player.selectWeapon(4);
  // }
   //if(input.IsKeyDown(DIK_0))
   //{
	  // if(showBins)
	  // {
		 //  showBins = false;
	  // }
	  // else
	  // {
		 //  showBins = true;
	  // }
   //}

	}
}

/*
 * drawFrame
 *
 * draws the current frame
 */
void GameEngine::drawFrame() 
{
	// clear out the device
	gameDX.GetDeviceHandle()->Clear( 0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0), 0.0f, 0);
	
	// begin scene... ?
	gameDX.GetDeviceHandle()->BeginScene();

	//start drawing sprites...
	spriteMgr->spriteBegin();
	int offsetX = 0;
	int offsetY = 0;
	switch(state) {
		case intro:
			introMgr->draw();
			break;
      case coreDying:
         offsetX = (rand() % 7) - 3;
			offsetY = (rand() % 7) - 3;
		case gameover:
		case playing:
		case paused:
		case gravityBombing:
			SpriteManager::Instance()->setOffsetX(offsetX);
			SpriteManager::Instance()->setOffsetY(offsetY);

            levelMgr.drawBgLayers();
			enemyMgr->drawWeapons();
			levelMgr.drawMgLayer();
			player.draw();
			enemyMgr->drawEnemies();
			PowerUpManager::Instance()->draw();
			levelMgr.drawFgLayers();

	        //sets offsets back to normal
			SpriteManager::Instance()->setOffsetX(0);
			SpriteManager::Instance()->setOffsetY(0);


			//HUD
			display->display();

			if(state == gameover) {
				display->gameOver();
			}

			if(showBins)
			{
				BinManager::Instance()->drawBins();
			}
            
			break;
		case title:
			titleMenu->draw();
			break;
		case options:
			optionMenu->draw();
			display->printSettings( startingLives, startingGBombs, difficulty );
			break;
		case story:
			display->printStory( storyScroll );
			break;
		case instructions:
			display->showInstructions();
			break;
		case initialsScreen:
			if(lastState == gameover) {
				//------draw the play field...--------
				levelMgr.drawBgLayers(alpha);
				enemyMgr->drawWeapons(alpha);
				levelMgr.drawMgLayer(alpha);
				player.draw(alpha);
				enemyMgr->drawEnemies(alpha);
				PowerUpManager::Instance()->draw(alpha);
				levelMgr.drawFgLayers(alpha);
				display->display(alpha);
				display->gameOver(alpha);
			}

			//------...crossfaded with intial entering high score screen----
			initialEntry->draw(255 - alpha);
			display->printInitials( initial1, initial2, initial3, (255 - alpha));
			break;
		case scoreScreen:
			if(lastState == gameover) {
				//------draw the play field...--------
				levelMgr.drawBgLayers(alpha);
				enemyMgr->drawWeapons(alpha);
				levelMgr.drawMgLayer(alpha);
				player.draw(alpha);
				enemyMgr->drawEnemies(alpha);
				PowerUpManager::Instance()->draw(alpha);
				levelMgr.drawFgLayers(alpha);
				display->display(alpha);
				display->gameOver(alpha);

			}

			display->printTopPlayers( topPlayers, topScores, 255 - alpha );
						
			break;
		case victory:
			//------draw the play field...--------
			levelMgr.drawBgLayers(alpha);
			enemyMgr->drawWeapons(alpha);
			levelMgr.drawMgLayer(alpha);
			player.draw(alpha);
			enemyMgr->drawEnemies(alpha);
			PowerUpManager::Instance()->draw(alpha);
			levelMgr.drawFgLayers(alpha);
			display->display(alpha);
			display->printCredits( storyScroll, 255 - alpha );
			break;
	}

	//...stop drawing sprites
	spriteMgr->spriteEnd();

	// display the frame rate
	//timer.DisplayFrameRate(gameDX.GetFont(), 20, 20);
	
	// display the player position...?
	//player.displayPosition(gameDX.GetFont());

	// end the scene
	gameDX.GetDeviceHandle()->EndScene();


	// begin a new scene for FX blending
	gameDX.GetDeviceHandle()->BeginScene();
	
	spriteMgr->spriteBegin();

	switch(state) 
		{
		case gravityBombing:
		case gameover:
		case paused:
		case coreDying:
		case playing:
		case victory:
			


			// setup blending
			gameDX.GetDeviceHandle()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			gameDX.GetDeviceHandle()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			
			SpriteManager::Instance()->setOffsetX(offsetX);
			SpriteManager::Instance()->setOffsetY(offsetY);

			// draw FX
			FXManager::Instance()->draw();

			SpriteManager::Instance()->setOffsetX(0);
			SpriteManager::Instance()->setOffsetY(0);
            
			break;
		}

	spriteMgr->spriteEnd();
			
	// end the scene
	gameDX.GetDeviceHandle()->EndScene();
            
	//flip to screen
	gameDX.GetDeviceHandle()->Present(NULL,NULL,NULL,NULL);

}

/*
 * initGameElements
 *
 * initializes all game elements (players, world, etc)
 *
 * returns status code (was this operation successful or not)
 */
HRESULT GameEngine::initGameElements() {
	HRESULT r;

	// build sin/cos tables
	LookUp::Instance()->buildCosTable();
	LookUp::Instance()->buildSinTable();
	
	// build bins
	BinManager::Instance()->initBins("graphics/binRect.dds", "graphics/binRect_O.dds");

	//loads the intro
	introMgr = new IntroManager("data/introscreens.txt"), 
	r = introMgr->loadTextures();
	if(FAILED(r)) {
		return r;
	}

	//load all textures associated w/player
	r = player.loadTextures();

	if(FAILED(r)) {
		return r;
	}

   // Create a new instance of the EnemyManager
   // and initialize it.
   enemyMgr = EnemyManager::Instance();
   enemyMgr->initEnemyManager( "map/level1/enemytriggers.txt" );

	// load texture for enemy(s)
	r = enemyMgr->loadTextures();

	if(FAILED(r))
	{
		return r;
	}

	// initialize power-ups
	PowerUpManager::Instance()->initPowerUpManager("map/level1/powerUpTriggers.txt");

	// load textures for power-ups
	r = PowerUpManager::Instance()->loadTextures();
	if(FAILED(r))
	{
		GameError("failed loading power-up textures");
		return r;
	}

	//loads the next level (in this case, the first level)
	if(!levelMgr.loadNextLevel()) {
		GameError("Error loading level");
		return S_FALSE;
	}

	titleMenuOptions = new char*[2];
	titleMenuOptions[0] = "START GAME";
	titleMenuOptions[1] = "OPTIONS";
	titleMenu = new Menu( 2, 1, 320, 272, 0, 64, titleMenuOptions, "graphics/title.dds" );
	titleMenu->init();

	optionMenuOptions = new char*[4];
	optionMenuOptions[0] = "LIVES";
	optionMenuOptions[1] = "GRAVITY BOMBS";
	optionMenuOptions[2] = "DIFFICULTY";
	optionMenuOptions[3] = "RETURN TO TITLE";
	optionMenu = new Menu( 4, 1, 200, 170, 0, 64, optionMenuOptions, "graphics/NameEntry.dds" );
	optionMenu->init();

	initial1 = " ";
	initial2 = " ";
	initial3 = " ";
	position = 0;

	topPlayers = new char*[5];
	topScores = new int[5];
	string temp;

	ifstream fileInput( "data/HighScores.txt" );
	for( int i = 0; i < 5; i++ ) {
		fileInput >> temp;
		topPlayers[i] = new char[3];
		strcpy( topPlayers[i], temp.c_str() );
		fileInput >> topScores[i];
	}
	fileInput.close();

	// make display manager
	display = new DisplayManager( &player, &levelMgr );
	display->init();
	display->setHighScore( topScores[0] );

	initials = new char*[110];
	initials[0] = "A";
	initials[1] = "B";
	initials[2] = "C";
	initials[3] = "D";
	initials[4] = "E";
	initials[5] = "F";
	initials[6] = "G";
	initials[7] = "H";
	initials[8] = "I";
	initials[9] = "J";
	initials[10] = "K";
	initials[11] = "L";
	initials[12] = "M";
	initials[13] = "N";
	initials[14] = "O";
	initials[15] = "P";
	initials[16] = "Q";
	initials[17] = "R";
	initials[18] = "S";
	initials[19] = "T";
	initials[20] = "U";
	initials[21] = "V";
	initials[22] = "W";
	initials[23] = "X";
	initials[24] = "Y";
	initials[25] = "Z";
	initials[26] = "a";
	initials[27] = "b";
	initials[28] = "c";
	initials[29] = "d";
	initials[30] = "e";
	initials[31] = "f";
	initials[32] = "g";
	initials[33] = "h";
	initials[34] = "i";
	initials[35] = "j";
	initials[36] = "k";
	initials[37] = "l";
	initials[38] = "m";
	initials[39] = "n";
	initials[40] = "o";
	initials[41] = "p";
	initials[42] = "q";
	initials[43] = "r";
	initials[44] = "s";
	initials[45] = "t";
	initials[46] = "u";
	initials[47] = "v";
	initials[48] = "w";
	initials[49] = "x";
	initials[50] = "y";
	initials[51] = "z";
	initials[52] = "1";
	initials[53] = "2";
	initials[54] = "3";
	initials[55] = "4";
	initials[56] = "5";
	initials[57] = "6";
	initials[58] = "7";
	initials[59] = "8";
	initials[60] = "9";
	initials[61] = "0";
	initials[62] = " ";
	initials[63] = "!";
	initials[64] = "\"";
	initials[65] = "#";
	initials[66] = "$";
	initials[67] = "%";
	initials[68] = "&";
	initials[69] = "'";
	initials[70] = "(";
	initials[71] = ")";
	initials[72] = "*";
	initials[73] = "+";
	initials[74] = ",";
	initials[75] = "-";
	initials[76] = ".";
	initials[77] = "/";
	initials[78] = ":";
	initials[79] = ";";
	initials[80] = "<";
	initials[81] = "=";
	initials[82] = ">";
	initials[83] = "?";
	initials[84] = "@";
	initials[85] = "[";
	initials[86] = "\\";
	initials[87] = "]";
	initials[88] = "^";
	initials[89] = "_";
	initials[90] = "`";
	initials[91] = "{";
	initials[92] = "|";
	initials[93] = "}";
	initials[94] = "~";
	initials[95] = " ";
	initials[96] = " ";
	initials[97] = " ";
	initials[98] = " ";
	initials[99] = " ";
	initials[100] = "BACKSPACE";
	initials[101] = "";
	initials[102] = "";
	initials[103] = "";
	initials[104] = "";
	initials[105] = "";
	initials[106] = "";
	initials[107] = "";
	initials[108] = "DONE";
	initials[109] = "";
	initialEntry = new Menu( 11, 10, 64, 200, 72, 36, initials, "graphics/NameEntry.dds" );
	initialEntry->init();

	startingLives = 3;
	maxSLives = 5;
	startingGBombs = 2;
	maxSGBombs = 3;
	difficulty = 1.0;
	diffMultiplier = 2;

	storyScroll = 632;

	player.setStartingValues( startingLives, startingGBombs, difficulty );
	player.reset();
	enemyMgr->setDifficulty( difficulty );

	FXManager::Instance()->loadTextures();

	// Initialize the sound elements
	soundMgr = SoundManager::Instance();
	soundMgr->initialize( 44000, 32, 0);

	// Play the intro music
	soundMgr->playBackgroundMusic( "audio/music/s_intro.mp3", true );

	return S_OK;
}
