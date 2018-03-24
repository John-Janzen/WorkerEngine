#pragma once
#include <GL\glew.h>
#include "Component.h"
class RenderComponent : public Component
{
public:
	RenderComponent(const GLfloat * arr = nullptr, const GLuint * arr2 = nullptr, size_t ind_size = 0, size_t vert_size = 0) 
		: _vertices (arr), _indices(arr2), numInd(ind_size), numVertices(vert_size) {}
	~RenderComponent() 
	{
		glDeleteVertexArrays(1, &_VAO);
		glDeleteBuffers(1, &_VBO);
	}
	void setVertices(const GLfloat * arr) { _vertices = arr; }
	void setIndices(const GLuint * arr) { _indices = arr; }

	const GLfloat * getVertices() { return _vertices; }
	const GLuint * getIndices() { return _indices; }

	GLuint _VBO = 0;
	GLuint _VAO = 0;
	GLuint _EBO = 0;
	size_t numInd;
	size_t numVertices;
private:
	const GLfloat * _vertices;
	const GLuint * _indices;
};

