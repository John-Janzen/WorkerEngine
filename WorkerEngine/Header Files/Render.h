#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\freeglut.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include <stdio.h>
#include <string>

#include "System.h"

enum Color {
	COLOR_MODE_CYAN,
	COLOR_MODE_MULTI
};

class Render : public System
{
public:
	Render();
	~Render();

	/*
	* Update function that checks the job type for what function to call
	*/
	virtual void Update(JOB_TYPES T, void* ptr = nullptr);

	/*
	* Deallocates SDL and OpenGL
	*/
	virtual void Close();

	/*
	* Initializes SDL and OpenGL
	* This should be called on the Main thread
	*/
	void Init();

	/*
	* Initializes OpenGL Window and Rendering
	*/
	void InitGL();

	/*
	* Renders what is to be put on screen
	* SwapsWindows at the very end
	*/
	void RenderWindow();

	/*
	* Swaps the cyan color and the multicolor flag
	*/
	void SwapColor();
	
protected:
	SDL_Window* _window = NULL;
	SDL_GLContext _context;

	bool _renderQuad = true;

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	Color _ColorMode = Color::COLOR_MODE_CYAN;
};

