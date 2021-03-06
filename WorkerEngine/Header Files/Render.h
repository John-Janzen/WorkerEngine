#pragma once
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <gtc\type_ptr.hpp>
#include <stdio.h>
#include <string>
#include <map>
#include <fstream>

#include "System.h"

enum Color {
	COLOR_MODE_CYAN,
	COLOR_MODE_MULTI
};

class Render : public System
{
public:
	Render(Application * a);
	~Render();

	/*
	* Update function that checks the job type for what function to call
	*/
	virtual void Update(JOB_TYPES T, bool & flag, BaseContent* ptr = nullptr);

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

	void LoadingView();

	void DoneLoading();

	void InitObject(BaseContent * ptr);

	/*
	* Renders what is to be put on screen
	* SwapsWindows at the very end
	*/
	void RenderWindow(BaseContent* ptr);
	
protected:
	SDL_Window* _window = NULL;
	SDL_GLContext _context;

	glm::mat4 projection_look_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 look_matrix;

	GLuint _VBO = 0;
	GLuint _VAO = 0;
	GLuint _EBO = 0;

	GameObject * _camera;

	bool _renderQuad = true;

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	Color _ColorMode = Color::COLOR_MODE_CYAN;
};
