#include "MyApp.h"

/*===================================================================================*//**

WorkerEngine

A Prototype Engine made using the worker and manager design pattern for
multithreading. This should allow the game to scale with higher level cpu's.

Copyright 2018 John Janzen. All rights reserved.
https://github.com/John-Janzen

@author John Janzen
@version 0.0.07
@file Main.cpp

*//*====================================================================================*/
int main(int argc, char *argv[]) 
{
	uint16_t num = std::thread::hardware_concurrency() / 2;
	MyApp * _myApp = new MyApp(num);
	
	while (!_myApp->Update());				// Game Loop
	
	Manager::instance().Close();	// Close the game
	delete(_myApp);

	//_myApp.Close();				
	SDL_Quit();						// Close SDL

	printf("System Closed");
	return NULL;
}