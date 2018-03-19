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

	/*
	* Renders what is to be put on screen
	* SwapsWindows at the very end
	*/
	void RenderWindow(BaseContent* ptr);

	/*
	* Swaps the cyan color and the multicolor flag
	*/
	void SwapColor();

	void handleCamera(GameObject* ptr);
	
protected:
	SDL_Window* _window = NULL;
	SDL_GLContext _context;

	GLuint r_ProgramID = 0;
	GLint r_VertexPos2DLocation = -1;
	GLuint r_VBO = 0;
	GLuint r_VAO = 0;
	GLuint r_EBO = 0;

	GLint render_projection_matrix_loc;
	GLint render_model_matrix_loc;

	bool _renderQuad = true;

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	Color _ColorMode = Color::COLOR_MODE_CYAN;
};
