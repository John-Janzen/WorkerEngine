#include "Render.h"

Render::Render() {}

Render::~Render() {}

void Render::Update(JOB_TYPES T, void* ptr)
{
	switch (T)
	{
	case SYSTEM_DEFAULT:
		break;
	case RENDER_LOAD:
		Init();
		InitGL();
		break;
	case RENDER_UPDATE:
		RenderWindow();
		break;
	case SWAP_COLOR:
		SwapColor();
		break;
	default:
		break;
	}
}

void Render::Close()
{
	SDL_DestroyWindow(_window);
	_window = NULL;
}

void Render::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL not Initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		
		_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (_window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get window surface
			_context = SDL_GL_CreateContext(_window);
			if (_context == NULL)
			{
				printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL_Error: %s\n", SDL_GetError());
				}
			}
		}
	}
}

void Render::InitGL()
{
	GLenum error = GL_NO_ERROR;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing 1 OpenGL! %s\n", gluErrorString(error));
	}

	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing 2 OpenGL! %s\n", gluErrorString(error));
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.f);

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing 3 OpenGL! %s\n", gluErrorString(error));
	}
}

void Render::RenderWindow()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.f);

	if (_ColorMode == Color::COLOR_MODE_CYAN)
	{
		glBegin(GL_QUADS);
			glColor3f(0.f, 1.f, 1.f);
			glVertex2f(-50.f, -50.f);
			glVertex2f(50.f, -50.f);
			glVertex2f(50.f, 50.f);
			glVertex2f(-50.f, 50.f);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
			glColor3f(1.f, 0.f, 0.f); glVertex2f(-50.f, -50.f);
			glColor3f(1.f, 1.f, 0.f); glVertex2f(50.f, -50.f);
			glColor3f(0.f, 1.f, 0.f); glVertex2f(50.f, 50.f);
			glColor3f(0.f, 0.f, 1.f); glVertex2f(-50.f, 50.f);
		glEnd();
	}

	SDL_GL_SwapWindow(_window);
}

void Render::SwapColor()
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	if (_ColorMode == COLOR_MODE_CYAN)
	{
		_ColorMode = COLOR_MODE_MULTI;
	}
	else
	{
		_ColorMode = COLOR_MODE_CYAN;
	}
	_c.notify_one();
}
