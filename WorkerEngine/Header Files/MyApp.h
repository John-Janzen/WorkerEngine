#pragma once
#include <list>

#include "Application.h"

class MyApp : public Application
{
public:
	MyApp();
	~MyApp();

	void Init(int n);

	void Update(SDL_Event * s);

private:
	int countedFrames = 0;
	const int SCREEN_FPS = 60;
	const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
};

