#pragma once

#include "Application.h"
#include "Quad.h"

class MyApp : public Application
{
public:
	MyApp();
	~MyApp();

	/*
	* Initializes the Application with threads + systems
	* Calls Application::Init()
	*/
	void Init(uint16_t n);

	/*
	* Updates the game and specific functions
	*/
	bool Update();

	void setNextScene(const SCENE & s);

private:
	const Uint32 SCREEN_FPS = 60;
	const Uint32 SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
	Uint32 now, frameTicks;
	Uint32 loadBegin = 0, loadEnd;
	bool _flag = false;
	STATE state = LOADING;
	
};

