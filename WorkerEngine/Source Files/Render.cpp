#include "Render.h"

Render::Render() {}

Render::~Render() {}

void Render::Update(JOB_TYPES T, BaseContent* ptr)
{
	Manager::instance().signalWorking();
	switch (T)
	{
	case SYSTEM_DEFAULT:
		break;
	case RENDER_LOAD:
		Init();
		InitGL();
		break;
	case RENDER_UPDATE:
		RenderWindow(ptr);
		break;
	case SWAP_COLOR:
		SwapColor();
		break;
	default:
		break;
	}
	if (ptr != nullptr)
		delete(ptr);
	Manager::instance().signalDone();
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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		
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
	r_ProgramID = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertexShaderSource[] =
	{
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
	}
	else
	{
		glAttachShader(r_ProgramID, vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar* fragmentShaderSource[] = 
		{
			"#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 0.0, 1.0 ); }"
		};

		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile fragment shader %d!\n", fragmentShader);
		}
		else
		{
			glAttachShader(r_ProgramID, fragmentShader);

			glLinkProgram(r_ProgramID);

			GLint programSuccess = GL_TRUE;
			glGetProgramiv(r_ProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", r_ProgramID);
			}
			else
			{
				r_VertexPos2DLocation = glGetAttribLocation(r_ProgramID, "LVertexPos2D");
				if (r_VertexPos2DLocation == -1)
				{
					printf("LVertex is not a valid glsl program variable!\n");
				}
				else 
				{
					glViewport(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT);

					glMatrixMode(GL_PROJECTION);

					glLoadIdentity();
					glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f, -100.0f);

					glMatrixMode(GL_MODELVIEW);
					glPopMatrix();
					glLoadIdentity();

					glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

					GLfloat vertexData[] = 
					{
						-0.5f, -0.5f,
						0.5f, -0.5f,
						0.5f, 0.5f,
						-0.5f, 0.5f
					};

					GLuint indexData[] = { 0, 1, 2, 3 };

					glGenBuffers(1, &r_VBO);
					glBindBuffer(GL_ARRAY_BUFFER, r_VBO);
					glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

					glGenBuffers(1, &r_IBO);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_IBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

					glPushMatrix();
				}
			}
		}
	}
}

void Render::RenderWindow(BaseContent* ptr)
{
	RenderUpdateContent * RUContent = static_cast<RenderUpdateContent*>(ptr);
	for (GameObject * go : RUContent->objects)
	{
		if (go->getName().compare("Camera") == 0)
		{
			handleCamera(go);
			break;
		}
	}
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(r_ProgramID);
	glEnableVertexAttribArray(r_VertexPos2DLocation);

	glBindBuffer(GL_ARRAY_BUFFER, r_VBO);
	glVertexAttribPointer(r_VertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_IBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(r_VertexPos2DLocation);

	glUseProgram(NULL);

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

void Render::handleCamera(GameObject * camera)
{
	glm::vec3 pos = camera->getPos();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glLoadIdentity();
	glTranslatef(pos.x, pos.y, pos.z);

	glPushMatrix();
}
