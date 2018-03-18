#include "FileLoader.h"

FileLoader::FileLoader() {}

FileLoader::~FileLoader() {}

void FileLoader::Update(JOB_TYPES j, void * ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	switch (j)
	{
	case FILE_LOAD_TXT_DATA:
		loadTextData(ptr);
		break;
	default:
		break;
	}
	if (ptr != nullptr)
		ptr = nullptr;
	_c.notify_one();
}

void FileLoader::Close()
{

}

void FileLoader::loadTextData(void * ptr)
{
	std::string * s = static_cast<std::string*>(ptr);
	std::ifstream in(s->c_str());
	std::vector<GameObject*> _objects;
	int location = -1;
	if (in.is_open())
	{
		std::string line;
		while (std::getline(in, line))
		{
			if ((line[0] == '/' && line[1] == '/') || line.empty())	continue;
			
			size_t find;
			if ((find = line.find_first_of(':')) != NULL)
			{
				std::string sub = line.substr(0, find);
				std::string sub2 = line.substr(find + 2, line.length() - 1);
				if (sub.compare("type") == 0)
				{
					if (sub2.compare("GameObject") == 0)
					{
						_objects.emplace_back(new GameObject());
						location++;
					}
				}
				else 
				{
					readGameObject(sub, sub2, _objects.at(location));
				}
			}
		}
		in.close();
	}
	else
	{
		printf("File Missing %s", s->c_str());
	}
}

void FileLoader::readGameObject(std::string type, std::string data, GameObject * & go)
{
	if (type.compare("id") == 0)
	{
		go->setID(atoi(data.c_str()));
	}
	else if (type.compare("Comp") == 0)
	{
		if (data.compare("render") == 0)
		{
			go->addComponent("Render", new RenderComponent());
		}
	}
	else if (type.compare("pos") == 0)
	{
		std::istringstream f(data);
		std::vector<std::string> s;
		std::string line;
		while (std::getline(f, line, ' '))
		{
			s.emplace_back(line);
		}
		go->setPos(glm::vec3(atoi(s.at(0).c_str()), atoi(s.at(1).c_str()), atoi(s.at(2).c_str())));
	}
	else if (type.compare("name") == 0)
	{
		go->setName(data);
	}
}
