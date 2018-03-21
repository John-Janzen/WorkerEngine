#include "FileLoader.h"

FileLoader::FileLoader() {}

FileLoader::~FileLoader() {}

void FileLoader::Update(JOB_TYPES j, BaseContent* ptr)
{
	Manager::instance().signalWorking();
	switch (j)
	{
	case FILE_LOAD_TXT_DATA:
		loadTextData(ptr);
		break;
	case FILE_OBJ_LOAD:
		break;
	default:
		break;
	}
	if (ptr != nullptr)
		ptr = nullptr;
	Manager::instance().signalDone();
}

void FileLoader::Close()
{

}

RenderComponent * FileLoader::ObjImporter(std::string path, RenderComponent* rc)
{
	std::ifstream in(path.c_str());
	
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLuint> faces;
	std::vector<GLfloat> textures;

	if (in.is_open())
	{
		std::string output((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

		in.close();

		if (output.empty())
		{
			printf("No data foudn within file %s", path.c_str());
		}
		else
		{
			std::vector<std::string> splitData = split(output, '\n');
			for (std::vector<std::string>::iterator it = splitData.begin(); it != splitData.end(); ++it)
			{
				std::vector<std::string> splitMore = split(it->data(), ' ');
				if (it->at(0) == 'v')
				{
					if (it->at(1) == ' ')
					{
						// Vertex found
						for (std::vector<std::string>::iterator it2 = splitMore.begin() + 1; it2 != splitMore.end(); ++it2)
						{
							vertices.emplace_back((GLfloat)atof(it2->data()));
						}
					}
					else if (it->at(1) == 'n')
					{
						// Normal found
						for (std::vector<std::string>::iterator it2 = splitMore.begin() + 1; it2 != splitMore.end(); ++it2)
						{
							normals.emplace_back((GLfloat)atof(it2->data()));
						}
					}
					else if (it->at(1) == 't')
					{
						// Texture found
						for (std::vector<std::string>::iterator it2 = splitMore.begin() + 1; it2 != splitMore.end(); ++it2)
						{
							textures.emplace_back((GLfloat)atof(it2->data()));
						}
					}
				}
				else if (it->at(0) == 'f')
				{
					// Face found
					
					for (std::vector<std::string>::iterator it2 = splitMore.begin() + 1; it2 != splitMore.end(); ++it2)
					{
						std::vector<std::string> splitMoreMore = split(it2->data(), '/');
						for (std::string s : splitMoreMore)
						{
							faces.emplace_back((GLfloat)atoi(s.c_str()));
						}
						
					}
				}
			}
			std::vector<GLuint> ind;
			std::vector<GLfloat> combined = combine(faces, vertices, normals, textures, ind);
			rc->setVertices(mallocSpace(combined));
			rc->setIndices(mallocSpace(ind));
			rc->numInd = ind.size();
			rc->numVertices = combined.size();
			rc->numTextures = textures.size();
			rc->numNormals = normals.size();
		}
	}
	else
	{
		printf("Error opening file: %s", path.c_str());
	}
	return rc;
}

void FileLoader::loadTextData(BaseContent * ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	FileToLoadContent * FTLContent = static_cast<FileToLoadContent*>(ptr);

	std::string path = FTLContent->path;
	std::ifstream in(path.c_str());

	int location = -1;
	std::vector<GameObject*> GameObjects;
	std::vector<Component*> components;
	if (in.is_open())
	{
		std::string output((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		output.erase(std::remove_if(output.begin(), output.end(), ::isspace), output.end());
		in.close();
		
		if (output.empty())
		{
			printf("No data found within file %s", path.c_str());
		}
		else
		{
			std::vector<std::string> splitDataVector = split(output, ';');
			for (std::vector<std::string>::iterator it = splitDataVector.begin(); it != splitDataVector.end(); ++it)
			{
				if (!(it->empty() || it->at(0) == '\t' || it->at(0) == '/'))
				{
					std::map<std::string, std::string> gameObjData;
					std::vector<std::string> splitMore = split(it->data(), ',');
					for (std::vector<std::string>::iterator it2 = splitMore.begin(); it2 != splitMore.end(); ++it2)
					{
						std::vector<std::string> keyValue = split(it2->data(), ':');
						if (keyValue[0].compare("comp") == 0)
						{
							std::vector<std::string> modelData = split(keyValue[1], '/');
							if (modelData[0].compare("render") == 0)
							{
								components.emplace_back(ObjImporter(std::string("Assets/" + modelData[1] + ".obj"), new RenderComponent()));
							}
							gameObjData.emplace(std::make_pair(keyValue[0], modelData[0]));
						}
						else
						{
							gameObjData.emplace(std::make_pair(keyValue[0], keyValue[1]));
						}
						
					}
					std::string gameObjectType = gameObjData.find("type")->second;
					if (gameObjectType.compare("GameObject") == 0)
					{
						GameObjects.emplace_back(new GameObject(gameObjData, components));
						components.clear();
					}
				}
			}
		}
		Manager::instance().addJob("Application", JOB_TYPES::APPLICATION_ADD_OBJECTS, new FileLoadedContent(GameObjects));
	}
	else
	{
		printf("File Missing %s", path.c_str());
	}
	_c.notify_one();
}

template<typename Out>
void FileLoader::split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> FileLoader::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

template<typename Out>
Out* FileLoader::mallocSpace(std::vector<Out> tooManyVecs)
{
	size_t sizeInner = tooManyVecs.size();
	Out* arr = (Out*)malloc(sizeInner * sizeof(Out));
	int j = 0;
	for (std::vector<Out>::iterator it = tooManyVecs.begin(); it != tooManyVecs.end(); ++it, j++)
	{
		arr[j] = *it;
	}
	return arr;
}

std::vector<GLfloat> FileLoader::combine(std::vector<GLuint> faces, std::vector<GLfloat> vert, std::vector<GLfloat> norm, std::vector<GLfloat> text, std::vector<GLuint> & indices)
{
	std::vector<GLfloat> finalData;
	std::map<std::string, size_t> locations;
	indices = std::vector<GLuint>();
	size_t location = 0;
	
	for (std::vector<GLuint>::iterator it = faces.begin(); it != faces.end(); it += 3)
	{
		GLuint v = *it, n = *(it + 1), t = *(it + 2);
		std::stringstream ss;
		ss << v << '/' << n << '/' << t;
		
		std::map <std::string, size_t> ::iterator loc;
		if ((loc = locations.find(ss.str())) == locations.end())
		{
			indices.emplace_back(location);
			locations.emplace(std::make_pair(ss.str(), location++));
			size_t p = ((*it) - 1) * 3;
			size_t pt = (*(it + 1) - 1) * 2;
			size_t pn = (*(it + 2) - 1) * 3;
			finalData.emplace_back(vert.at(p));
			finalData.emplace_back(vert.at(p + 1)); 
			finalData.emplace_back(vert.at(p + 2));
			
			finalData.emplace_back(text.at(pt));
			finalData.emplace_back(text.at(pt + 1));

			finalData.emplace_back(norm.at(pn));
			finalData.emplace_back(norm.at(pn + 1));
			finalData.emplace_back(norm.at(pn + 2));
		}
		else 
		{
			indices.emplace_back(loc->second);
		}
	}
	return finalData;
}
