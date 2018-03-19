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
	void ObjImporter(BaseContent* ptr);
	void loadTextData(BaseContent* ptr);

	std::vector<std::string> split(const std::string & s, char delim);
	template <typename Out>
	void split(const std::string &s, char delim, Out result);
};

