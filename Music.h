#pragma once

#include <windows.h>
#include "bass.h"

class Music
{
private:
	HSTREAM mp3Strewam;

public:
	Music(void);
	~Music(void);

	void LoadMusic(const char *filename);
	void Play();
	void Stop();
	void SetPosition(float time);
	float GetPosition();
};
