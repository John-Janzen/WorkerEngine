#pragma once

#include "Application.h"
#include "Quad.h"

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

	void ShutDown();

	/*
	* Initializes the Application with threads + systems
	* Calls Application::Init()
	*/
	void Init(uint16_t n);

	/*
	* Updates the game and specific functions
	*/
	bool Update();

	bool ReadInputs();

private:
	const Uint32 SCREEN_FPS = 60;
	const Uint32 SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
	Uint32 now, frameTicks;
	STATE state = LOADING;
	bool _flag = false;
};

