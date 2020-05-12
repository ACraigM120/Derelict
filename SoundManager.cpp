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

#include "SoundManager.h"

/*
 * Constructor
 * Creates a new instance of the SoundManager
 *
 * @returns - None
 */
SoundManager::SoundManager()
{ 
   m_currentSong = 0;
}

/*
 * Destructor
 * Perform cleanup
 *
 * @returns - None
 */
SoundManager::~SoundManager()
{
   // Close the current song.
   FSOUND_Stream_Close( m_currentSong );
   m_currentSong = 0;

   // Close all FMOD related stuff
   FSOUND_Close();
}


/*
 * Returns the Singleton instance of the SoundManager object. 
 */
SoundManager* SoundManager::Instance()
{
   //instead of 'new'ing 'this' and returning the pointer, just return the address. this way we dont
   //have to worry about explicitly calling the destructor of 'this'
   static SoundManager soundMgr;
   return &soundMgr; 
}


/*
 * Calls the FMOD Initialize method with the passed
 * in parameters.  If this fails, it will log an error
 * and exit.
 *
 * @param - mixRate - The audio output mix rate in Hz.w
 * @param - maxSWChannels - The maximum number of software channels to use.
 * @param - fmodFlags - Misc FMOD specific flags that can be passed.
 * @returns - None
 */
void SoundManager::initialize( int mixRate, int maxSWChannels, int fmodFlags )
{
   if (!FSOUND_Init(mixRate, maxSWChannels, fmodFlags ))
   {
      GameError( strcat("Error on FMOD ", FMOD_ErrorString(FSOUND_GetError()) ) );
   }
}


/*
 * Plays the passed in audio file.
 *
 * @param - effectFilename - The sound effect file to play
 * @returns - None
 */
void SoundManager::playSoundEffect( char* effectFilename )
{
   //Create a new sample
   FSOUND_SAMPLE *m_sample;
   m_sample = NULL;

   //Load the sample - sound
   m_sample = FSOUND_Sample_Load(FSOUND_FREE, effectFilename, FSOUND_NORMAL | FSOUND_2D, 0, 0);
 
   //Play the sound
   int m_channel = FSOUND_PlaySound(FSOUND_FREE, m_sample);
}


/*
 * Plays the passed in audio file.
 *
 * @param - songFilename - The song file to play
 * @returns - None
 */
void SoundManager::playBackgroundMusic( char* songFilename, bool loop )
{
   // If there is a song currently playing, stop it.
   if ( m_currentSong != 0 )
   {
      FSOUND_Stream_Close( m_currentSong );
   }

   //Open the stream
   m_currentSong = FSOUND_Stream_Open( songFilename, FSOUND_NORMAL | FSOUND_2D, 0, 0 );
 
   //Loop the sound
   if ( loop )
   {
      FSOUND_Stream_SetMode( m_currentSong, FSOUND_LOOP_NORMAL );
   }
   else {
      FSOUND_Stream_SetMode( m_currentSong, FSOUND_LOOP_OFF );
   }

   //Play the sound
   int m_channel = FSOUND_Stream_Play( FSOUND_FREE, m_currentSong );
}

/*
 * Pauses all sound/music.
 *
 * @returns - None
 */
void SoundManager::pauseMusic()
{
   FSOUND_SetPaused( FSOUND_ALL, TRUE );
}

/*
 * Resumes all sound/music
 *
 */
void SoundManager::resumeMusic()
{
   FSOUND_SetPaused( FSOUND_ALL, FALSE );
}
