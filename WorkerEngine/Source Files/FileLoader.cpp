#include <IL\il.h>
#include <IL\ilu.h>

#include "FileLoader.h"
#include "Application.h"

GLuint powerOfTwo(GLuint num)
{
	if (num != 0)
	{
		num--;
		num |= (num >> 1); //Or first 2 bits
		num |= (num >> 2); //Or next 2 bits
		num |= (num >> 4); //Or next 4 bits
		num |= (num >> 8); //Or next 8 bits
		num |= (num >> 16); //Or next 16 bits
		num++;
	}

	return num;
}

FileLoader::FileLoader(Application * a) : System(a)
{
	ilInit();
	iluInit();
	ilClearColour(255, 255, 255, 000);
	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
	{
		printf("Error initializing DevIL! %s\n", iluErrorString(ilError));
	}
}

FileLoader::~FileLoader() { Close(); }

void FileLoader::Update(JOB_TYPES j, bool & flag, BaseContent* ptr)
{
	switch (j)
	{
	case FILE_LOAD_EXTERNAL:
		LoadExternalFile(ptr);
		break;
	case FILE_LOAD_TXT_DATA:
		loadTextData(ptr);
		break;
	case FILE_LOAD_GAMEOBJECT:
		individualGameObject(ptr);
		break;
	case FILE_LOAD_MODEL:
		ObjImporter(ptr);
		break;
	case FILE_LOAD_TEXTURE:
		loadTextureData(ptr);
		break;
	default:
		break;
	}
	if (ptr != nullptr)
		delete(ptr);
}

void FileLoader::Close()
{
	for (std::map<std::string, Texture*>::iterator it = _loadedTextures.begin(); it != _loadedTextures.end(); ++it)
		delete(it->second);
	_loadedTextures.clear();

	for (std::map<std::string, Model*>::iterator it = _loadedModels.begin(); it != _loadedModels.end(); ++it)
		delete(it->second);
	_loadedModels.clear();
}

void FileLoader::LoadExternalFile(BaseContent * ptr)
{
	FileToLoadContent * FLContent = static_cast<FileToLoadContent*> (ptr);
	std::string extension = FLContent->path.substr(FLContent->path.find_last_of('.') + 1);
	if (extension.compare("dat") == 0)
	{
		_app->addJob("FileLoader", FILE_LOAD_TXT_DATA, new FileToLoadContent(FLContent->path, 0));
	}
	else if (extension.compare("obj") == 0)
	{
		_app->addJob("FileLoader", FILE_LOAD_MODEL, new FileLoadOBJContent(FLContent->path));
	}

}

void FileLoader::ObjImporter(BaseContent * ptr)
{
	FileLoadOBJContent * FLContent = static_cast<FileLoadOBJContent*> (ptr);
	
	Model * ml;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::u16vec3> faces;
	std::vector<glm::vec2> textures;
	FILE * file_stream;

	if (fopen_s(&file_stream, FLContent->data.c_str(), "r") != 0)
	{
		printf("No data foudn within file %s", FLContent->data.c_str());
	}
	else
	{
		while (1)
		{
			char lineHeader[128];

			int res = fscanf_s(file_stream, "%s", lineHeader, _countof(lineHeader));
			if (res == EOF)
				break;
			if (strcmp(lineHeader, "v") == 0)
			{
				glm::vec3 vertex;
				fscanf_s(file_stream, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 uv;
				fscanf_s(file_stream, "%f %f\n", &uv.x, &uv.y);
				textures.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 normal;
				fscanf_s(file_stream, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				std::string vertex1, vertex2, vertex3;
				glm::uvec3 face[3];
				int matches = fscanf_s(file_stream, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
					&face[0].x, &face[0].y, &face[0].z,
					&face[1].x, &face[1].y, &face[1].z,
					&face[2].x, &face[2].y, &face[2].z);

				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				}
				faces.push_back(face[0]);
				faces.push_back(face[1]);
				faces.push_back(face[2]);
			}
			else
			{
				fgets(lineHeader, 128, file_stream);
			}
		}
		fclose(file_stream);

		std::vector<GLuint> ind;
		std::vector<GLfloat> combined = combine(faces, vertices, normals, textures, ind);
		ml = new Model(mallocSpace(combined), mallocSpace(ind), ind.size(), combined.size());
		addModel(std::make_pair(FLContent->data, ml));
		printf("Model Loaded: %s\n", FLContent->data.c_str());
		modelCount++;
	}
}

void FileLoader::loadTextureData(BaseContent * ptr)
{
	FileToLoadContent * FTLContent = static_cast<FileToLoadContent*>(ptr);

	Texture * txt_Load;

	std::unique_lock<std::mutex> lock(_lockMutex);

	ILuint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);
	ILboolean success = ilLoadImage(FTLContent->path.c_str());
	if (success == IL_TRUE)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (success == IL_TRUE)
		{
			GLuint imgWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
			GLuint imgHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

			GLuint texWidth = powerOfTwo(imgWidth);
			GLuint texHeight = powerOfTwo(imgHeight);

			if (imgWidth != texWidth || imgHeight != texHeight)
			{
				iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
				iluEnlargeCanvas((int)texWidth, (int)texHeight, 1);
			}
			ILint size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
			void* _data = malloc(size);
			ILubyte* data = ilGetData();
			memcpy(_data, data, size);
			txt_Load = new Texture((GLuint*)_data, imgWidth, imgHeight, texWidth, texHeight);
			_loadedTextures.emplace(std::make_pair(FTLContent->path, txt_Load));
			printf("Texure Loaded: %s\n", FTLContent->path.c_str());
			textCount++;
		}
		ilBindImage(0);
		ilDeleteImages(1, &imgID);
	}
	else
	{
		ILenum ilError = ilGetError();
		printf("Error occured: %s\n", iluErrorString(ilError));
	}
	_c.notify_all();
}

void FileLoader::loadTextData(BaseContent * ptr)
{
	FileToLoadContent * FTLContent = static_cast<FileToLoadContent*>(ptr);

	FILE * file_stream;
	int location = -1;

	if (fopen_s(&file_stream, FTLContent->path.c_str(), "r") != 0)
	{
		printf("No data found within file %s", FTLContent->path.c_str());
	}
	else
	{
		fseek(file_stream, FTLContent->location, SEEK_SET);
		char lineHeader[32];
		int end;
		while (fscanf_s(file_stream, "%s", lineHeader, _countof(lineHeader)) != EOF)
		{
			if (strcmp(lineHeader, "load") == 0)
			{
				uint32_t num = 0;
				fscanf_s(file_stream, "%u;", &num);
				_app->initNumberObjects(num);
			}
			else if (strcmp(lineHeader, "models") == 0)
			{
				modelCount = modelsToLoad;
				do
				{
					fscanf_s(file_stream, "%s", lineHeader, _countof(lineHeader));
					if (strcmp(lineHeader, "{") != 0 && strcmp(lineHeader, "};") != 0)
					{
						std::string name = "Assets/" + std::string(lineHeader) + ".obj";
						if (_loadedModels.find(name) == _loadedModels.end())
						{
							modelsToLoad++;
							_app->addJob("FileLoader", FILE_LOAD_EXTERNAL, new FileToLoadContent(name, 0));
						}
					}
				} while (strcmp(lineHeader, "};") != 0);
			}
			else if (strcmp(lineHeader, "textures") == 0)
			{
				textCount = texturesToLoad;
				do
				{
					fscanf_s(file_stream, "%s", lineHeader, _countof(lineHeader));
					if (strcmp(lineHeader, "{") != 0 && strcmp(lineHeader, "};") != 0)
					{
						std::string name = "Assets/" + std::string(lineHeader) + ".png";
						if (_loadedTextures.find(name) == _loadedTextures.end())
						{
							texturesToLoad++;
							_app->addJob("FileLoader", FILE_LOAD_TEXTURE, new FileToLoadContent(name, 0));
						}
					}
				} while (strcmp(lineHeader, "};") != 0);
			}
			else
			{
				if (modelCount != modelsToLoad && textCount != texturesToLoad)
				{

					_app->addJob("FileLoader", FILE_LOAD_TXT_DATA, new FileToLoadContent(FTLContent->path, ftell(file_stream) - strlen(lineHeader)));
					fclose(file_stream);
					return;
				}
				else
				{
					std::string obj = lineHeader;
					obj += " ";
					do
					{
						fscanf_s(file_stream, "%s", lineHeader, _countof(lineHeader));
						obj.append(lineHeader);
						obj.append(" ");
					} while (strcmp(lineHeader, "};") != 0);
					_app->addJob("FileLoader", FILE_LOAD_GAMEOBJECT, new FileIndividualContent(obj));
				}
			}
		}
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
	char lineHeader[32];
	sscanf_s(data.c_str(), "%s", lineHeader, _countof(lineHeader));
	gameObjData.emplace(std::make_pair(NAME, std::string(lineHeader)));
	do
	{
		data = data.substr(data.find_first_of(' ') + 1, data.length());
		sscanf_s(data.c_str(), "%s", lineHeader, _countof(lineHeader));
		if (strcmp(lineHeader, "{") != 0 && strcmp(lineHeader, "};") != 0)
		{
			data = data.substr(data.find_first_of(' ') + 1, data.length());
			if (strcmp(lineHeader, "type:") == 0)
			{
				sscanf_s(data.c_str(), "%s", lineHeader, _countof(lineHeader));
				gameObjData.emplace(std::make_pair(TYPE, std::string(lineHeader)));
			}
			else if (strcmp(lineHeader, "id:") == 0)
			{
				sscanf_s(data.c_str(), "%s", lineHeader, _countof(lineHeader));
				gameObjData.emplace(std::make_pair(ID, std::string(lineHeader)));
			}
			else if (strcmp(lineHeader, "comp:") == 0)
			{	
				size_t brac1 = data.find_first_of('('), brac2 = data.find_first_of(')');
				if (data.substr(0, brac1).compare("render") == 0)
				{
					std::string sub2 = data.substr(brac1 + 1, brac2 - brac1 - 1);
					size_t slash = sub2.find_first_of('/');
					RenderComponent * rc;
					Model * m;
					Texture * t;
					if (slash < sub2.size())
					{
						m = checkForModel(std::string("Assets/" + sub2.substr(0, slash) + ".obj"));
						t = checkForTexture(std::string("Assets/" + sub2.substr(slash + 1) + ".png"));
						rc = new RenderComponent(m, t);
					}
					else
					{
						m = checkForModel(std::string("Assets/" + sub2 + ".obj"));
						rc = new RenderComponent(m);
					}
					components.emplace_back(rc);
				}
				gameObjData.emplace(std::make_pair(COMP, data.substr(0, brac1)));
			}
			else if (strcmp(lineHeader, "pos:") == 0)
			{
				char num[8];
				std::string info = "";
				for (int i = 0; i < 3; i++)
				{
					sscanf_s(data.c_str(), "%s", num, _countof(num));
					info.append(num);
					if (i != 2)
					{
						data = data.substr(data.find_first_of(' ') + 1, data.length());
					}
				}
				gameObjData.emplace(std::make_pair(POS, info));
			}
			else if (strcmp(lineHeader, "rot:") == 0)
			{
				char line[8];
				std::string info = "";
				for (int i = 0; i < 3; i++)
				{
					sscanf_s(data.c_str(), "%s", line, _countof(line));
					info.append(line);
					if (i != 2)
					{
						data = data.substr(data.find_first_of(' ') + 1, data.length());
					}
				}
				gameObjData.emplace(std::make_pair(ROT, info));
			}
		}
	} while (strcmp(lineHeader, "};") != 0);
	
	if (gameObjData.find(TYPE)->second.compare("Player") == 0)
	{
		go = new Player(gameObjData, components);
		_app->addJob("Input", INPUT_ADD_PLAYER, new InputIPContent(go));
	}
	else if (gameObjData.find(TYPE)->second.compare("Quad") == 0)
	{
		go = new Quad(gameObjData, components);
	}
	else
	{
		go = new GameObject(gameObjData, components);
	}
	_app->addSingleObject(go);
}

void FileLoader::addModel(std::pair<std::string, Model*> pair)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	_loadedModels.emplace(pair);
	_c.notify_all();
}

Model * FileLoader::checkForModel(const std::string & s)
{
	return (_loadedModels.find(s) != _loadedModels.end()) ? _loadedModels.find(s)->second : nullptr;
}

void FileLoader::addTexture(std::pair<std::string, Texture*> pair)
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	_loadedTextures.emplace(pair);
	_c.notify_all();
}

Texture * FileLoader::checkForTexture(const std::string & s)
{
	return (_loadedTextures.find(s) != _loadedTextures.end()) ? _loadedTextures.find(s)->second : nullptr;
}

std::vector<GLfloat> FileLoader::combine
(
	const std::vector<glm::u16vec3> & faces, 
	const std::vector<glm::vec3> & vert, 
	const std::vector<glm::vec3> & norm, 
	const std::vector<glm::vec2> & text, 
	std::vector<GLuint> & indices
)
{
	std::vector<GLfloat> finalData;
	std::map<std::string, size_t> locations;
	indices = std::vector<GLuint>();
	size_t location = 0;
	std::map <std::string, size_t> ::iterator loc;

	for (std::vector<glm::u16vec3>::const_iterator it = faces.begin(); it != faces.end(); ++it)
	{
		GLuint v = (*it).x, n = (*it).y, t = (*it).z;
		std::stringstream ss;
		ss << v << '/' << n << '/' << t;

		if ((loc = locations.find(ss.str())) == locations.end())
		{
			indices.emplace_back(location);
			locations.emplace(std::make_pair(ss.str(), location++));
			size_t p = ((*it).x - 1);
			size_t pt = ((*it).y - 1);
			size_t pn = ((*it).z - 1);
			finalData.emplace_back(vert.at(p).x);
			finalData.emplace_back(vert.at(p).y);
			finalData.emplace_back(vert.at(p).z);

			finalData.emplace_back(text.at(pt).x);
			finalData.emplace_back(text.at(pt).y);

			finalData.emplace_back(norm.at(pn).x);
			finalData.emplace_back(norm.at(pn).y);
			finalData.emplace_back(norm.at(pn).z);
		}
		else
		{
			indices.emplace_back(loc->second);
		}
	}
	return finalData;
}
