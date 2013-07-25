#pragma once

#include <windows.h>

class TimeControl
{
private:
	LONGLONG counter_base;
	float time_elapsed;
	float last_ms_check;
	float freq;

	BYTE state;

public:
	enum
	{
		TC_PLAY,
		TC_PAUSE,
		TC_STOP
	};

	TimeControl();

	void Start();
	void Pause();
	void Resume();

	float GetTime();
	float CheckFrameMS();
};