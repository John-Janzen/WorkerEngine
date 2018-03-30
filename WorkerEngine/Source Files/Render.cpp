#include "Render.h"

GLuint loadShaderFromFile(std::string path, GLenum shaderType)
{
	GLuint shaderID = 0;
	std::string shaderString;
	std::ifstream sourceFile(path.c_str());
	if (sourceFile.is_open())
	{
		shaderString.assign((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());
		shaderID = glCreateShader(shaderType);

		const GLchar* shaderSource = shaderString.c_str();
		glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);

		glCompileShader(shaderID);

		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE)
		{
			printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
			glDeleteShader(shaderID);
			shaderID = 0;
		}
	}
	else
	{
		printf("Unable to open file %s\n", path.c_str());
	}
	return shaderID;
}

Render::Render(Scheduler * sch) : _scheduler{sch} {}

Render::~Render() {}

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

	GLuint vertexShader = loadShaderFromFile("Assets/shader.glvs", GL_VERTEX_SHADER);

	if (vertexShader == 0)
	{
		printf("Vertex Shader went wrong");
	}

	glAttachShader(r_ProgramID, vertexShader);

	GLuint fragmentShader = loadShaderFromFile("Assets/shader.glfs", GL_FRAGMENT_SHADER);

	if (fragmentShader == 0)
	{
		printf("Fragment Shader went wrong");
	}

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
		color_vec_loc = glGetUniformLocation(r_ProgramID, "color_vec");

		projection_matrix = glm::perspective(glm::radians(60.0f), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 1.0f, 500.0f);
		look_matrix = glm::lookAtRH(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		SDL_GL_SwapWindow(_window);
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Render::InitObject(void * ptr)
{
	RenderLoadContent * RUContent = static_cast<RenderLoadContent*>(ptr);
	RenderComponent * rc;
	_camera = RUContent->camera;

	for (GameObject * go : *RUContent->objects)
	{
		if ((rc = static_cast<RenderComponent*>(go->getComponent("render"))) != nullptr)
		{
			rc->Initalize();
		}
	}
}

void Render::RenderWindow(BaseContent* ptr)
{
	glm::mat4 translate;
	RenderUpdateContent * RUContent = static_cast<RenderUpdateContent*>(ptr);
	for (GameObject * go : *RUContent->objects)
	{
		if (go->getName().compare("Camera") == 0)
			translate = glm::translate(glm::mat4(), go->getPos());
	}
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(r_ProgramID);

	projection_look_matrix = projection_matrix * (look_matrix * translate);
	glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_look_matrix));

	for (GameObject * go : *RUContent->objects)
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

	GLsizei num = rc->BindBuffers();
	//glUniform1i(tex_unit_loc, 0);

	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), go->getPos());
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniform4f(color_vec_loc, go->color.x, go->color.y, go->color.z, go->color.w);
	glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}
