#include "timecontrol.h"

TimeControl::TimeControl()
{
	LARGE_INTEGER	li;
	QueryPerformanceFrequency(&li);
	freq = (float) li.LowPart;

	last_ms_check = 0;

	state	= TC_STOP;
}

void TimeControl::Start()
{
	LARGE_INTEGER	li;
	QueryPerformanceCounter(&li);
	counter_base = li.QuadPart;

	time_elapsed = 0;
	state = TC_PLAY;
}

float TimeControl::GetTime()
{
	LARGE_INTEGER	li;
	LARGE_INTEGER	tacts_elapsed;

	QueryPerformanceCounter(&li);

	tacts_elapsed.QuadPart = li.QuadPart - counter_base;
	if (state == TC_PLAY)
		time_elapsed += (float) ((/*(float)*/tacts_elapsed.QuadPart) / freq);
	counter_base = li.QuadPart;

	return time_elapsed;
}

float TimeControl::CheckFrameMS()
{
	float time = GetTime();
	float ret_ms = time - last_ms_check;
	last_ms_check = time;

	return ret_ms;
}

void TimeControl::Pause()
{
	state = TC_PAUSE;
}

void TimeControl::Resume()
{
	state = TC_PLAY;
}