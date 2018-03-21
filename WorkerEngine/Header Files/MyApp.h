#pragma once

#include "Application.h"

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

enum STATE
{
	LOADING,
	UPDATE
};

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
	Uint32 now, frameTicks;
	STATE state = LOADING;
};

