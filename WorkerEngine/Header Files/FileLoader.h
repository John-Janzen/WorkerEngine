#pragma once
#include "System.h"
class FileLoader :
	public System
{
public:
	FileLoader();
	~FileLoader();

	virtual void Update(JOB_TYPES j, void* ptr);
	virtual void Close();
};

