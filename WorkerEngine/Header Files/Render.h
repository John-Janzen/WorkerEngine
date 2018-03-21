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

#include "System.h"
#include "Manager.h"

enum Color {
	COLOR_MODE_CYAN,
	COLOR_MODE_MULTI
};

static const GLfloat vertex_positions[] = {
	-1.0f, -1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  0.0f, 1.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 1.0f,
};
//
//// Color for each vertex
//static const GLfloat vertex_colors[] = {
//	1.0f, 1.0f, 1.0f, 1.0f,
//	1.0f, 1.0f, 0.0f, 1.0f,
//	1.0f, 0.0f, 1.0f, 1.0f,
//	0.0f, 1.0f, 1.0f, 1.0f
//};
//
//// Indices for the triangle strips
//static const GLushort vertex_indices[] = {
//	0, 1, 2
//};

class Render : public System
{
public:
	Render();
	~Render();

	/*
	* Update function that checks the job type for what function to call
	*/
	virtual void Update(JOB_TYPES T, BaseContent* ptr = nullptr);

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

	void InitObject(void * ptr);

	/*
	* Renders what is to be put on screen
	* SwapsWindows at the very end
	*/
	void RenderWindow(BaseContent* ptr);

	void RenderObject(GameObject * go);

	/*
	* Swaps the cyan color and the multicolor flag
	*/
	void SwapColor();

	void handleCamera(GameObject* ptr);

	
protected:
	SDL_Window* _window = NULL;
	SDL_GLContext _context;

	GLuint r_ProgramID = 0;

	GLint render_projection_matrix_loc;
	GLint render_model_matrix_loc;
	glm::mat4 projection_look_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 look_matrix;

	GLuint _VBO = 0;
	GLuint _VAO = 0;
	GLuint _EBO = 0;



	bool _renderQuad = true;

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	Color _ColorMode = Color::COLOR_MODE_CYAN;
};
