#pragma once
#include <GL\glew.h>
#include "Component.h"
class RenderComponent : public Component
{
public:
	RenderComponent() {}
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
	size_t numNormals;
	size_t numTextures;
private:
	const GLfloat * _vertices;
	const GLuint * _indices;
};

