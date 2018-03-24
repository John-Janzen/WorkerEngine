#include "FileLoader.h"

FileLoader::FileLoader() {}

FileLoader::~FileLoader() {}

void FileLoader::Update(JOB_TYPES j, bool & flag, BaseContent* ptr)
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
	case FILE_LOAD_MODEL:
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
	FileLoadOBJContent * FLContent = static_cast<FileLoadOBJContent*> (ptr);
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
			ml = new Model_Loaded(mallocSpace(combined), mallocSpace(ind), ind.size(), combined.size(), textures.size(), normals.size());
			addModel(std::make_pair(FLContent->path, ml));
			printf("Model Loaded: %s\n", FLContent->path.c_str());
			modelCount++;
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
	std::vector<std::string> splitData;
	if (in.is_open())
	{
		
		std::string output((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		output.erase(std::remove_if(output.begin(), output.end(), ::isspace), output.end());

		if (output.empty())
		{
			printf("No data found within file %s", path.c_str());
		}
		else
		{
			std::string sub;
			size_t local, local2;
			do 
			{
				if (output[0] == '/' && output[1] == '/')
				{
					output = output.substr(local = output.find_first_of(';') + 1);
					continue;
				}
				sub = output.substr(0, local = output.find_first_of(';'));

				if ((local2 = output.find_first_of('{')) > local)
				{
					split(sub, ':', splitData);
					if (splitData[0].compare("load") == 0)
					{
						Manager::instance().addJob("Application", APPLICATION_NUMBER_OBJECTS, new IntPassContent(stoi(splitData[1])));
					}
					splitData.clear();
				}
				else
				{
					if (sub.substr(0, local2).compare("models") == 0)
					{
						size_t modelsEnd = sub.find_last_of('}') - 1;
						sub = sub.substr(local2 + 1, modelsEnd - local2);
						split(sub, ',', splitData);
						modelsToLoad = splitData.size();
						modelCount = 0;
						for (std::string s : splitData)
						{
							Manager::instance().addJob("FileLoader", FILE_LOAD_MODEL, new FileLoadOBJContent(std::string("Assets/" + s + ".obj")));
						}
						splitData.clear();
					}
					else
					{
						while (modelCount != modelsToLoad);
						Manager::instance().addJob("FileLoader", FILE_LOAD_GAMEOBJECT, new FileIndividualContent(sub));
					}
				}
				output = output.substr(local + 1);
			} while (!output.empty());
		}
		in.close();
	}
	else
	{
		printf("File Missing %s", path.c_str());
	}
}

void FileLoader::individualGameObject(BaseContent * ptr)
{
	FileIndividualContent * FIContent = static_cast<FileIndividualContent*> (ptr);

	std::vector<std::string> modelData;
	std::map<LOADABLE_ITEMS, std::string> gameObjData;
	std::vector<Component*> components;
	std::string data = FIContent->info;
	GameObject *go = nullptr;

	size_t begin = data.find_first_of('{'), end = data.find_last_of('}');

	findLoadItem("name", data.substr(0, begin), gameObjData);
	data = data.substr(begin + 1, end - begin - 1);

	size_t local;
	do
	{
		std::string s = data.substr(0, local = data.find_first_of(','));

		split(s, ':', modelData);
		if (modelData[0].compare("comp") == 0)
		{
			findLoadItem(modelData[0], modelData[1], gameObjData, &components);
		}
		else
		{
			if (modelData[0].compare("pos") == 0)
			{
				local = data.find_last_of(')');
				size_t local2 = data.find_first_of('(');
				findLoadItem(modelData[0], data.substr(local2 + 1, local - local2 - 1), gameObjData);
			}
			else
				findLoadItem(modelData[0], modelData[1], gameObjData);
		}
		modelData.clear();
		data = data.substr(local + 1);
	} while (!data.empty());
	
	if (gameObjData.find(TYPE)->second.compare("GameObject") == 0)
	{
		go = new GameObject(gameObjData, components);
	}
	else if (gameObjData.find(TYPE)->second.compare("Quad") == 0)
	{
		go = new Quad(gameObjData, components);
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

void FileLoader::split(const std::string &s, char delim, std::vector<std::string> & out) {
	split(s, delim, std::back_inserter(out));
}

void FileLoader::addModel(std::pair<std::string, Model_Loaded *> pair)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	_loadedModels.emplace(pair);
	_c.notify_all();
}

Model_Loaded * FileLoader::checkForModel(const std::string & s)
{
	return (_loadedModels.find(s) != _loadedModels.end()) ? _loadedModels.find(s)->second : nullptr;
}

void FileLoader::findLoadItem(const std::string & item, const std::string & data, std::map<LOADABLE_ITEMS, std::string> & map, std::vector<Component*> * comps)
{
	if (item.compare("type") == 0)
	{
		map.emplace(std::make_pair(TYPE, data));
	}
	else if (item.compare("name") == 0)
	{
		map.emplace(std::make_pair(NAME, data));
	}
	else if (item.compare("id") == 0)
	{
		map.emplace(std::make_pair(ID, data));
	}
	else if (item.compare("comp") == 0)
	{
		size_t brac1 = data.find_first_of('('), brac2 = data.find_last_of(')');
		if (data.substr(0, brac1).compare("render") == 0)
		{
			std::string sub2 = data.substr(brac1 + 1, brac2 - brac1 - 1);
			RenderComponent * rc = new RenderComponent();
			Model_Loaded * ml;
			if ((ml = checkForModel(std::string("Assets/" + sub2 + ".obj"))) != nullptr)
			{
				rc->setVertices(ml->vertices);
				rc->setIndices(ml->indices);
				rc->numInd = ml->ISize;
				rc->numVertices = ml->VSize;
			}
			else
			{
				this->ObjImporter(new FileLoadOBJContent(std::string("Assets/" + sub2 + ".obj"), rc));
			}
			comps->emplace_back(rc);
		}
		map.emplace(std::make_pair(COMP, data.substr(0, brac1)));
	}
	else if (item.compare("pos") == 0)
	{
		map.emplace(std::make_pair(POS, data));
	}
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
