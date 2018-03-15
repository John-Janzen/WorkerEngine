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

	virtual void Update(JOB_TYPES T, void* ptr = nullptr);
	virtual void Close();
	void Init();

	void InitGL();
	void RenderWindow();

	void SwapColor();

	//void loadMedia();
	
protected:
	SDL_Window* _window = NULL;
	SDL_GLContext _context;

	bool _renderQuad = true;

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	Color _ColorMode = Color::COLOR_MODE_CYAN;
};

