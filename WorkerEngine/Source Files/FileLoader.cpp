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

void FileLoader::ObjImporter(BaseContent * ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	FileToLoadContent * FTLContent = static_cast<FileToLoadContent*>(ptr);

	std::string path = FTLContent->path;
	std::ifstream in(path.c_str());

	if (in.is_open())
	{

	}
	else
	{
		printf("Error opening file: %s", path.c_str());
	}
	_c.notify_all();
}

void FileLoader::loadTextData(BaseContent * ptr)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	FileToLoadContent * FTLContent = static_cast<FileToLoadContent*>(ptr);

	std::string path = FTLContent->path;
	std::ifstream in(path.c_str());

	int location = -1;
	std::vector<GameObject*> GameObjects;
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
					GameObject * go;
					std::vector<std::string> splitMore = split(it->data(), ',');
					for (std::vector<std::string>::iterator it2 = splitMore.begin(); it2 != splitMore.end(); ++it2)
					{
						std::vector<std::string> keyValue = split(it2->data(), ':');
						gameObjData.emplace(std::make_pair(keyValue[0], keyValue[1]));
					}

					std::string gameObjectType = gameObjData.find("type")->second;
					if (gameObjectType.compare("GameObject") == 0)
					{
						GameObjects.emplace_back(new GameObject(gameObjData));
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
