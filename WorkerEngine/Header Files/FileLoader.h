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

#include "System.h"
#include "Manager.h"

static std::atomic_int32_t modelCount = -1;

struct Model_Loaded
{
	const GLfloat * vertices;
	const GLuint * indices;
	const size_t VSize, ISize, TSize, NSize;
	Model_Loaded(const GLfloat * vert, const GLuint * ind, const size_t is, const size_t vs, const size_t ts, const size_t ns)
		: vertices{ vert }, indices{ ind }, VSize(vs), ISize(is), TSize(ts), NSize(ns) {};
};

class FileLoader : public System
{
public:
	FileLoader();
	~FileLoader();

	virtual void Update(JOB_TYPES j, bool & flag, BaseContent* ptr);
	virtual void Close();

	void ObjImporter(BaseContent * ptr);

	void loadTextData(BaseContent* ptr);

	void individualGameObject(BaseContent * ptr);

	GLfloat parseFloat(const std::string & str);

	GLuint parseInt(const std::string & str);

	void split(const std::string & s, char delim, std::vector<std::string> & out);
	
	void addModel(std::pair<std::string, Model_Loaded*> pair);

	Model_Loaded * checkForModel(const std::string & s);

	void findLoadItem(const std::string & item, const std::string & data, std::map<LOADABLE_ITEMS, std::string>&, std::vector<Component*> * c = nullptr);
	
	std::vector<GLfloat> combine(std::vector<GLuint> faces, std::vector<GLfloat> vert, std::vector<GLfloat> norm, std::vector<GLfloat> text, std::vector<GLuint> & ind);

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

	template <typename Out>
	void split(const std::string &s, char delim, Out result)
	{
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::map<std::string, Model_Loaded*> _loadedModels;
	size_t modelsToLoad = 0;
};

