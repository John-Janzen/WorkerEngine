#pragma once
#include <iostream>
#include <sstream>
#include <ostream>
#include <fstream>
#include <string>
#include <iterator>
#include <chrono>
#include <algorithm>
#include <filesystem>
#include <vector>
#include <atomic>
#include <regex>

#include "System.h"

static std::atomic_int32_t modelCount = -1, textCount = -1;

class FileLoader : public System
{
public:
	FileLoader(Application * a);
	~FileLoader();

	virtual void Update(JOB_TYPES j, bool & flag, BaseContent* ptr);
	virtual void Close();

	void LoadExternalFile(BaseContent * ptr);

	void ObjImporter(BaseContent * ptr);

	void loadTextureData(BaseContent * ptr);

	void loadTextData(BaseContent* ptr);

	void individualGameObject(BaseContent * ptr);

	GLfloat parseFloat(const std::string & str);

	GLuint parseInt(const std::string & str);

	void split(const std::string & s, char delim, std::vector<std::string> & out);
	
	void addModel(std::pair<std::string, Model*> pair);

	Model * checkForModel(const std::string & s);

	void addTexture(std::pair<std::string, Texture*> pair);

	Texture * checkForTexture(const std::string & s);

	void findLoadItem(const std::string & item, const std::string & data, std::map<LOADABLE_ITEMS, std::string>&, std::vector<Component*> * c = nullptr);

	std::vector<GLfloat> combine (
		const std::vector<glm::u16vec3>& faces, 
		const std::vector<glm::vec3>& vert, 
		const std::vector<glm::vec3>& norm, 
		const std::vector<glm::vec2>& text, 
		std::vector<GLuint>& indices
	);
	

private:

	template<typename Out>
	Out * mallocSpace(std::vector<Out> tooManyVecs)
	{
		Out* arr = (Out*)malloc(tooManyVecs.size() * sizeof(Out));
		int j = 0;
		for (std::vector<Out>::iterator it = tooManyVecs.begin(); it != tooManyVecs.end(); ++it, j++)
			arr[j] = *it;
		return arr;
	}

	std::map<std::string, Model*> _loadedModels;
	std::map<std::string, Texture*>  _loadedTextures;
	std::atomic<size_t> modelsToLoad = 0, texturesToLoad = 0;
};

