#pragma once
#include <SDL.h>


class JTime
{
public:

	static JTime& instance()
	{
		static JTime jt;
		return jt;
	}

	void CalcDeltaTime()
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = ((NOW - LAST) / (float)SDL_GetPerformanceFrequency());
	}

	float deltaTime;
	Uint64 LAST = 0, NOW;

	~JTime() {}
private:
	JTime() {}

};
