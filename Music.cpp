#include "Music.h"
#include <assert.h>

Music::Music(void)
{
	mp3Strewam = NULL;
}

Music::~Music(void)
{
}

void Music::LoadMusic(const char *filename)
{
	BOOL results;
	results = BASS_Init(-1, 44100, 0, NULL, NULL);
	if (results)
	{
		mp3Strewam = BASS_StreamCreateFile(false, filename, 0, 0, 0);
	}
}

void Music::Play()
{
	BASS_ChannelPlay(mp3Strewam, false);
}

void Music::Stop()
{
	if (mp3Strewam != NULL)
		BASS_StreamFree(mp3Strewam);

	BASS_Free();
}

void Music::SetPosition(float time)
{
	QWORD bytesPos = BASS_ChannelSeconds2Bytes(mp3Strewam, time);
	BOOL result = BASS_ChannelSetPosition(mp3Strewam, bytesPos, BASS_POS_BYTE);

	if (result != TRUE)
	{
		int errorCode = BASS_ErrorGetCode();
		assert(false);
	}
}

float Music::GetPosition()
{
	QWORD bytesPos = BASS_ChannelGetPosition(mp3Strewam, BASS_POS_BYTE);
	return (float)BASS_ChannelBytes2Seconds(mp3Strewam, bytesPos);
}

