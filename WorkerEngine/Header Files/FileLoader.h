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
	void ObjImporter(BaseContent * ptr);
	void loadTextData(BaseContent* ptr);

	void individualGameObject(BaseContent * ptr);

	GLfloat parseFloat(const std::string & str);

	GLuint parseInt(const std::string & str);

	void split(const std::string & s, char delim, std::vector<std::string> & out);
	std::vector<GLfloat> combine(std::vector<GLuint> faces, std::vector<GLfloat> vert, std::vector<GLfloat> norm, std::vector<GLfloat> text, std::vector<GLuint> & ind);
	template<typename Out>
	Out * mallocSpace(std::vector<Out>);
	template <typename Out>
	void split(const std::string &s, char delim, Out result);
};

