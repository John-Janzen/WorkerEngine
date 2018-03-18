#pragma once
#include <iostream>
#include <sstream>
#include <ostream>
#include <fstream>
#include <vector>
#include <map>

#include "System.h"

class FileLoader : public System
{
public:
	FileLoader();
	~FileLoader();

	virtual void Update(JOB_TYPES j, void* ptr);
	virtual void Close();
	void loadTextData(void * ptr);

	void readGameObject(std::string type, std::string data, GameObject * & go);
};

