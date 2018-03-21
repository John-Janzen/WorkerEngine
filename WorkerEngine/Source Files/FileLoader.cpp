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
	case FILE_LOAD_GAMEOBJECT:
		individualGameObject(ptr);
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
	FileLoadOBJContent * FLContent = static_cast<FileLoadOBJContent*> (ptr);
	RenderComponent * rc = FLContent->rc;
	std::ifstream in(FLContent->path.c_str());

	Model_Loaded * ml;
		
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
			GLfloat * vert = mallocSpace(combined);
			GLuint * in = mallocSpace(ind);
			rc->setVertices(vert);
			rc->setIndices(in);
			rc->numInd = ind.size();
			rc->numVertices = combined.size();
			rc->numTextures = textures.size();
			rc->numNormals = normals.size();

			ml = new Model_Loaded(vert, in, ind.size(), combined.size(), textures.size(), normals.size());
			addModel(std::make_pair(FLContent->path, ml));
			printf("Model Loaded: %s\n", FLContent->path.c_str());
		}
	}
	else
	{
		printf("Error opening file: %s", FLContent->path.c_str());
	}

}

void FileLoader::loadTextData(BaseContent * ptr)
{
	FileToLoadContent * FTLContent = static_cast<FileToLoadContent*>(ptr);

	std::string path = FTLContent->path;
	std::ifstream in(path.c_str());

	int location = -1;
	std::vector<std::string> splitData, splitMore;
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
					if (it->at(0) == '#')
					{
						std::vector<std::string> smallSplit;
						split(it->data(), ':', smallSplit);
						Manager::instance().addJob("Application", APPLICATION_NUMBER_OBJECTS, new IntPassContent(stoi(smallSplit[1])));
					}
					else
					{
						split(it->data(), ',', splitMore);
						Manager::instance().addJob("FileLoader", FILE_LOAD_GAMEOBJECT, new FileIndividualContent(splitMore));
						splitMore.clear();
					}
				}
			}
			splitData.clear();
		}
	}
	else
	{
		printf("File Missing %s", path.c_str());
	}
}

void FileLoader::individualGameObject(BaseContent * ptr)
{
	FileIndividualContent * FIContent = static_cast<FileIndividualContent*> (ptr);

	std::vector<std::string> modelData, keyValue;
	std::map<LOADABLE_ITEMS, std::string> gameObjData;
	std::vector<Component*> components;
	GameObject *go = nullptr;
	for (std::vector<std::string>::iterator it2 = FIContent->info.begin(); it2 != FIContent->info.end(); ++it2)
	{
		split(it2->data(), ':', keyValue);
		if (keyValue[0].compare("comp") == 0)
		{
			split(keyValue[1], '/', modelData);
			if (modelData[0].compare("render") == 0)
			{
				RenderComponent * rc = new RenderComponent();
				Model_Loaded * ml;
				std::unique_lock<std::mutex> lock(_lockMutex);
				if ((ml = checkForModel(std::string("Assets/" + modelData[1] + ".obj"))) != nullptr)
				{
					rc->setVertices(ml->vertices);
					rc->setIndices(ml->indices);
					rc->numInd = ml->ISize;
					rc->numNormals = ml->NSize;
					rc->numTextures = ml->TSize;
					rc->numVertices = ml->VSize;
				}
				else
				{
					this->ObjImporter(new FileLoadOBJContent(std::string("Assets/" + modelData[1] + ".obj"), rc));
				}
				_c.notify_all();
				components.emplace_back(rc);
			}
			gameObjData.emplace(std::make_pair(findLoadItem(keyValue[0]), modelData[0]));
			modelData.clear();
		}
		else
		{
			gameObjData.emplace(std::make_pair(findLoadItem(keyValue[0]), keyValue[1]));
		}
		keyValue.clear();
	}
	if (gameObjData.find(TYPE)->second.compare("GameObject") == 0)
	{
		go = new GameObject(gameObjData, components);
	}
	Manager::instance().addJob("Application", APPLICATION_ADD_SINGLE_OBJECT, new FileLoadedContent(go));
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

void FileLoader::addModel(std::pair<std::string, Model_Loaded *> pair)
{
	_loadedModels.emplace(pair);
}

Model_Loaded * FileLoader::checkForModel(const std::string & s)
{
	return (_loadedModels.find(s) != _loadedModels.end()) ? _loadedModels.find(s)->second : nullptr;
}

LOADABLE_ITEMS FileLoader::findLoadItem(const std::string & item)
{
	if (item.compare("type") == 0)
	{
		return TYPE;
	}
	else if (item.compare("name") == 0)
	{
		return NAME;
	}
	else if (item.compare("id") == 0)
	{
		return ID;
	}
	else if (item.compare("comp") == 0)
	{
		return COMP;
	}
	else if (item.compare("posX") == 0)
	{
		return POSX;
	}
	else if (item.compare("posY") == 0)
	{
		return POSY;
	}
	else if (item.compare("posZ") == 0)
	{
		return POSZ;
	}
	return DEFAULT;
}

std::vector<GLfloat> FileLoader::combine(std::vector<GLuint> faces, std::vector<GLfloat> vert, std::vector<GLfloat> norm, std::vector<GLfloat> text, std::vector<GLuint> & indices)
{
	std::vector<GLfloat> finalData;
	std::map<std::string, size_t> locations;
	indices = std::vector<GLuint>();
	size_t location = 0; 
	std::map <std::string, size_t> ::iterator loc;
	
	for (std::vector<GLuint>::iterator it = faces.begin(); it != faces.end(); it += 3)
	{
		GLuint v = *it, n = *(it + 1), t = *(it + 2);
		std::stringstream ss;
		ss << v << '/' << n << '/' << t;

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
