#pragma once

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
	void Update();

private:
	const Uint32 SCREEN_FPS = 60;
	const Uint32 SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
};

