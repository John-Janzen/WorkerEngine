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
		InitObject(ptr);
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
	glUseProgram(0);
	glDeleteProgram(r_ProgramID);

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
	r_ProgramID = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertexShaderSource[] =
	{
		"#version 330\nuniform mat4 model_matrix; uniform mat4 projection_matrix; layout(location = 0) in vec4 position; layout(location = 1) in vec4 color; out vec4 vs_fs_color; void main(void) { vs_fs_color = vec4(1.0, 1.0, 1.0, 1.0); gl_Position = projection_matrix * (model_matrix * position); } "
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
			"#version 330\nin vec4 vs_fs_color; layout(location = 0) out vec4 color; void main(void) { color = vs_fs_color; } "
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
				glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
				glUseProgram(r_ProgramID);
				render_model_matrix_loc = glGetUniformLocation(r_ProgramID, "model_matrix");
				render_projection_matrix_loc = glGetUniformLocation(r_ProgramID, "projection_matrix");

				projection_matrix = glm::perspective(glm::radians(60.0f), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 1.0f, 500.0f);
				look_matrix = glm::lookAtRH(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
			}
		}
	}
}

void Render::InitObject(void * ptr)
{
	RenderUpdateContent * RUContent = static_cast<RenderUpdateContent*>(ptr);
	RenderComponent * rc;
	for (GameObject * go : RUContent->objects)
	{
		if ((rc = static_cast<RenderComponent*>(go->getComponent("render"))) != nullptr)
		{
			glGenBuffers(1, &rc->_EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc->_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(GLuint) * rc->numFaces), rc->getFaces(), GL_STATIC_DRAW);

			glGenVertexArrays(1, &rc->_VAO);
			glBindVertexArray(rc->_VAO);

			glGenBuffers(1, &rc->_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, rc->_VBO);
			glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * rc->numVertices), rc->getVertices(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
	}
}

void Render::RenderWindow(BaseContent* ptr)
{
	glm::mat4 translate;
	RenderUpdateContent * RUContent = static_cast<RenderUpdateContent*>(ptr);
	for (GameObject * go : RUContent->objects)
	{
		if (go->getName().compare("Camera") == 0)
			translate = glm::translate(glm::mat4(), go->getPos());
	}
	
	//glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(r_ProgramID);

	projection_look_matrix = projection_matrix * (look_matrix * translate);
	glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_look_matrix));

	for (GameObject * go : RUContent->objects)
	{
		if (go->getComponent("render") != nullptr)
			RenderObject(go);
	}

	glBindVertexArray(0);
	SDL_GL_SwapWindow(_window);
}

void Render::RenderObject(GameObject * go)
{
	RenderComponent * rc = static_cast<RenderComponent*>(go->getComponent("render"));
	glBindVertexArray(rc->_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc->_EBO);

	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), go->getPos());
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, rc->numFaces / 9, GL_UNSIGNED_INT, NULL);
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

}
