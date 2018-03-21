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

class FileLoader : public System
{
public:
	FileLoader();
	~FileLoader();

	virtual void Update(JOB_TYPES j, BaseContent* ptr);
	virtual void Close();
	RenderComponent * ObjImporter(std::string path, RenderComponent * rc);
	void loadTextData(BaseContent* ptr);

	std::vector<std::string> split(const std::string & s, char delim);
	std::vector<GLfloat> combine(std::vector<GLuint> faces, std::vector<GLfloat> vert, std::vector<GLfloat> norm, std::vector<GLfloat> text, std::vector<GLuint> & ind);
	template<typename Out>
	Out * mallocSpace(std::vector<Out>);
	template <typename Out>
	void split(const std::string &s, char delim, Out result);
};

