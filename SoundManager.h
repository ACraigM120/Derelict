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

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "fmod.h"
#include "fmod_errors.h"
#include "GameCore.h"

class SoundManager
{
public:
   // Returns a singleton instance of the SoundManager class
   static SoundManager* Instance();

   // Some FMOD wrapper methods for playing music and sound effects.
   void initialize( int mixRate, int maxSWChannels, int fmodFlags );
   void playSoundEffect( char* effectFilename );
   void playBackgroundMusic( char* songFilename, bool loop );
   void pauseMusic();
   void resumeMusic();

protected:
   // Protected Constructor/Destructor.
   SoundManager();
   ~SoundManager();

private:
   FSOUND_STREAM *m_currentSong;       // The currently loaded/playing audio file.
};

#endif
