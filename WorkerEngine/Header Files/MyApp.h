#pragma once

#include "Application.h"
#include "Quad.h"

class MyApp : public Application
{
public:
	MyApp(const size_t & n);
	~MyApp();

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
	std::shared_ptr<Job> localJob;
};

