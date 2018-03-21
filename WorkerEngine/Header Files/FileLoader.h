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

#include "System.h"
#include "Manager.h"

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

	virtual void Update(JOB_TYPES j, BaseContent* ptr);
	virtual void Close();

	void ObjImporter(BaseContent * ptr);

	void loadTextData(BaseContent* ptr);

	void individualGameObject(BaseContent * ptr);

	GLfloat parseFloat(const std::string & str);

	GLuint parseInt(const std::string & str);

	void split(const std::string & s, char delim, std::vector<std::string> & out);
	
	void addModel(std::pair<std::string, Model_Loaded*> pair);

	Model_Loaded * checkForModel(const std::string & s);

	LOADABLE_ITEMS findLoadItem(const std::string & item);
	
	std::vector<GLfloat> combine(std::vector<GLuint> faces, std::vector<GLfloat> vert, std::vector<GLfloat> norm, std::vector<GLfloat> text, std::vector<GLuint> & ind);
	
	template<typename Out>
	Out * mallocSpace(std::vector<Out>);

	template <typename Out>
	void split(const std::string &s, char delim, Out result);

private:
	std::map<std::string, Model_Loaded*> _loadedModels;
};

