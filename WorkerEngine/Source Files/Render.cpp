#include "Render.h"

Render::Render(Application * a) : System(a) {}

Render::~Render() { Close(); }

void Render::Update(JOB_TYPES T, bool & flag, BaseContent* ptr)
{
	//Manager::instance().signalWorking();
	switch (T)
	{
	case SYSTEM_DEFAULT:
		break;
	case RENDER_INIT:
		Init();
		InitGL();
		break;
	case RENDER_LOAD:
		InitObject(ptr);
		break;
	case RENDER_UPDATE:
		RenderWindow(ptr);
		break;
	default:
		break;
	}
	if (ptr != nullptr)
		delete(ptr);
	//Manager::instance().signalDone();
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
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		
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
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					printf("Error initializing GLEW! %s", glewGetErrorString(glewError));
				}

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
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	projection_matrix = glm::perspective(glm::radians(60.0f), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 1.0f, 500.0f);
	look_matrix = glm::lookAtRH(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Render::LoadingView()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_GL_SwapWindow(_window);
}

void Render::DoneLoading()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void Render::InitObject(BaseContent * ptr)
{
	RenderLoadContent * RUContent = static_cast<RenderLoadContent*>(ptr);
	RenderComponent * rc;
	_camera = RUContent->camera;

	for (GameObject * go : RUContent->objects)
	{
		if ((rc = static_cast<RenderComponent*>(go->getComponent("render"))) != nullptr)
		{
			rc->Initalize();
		}
	}
}

void Render::RenderWindow(BaseContent* ptr)
{
	RenderUpdateContent * RUContent = static_cast<RenderUpdateContent*>(ptr);
	glm::mat4 model_matrix, rotation;
	rotation = glm::rotate(rotation, RUContent->camera->getRot().z, glm::vec3(0, 0, 1));
	rotation = glm::rotate(rotation, RUContent->camera->getRot().x, glm::vec3(1, 0, 0));
	rotation = glm::rotate(rotation, RUContent->camera->getRot().y, glm::vec3(0, 1, 0));
	model_matrix = glm::translate(rotation, RUContent->camera->getPos());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projection_look_matrix = projection_matrix * (look_matrix * model_matrix);

	for (GameObject * go : RUContent->objects)
		go->DrawMyself(glm::value_ptr(projection_look_matrix));

	SDL_GL_SwapWindow(_window);
}
