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

	void setTextures(const GLfloat * arr) { _textures = arr; }
	void setVertices(const GLfloat * arr) { _vertices = arr; }
	void setNormals(const GLfloat * arr) { _normals = arr; }
	void setFaces(const GLuint * arr) { _faces = arr; }

	const GLfloat * getTextures() { return _textures; }
	const GLfloat * getNormals() { return _normals; }
	const GLfloat * getVertices() { return _vertices; }
	const GLuint * getFaces() { return _faces; }

	GLuint _VBO = 0;
	GLuint _VAO = 0;
	GLuint _EBO = 0;
	size_t numFaces;
	size_t numVertices;
private:
	const GLfloat * _textures;
	const GLfloat * _vertices;
	const GLfloat * _normals;
	const GLuint * _faces;
	

};

