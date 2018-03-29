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
	uint16_t num = std::thread::hardware_concurrency() / 2;
	MyApp _myApp;
	SDL_Event e;

	_myApp.Init(num);
	
	while (!_myApp.Update());				// Game Loop
	
	Manager::instance().Close();	// Close the game
	//_myApp.Close();				
	SDL_Quit();						// Close SDL

	printf("System Closed");
	getchar();
	return NULL;
}