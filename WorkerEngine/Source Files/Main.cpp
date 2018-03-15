#include <SDL.h>

#include "MyApp.h"

int main(int argc, char *argv[]) 
{
	int num = 2;
	MyApp _myApp;
	bool dead = false;
	SDL_Event * e;

	_myApp.Init(num);
	
	while (!dead)
	{
		e = new SDL_Event();
		while (SDL_PollEvent(e))
		{
			switch (e->type)
			{
			case SDL_QUIT:
				dead = true;
				break;
			default:
				break;
			}
		}
		_myApp.Update(e);
	}
	
	_myApp.Close();
	SDL_Quit();

	printf("System Closed");
	getchar();
	return NULL;
}