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
		ObjImporter(ptr);
		break;
	default:
		break;
	}
	if (ptr != nullptr)
		delete(ptr);
	Manager::instance().signalDone();
}

void FileLoader::Close()
{

}

void FileLoader::ObjImporter(BaseContent * ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);

	FileLoadOBJContent * FLContent = static_cast<FileLoadOBJContent*> (ptr);
	RenderComponent * rc = FLContent->rc;
	std::ifstream in(FLContent->path.c_str());
	
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLuint> faces;
	std::vector<GLfloat> textures;
	std::vector<std::string> splitData, splitMore, splitMoreMore;

	if (in.is_open())
	{
		std::string output((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

		in.close();

		if (output.empty())
		{
			printf("No data foudn within file %s", FLContent->path.c_str());
		}
		else
		{
			split(output, '\n', splitData);
			for (std::vector<std::string>::iterator it = splitData.begin(); it != splitData.end(); ++it)
			{
				split(it->data(), ' ', splitMore);
				if (it->at(0) == 'v')
				{
					if (it->at(1) == ' ')
					{
						// Vertex found
						vertices.emplace_back(parseFloat(splitMore.at(1)));
						vertices.emplace_back(parseFloat(splitMore.at(2)));
						vertices.emplace_back(parseFloat(splitMore.at(3)));
					}
					else if (it->at(1) == 'n')
					{
						// Normal found
						normals.emplace_back(parseFloat(splitMore.at(1)));
						normals.emplace_back(parseFloat(splitMore.at(2)));
						normals.emplace_back(parseFloat(splitMore.at(3)));
					}
					else if (it->at(1) == 't')
					{
						// Texture found
						textures.emplace_back(parseFloat(splitMore.at(1)));
						textures.emplace_back(parseFloat(splitMore.at(2)));
					}
				}
				else if (it->at(0) == 'f')
				{
					// Face found
					
					for (std::vector<std::string>::iterator it2 = splitMore.begin() + 1; it2 != splitMore.end(); ++it2)
					{
						split(it2->data(), '/', splitMoreMore);
						faces.emplace_back(parseInt(splitMoreMore.at(0)));
						faces.emplace_back(parseInt(splitMoreMore.at(1)));
						faces.emplace_back(parseInt(splitMoreMore.at(2)));
						splitMoreMore.clear();
					}
				}
				splitMore.clear();
			}
			splitData.clear();
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
		printf("Error opening file: %s", FLContent->path.c_str());
	}
	_c.notify_one();
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
	std::map<std::string, std::string> gameObjData;
	std::vector<std::string> splitData, splitMore, modelData, keyValue;
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
			split(output, ';', splitData);
			for (std::vector<std::string>::iterator it = splitData.begin(); it != splitData.end(); ++it)
			{
				if (!(it->empty() || it->at(0) == '\t' || it->at(0) == '/'))
				{
					split(it->data(), ',', splitMore);
					for (std::vector<std::string>::iterator it2 = splitMore.begin(); it2 != splitMore.end(); ++it2)
					{
						split(it2->data(), ':', keyValue);
						if (keyValue[0].compare("comp") == 0)
						{
							split(keyValue[1], '/', modelData);
							if (modelData[0].compare("render") == 0)
							{
								RenderComponent * rc = new RenderComponent();
								Manager::instance().addJob("FileLoader", JOB_TYPES::FILE_OBJ_LOAD, new FileLoadOBJContent(std::string("Assets/" + modelData[1] + ".obj"), rc));
								components.emplace_back(rc);
							}
							gameObjData.emplace(std::make_pair(keyValue[0], modelData[0]));
							modelData.clear();
						}
						else
						{
							gameObjData.emplace(std::make_pair(keyValue[0], keyValue[1]));
						}
						keyValue.clear();
					}
					if (gameObjData.find("type")->second.compare("GameObject") == 0)
					{
						GameObjects.emplace_back(new GameObject(gameObjData, components));
						components.clear();
						gameObjData.clear();
					}
					splitMore.clear();
				}
			}
			splitData.clear();
		}
		Manager::instance().addJob("Application", JOB_TYPES::APPLICATION_ADD_OBJECTS, new FileLoadedContent(GameObjects));
	}
	else
	{
		printf("File Missing %s", path.c_str());
	}
	_c.notify_one();
}

GLfloat FileLoader::parseFloat(const std::string& str)
{
	return std::stof(str);
}

GLuint FileLoader::parseInt(const std::string& str)
{
	return std::stoi(str);
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

void FileLoader::split(const std::string &s, char delim, std::vector<std::string> & out) {
	split(s, delim, std::back_inserter(out));
}

template<typename Out>
Out* FileLoader::mallocSpace(std::vector<Out> tooManyVecs)
{
	Out* arr = (Out*)malloc(tooManyVecs.size() * sizeof(Out));
	int j = 0;
	for (std::vector<Out>::iterator it = tooManyVecs.begin(); it != tooManyVecs.end(); ++it, j++)
		arr[j] = *it;
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
