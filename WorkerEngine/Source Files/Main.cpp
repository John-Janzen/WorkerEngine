#include "MyApp.h"

/*===================================================================================*//**

WorkerEngine

A Prototype Engine made using the worker and manager design pattern for
multithreading. This should allow the game to scale with higher level cpu's.

Copyright 2017 John Janzen. All rights reserved.
https://github.com/John-Janzen

@author John Janzen
@version 0.0.03
@file Main.cpp

*//*====================================================================================*/
int main(int argc, char *argv[]) 
{
	int num = 3;
	MyApp _myApp;
	bool dead = false;
	SDL_Event e;

	_myApp.Init(num);
	
	while (!dead)				// Game Loop
	{
		while (SDL_PollEvent(&e))// Listen for events
		{
			switch (e.type)
			{
			case SDL_QUIT:		// Quit the game
				dead = true;
				break;
			case SDL_KEYDOWN:
				if (e.key.repeat == 0)
					Manager::instance().addJob("Input", JOB_TYPES::INPUT_READ_PRESSED, new InputContent(&e));	// Send Job if Event is changed
				
				break;
			default:
				break;
			}
		}
		Manager::instance().addJob("Input", JOB_TYPES::INPUT_READ_CONTINUOUS);	// Read held keys
		_myApp.Update();		// Update the game
	}
	
	Manager::instance().Close();	// Close the game
	//_myApp.Close();				
	SDL_Quit();						// Close SDL

	printf("System Closed");
	getchar();
	return NULL;
}