#include "GameObject.h"

GameObject::GameObject(std::string name, int id, glm::vec3 pos) :
	_name(name), _ID(id), _position(pos)
{}

GameObject::GameObject(std::map<LOADABLE_ITEMS, std::string> map, std::vector<Component*> comp)
{
	GLfloat x = 0, y = 0, z = 0;
	GLfloat rotX = 0, rotY = 0, rotZ = 0;
	for (std::map<LOADABLE_ITEMS, std::string>::iterator it = map.begin(); it != map.end(); ++it)
	{
		switch (it->first)
		{
		case NAME:
			_name = it->second;
			break;
		case COMP:
		{
			for (Component * c : comp)
			{
				this->addComponent(it->second, c);
			}
			break;
		}
		case ID:
			_ID = atoi(it->second.c_str());
			break;
		case POS:
		{
			std::string data = it->second;
			size_t loc;
			int count = 0;
			do
			{
				std::string sub = data.substr(0, (loc = data.find_first_of(',')));
				if (count == 0) x = (GLfloat)atof(sub.c_str());
				else if (count == 1) y = (GLfloat)atof(sub.c_str());
				else { z = (GLfloat)atof(sub.c_str()); break; }
				data = data.substr(loc + 1);
				count++;
			} while (!data.empty());
			_position = glm::vec3(x, y, z);
			break;
		}
		case ROT:
		{
			std::string data = it->second;
			size_t loc;
			int count = 0;
			do
			{
				std::string sub = data.substr(0, (loc = data.find_first_of(',')));
				if (count == 0) rotX = (GLfloat)atof(sub.c_str());
				else if (count == 1) rotY = (GLfloat)atof(sub.c_str());
				else { rotZ = (GLfloat)atof(sub.c_str()); break; }
				data = data.substr(loc + 1);
				count++;
			} while (!data.empty());
			_rotation = glm::vec3(rotX, rotY, rotZ);
			break;
		}
		default:
			break;
		}
	}
}

GameObject::~GameObject() 
{
	for (std::map<std::string, Component*>::iterator it = _components.begin(); it != _components.end(); ++it)
		delete(it->second);
	_components.clear();
}

void GameObject::DrawMyself(const float * matrix)
{
	RenderComponent * rc = nullptr;
	if ((rc = static_cast<RenderComponent*>(getComponent("render"))) != nullptr)
	{
		GLsizei num = rc->BindBuffers();

		glm::mat4 rotation = glm::mat4();
		rotation = glm::translate(rotation, _position);
		rotation = glm::rotate(rotation, _rotation.z, glm::vec3(0, 0, 1));
		rotation = glm::rotate(rotation, _rotation.x, glm::vec3(1, 0, 0));
		glm::mat4 model_matrix = glm::rotate(rotation, _rotation.y, glm::vec3(0, 1, 0));

		glUniformMatrix4fv(rc->render_projection_matrix_loc, 1, GL_FALSE, matrix);
		glUniformMatrix4fv(rc->render_model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}
	rc = nullptr;
}
