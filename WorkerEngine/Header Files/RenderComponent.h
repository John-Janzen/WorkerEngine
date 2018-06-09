#pragma once
#include <GL\glew.h>

#include "Component.h"

struct Model
{
	const GLfloat * _vertices;
	const GLuint * _indices;
	GLsizei VSize = 0, ISize = 0;

	Model() { _vertices = nullptr; _indices = nullptr; }
	Model(const GLfloat * vert, const GLuint * ind, const GLsizei & is, const GLsizei & vs)
		: _vertices{ vert }, _indices{ ind }, VSize(vs), ISize(is) {};

	~Model() { if (_vertices != nullptr) delete(_vertices); if (_indices != nullptr) delete(_indices); }

	void setVertices(const GLfloat * arr) { _vertices = arr; }
	void setIndices(const GLuint * arr) { _indices = arr; }

	const GLfloat * getVertices() { return _vertices; }
	const GLuint * getIndices() { return _indices; }
};

struct Texture
{
	const GLuint * _texture;
	GLuint imgWidth, imgHeight, texWidth, texHeight;
	GLuint TextureID;

	Texture(const GLuint * data, const GLuint & imgW, const GLuint & imgH, const GLuint & texW, const GLuint & texH)
		: _texture{ data }, imgWidth(imgW), imgHeight(imgH), texWidth(texW), texHeight(texH) {}

	~Texture() { if (TextureID != 0) glDeleteTextures(1, _texture); }

	void setTexture(const GLuint * arr) { _texture = arr; }
	const GLuint * getTexture() { return _texture; }
};

struct Shader
{
	const GLuint _shaderID;
	
	Shader(const GLuint & data) : _shaderID(data) {};

	~Shader() {}

	const GLuint getShaderID() { return _shaderID; };
};

class RenderComponent : public Component
{
public:

	RenderComponent
	(
		std::string id,
		Model * mdl = nullptr,
		Texture * tdl = nullptr,
		Shader * vShader = nullptr,
		Shader * fShader = nullptr
	) : Component(id), _model{ std::move(mdl) }, _texture{ tdl }, _vertexS{ vShader }, _fragmentS{ fShader }
	{
		_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	~RenderComponent()
	{
		glDeleteVertexArrays(1, &_VAO);
		glDeleteBuffers(1, &_VBO);
		glDeleteProgram(_ProgramID);
		glUseProgram(0);
	}

	void Initalize()
	{
		if (_model != nullptr)
		{
			glGenBuffers(1, &_EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(GLuint) * _model->ISize), _model->getIndices(), GL_STATIC_DRAW);

			glGenVertexArrays(1, &_VAO);
			glBindVertexArray(_VAO);

			glGenBuffers(1, &_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * _model->VSize), _model->getVertices(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(5 * sizeof(GL_FLOAT)));
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		if (_texture != nullptr)
		{
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &_texture->TextureID);
			glBindTexture(GL_TEXTURE_2D, _texture->TextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _texture->texWidth, _texture->texWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, _texture->getTexture());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (_vertexS != nullptr && _fragmentS != nullptr)
		{
			_ProgramID = glCreateProgram();

			glAttachShader(_ProgramID, _vertexS->_shaderID);
			glAttachShader(_ProgramID, _fragmentS->_shaderID);
			glLinkProgram(_ProgramID);

			GLint programSuccess = GL_FALSE;
			glGetProgramiv(_ProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", _ProgramID);
			}
			else
			{
				glUseProgram(_ProgramID);

				render_model_matrix_loc = glGetUniformLocation(_ProgramID, "model_matrix");
				render_projection_matrix_loc = glGetUniformLocation(_ProgramID, "projection_matrix");

				iSpriteFrame_loc = glGetUniformLocation(_ProgramID, "iSpriteFrame");
				texture_width_adj_loc = glGetUniformLocation(_ProgramID, "texture_width_adj");
				texture_height_adj_loc = glGetUniformLocation(_ProgramID, "texture_height_adj");
				if (_texture != nullptr && _texture->TextureID != 0)
				{
					glUniform1i(glGetUniformLocation(_ProgramID, "tex_available"), 1);
					glUniform1f(texture_width_adj_loc, _texture->imgWidth / (GLfloat)_texture->texWidth);
					glUniform1f(texture_height_adj_loc, _texture->imgHeight / (GLfloat)_texture->texHeight);
				}
				else
				{
					glUniform1i(glGetUniformLocation(_ProgramID, "tex_available"), 0);
				}
				color_vec_loc = glGetUniformLocation(_ProgramID, "color_vec");
				tex_color_loc = glGetUniformLocation(_ProgramID, "tex_color");
				tex_unit_loc = glGetUniformLocation(_ProgramID, "tex_unit");
			}
		}
	}

	GLsizei BindBuffers()
	{
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
		if (_texture != nullptr && _texture->TextureID != 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _texture->TextureID);
		}
		else
		{
			_color = glm::vec4(1.0f, 0.411f, 0.705f, 1.0f);
		}
		glUseProgram(_ProgramID);

		glUniform1i(tex_unit_loc, 0);
		glUniform4f(tex_color_loc, 1.0f, 1.0f, 1.0f, 1.0f);
		glUniform4f(color_vec_loc, _color.x, _color.y, _color.z, _color.w);

		if (iSpriteFrame_loc != -1)
		{
			glUniform1ui(iSpriteFrame_loc, 1);
		}
		return _model->ISize;
	}

	GLuint _VBO = 0;
	GLuint _VAO = 0;
	GLuint _EBO = 0;
	GLuint _ProgramID = 0;
	glm::vec4 _color;

	GLint render_projection_matrix_loc;
	GLint render_model_matrix_loc;

private:

	Model * _model;
	Texture * _texture;
	Shader * _vertexS;
	Shader * _fragmentS;

	GLint tex_color_loc;
	GLint tex_unit_loc;

	GLint iSpriteFrame_loc;
	GLint texture_width_adj_loc;
	GLint texture_height_adj_loc;

	GLint color_vec_loc;
};
