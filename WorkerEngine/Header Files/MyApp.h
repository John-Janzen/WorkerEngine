#pragma once
#include <list>

#include "Application.h"

class MyApp : public Application
{
public:
	MyApp();
	~MyApp();

	/*
	* Initializes the Application with threads + systems
	* Calls Application::Init()
	*/
	void Init(int n);

	/*
	* Updates the game and specific functions
	*/
	void Update(SDL_Event * s);

private:
	const int SCREEN_FPS = 60;
	const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
};

