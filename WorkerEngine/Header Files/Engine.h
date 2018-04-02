#pragma once
#include <stdio.h>

#include "System.h"

class Engine : public System
{
public:
	Engine(Application * app);
	~Engine();

	/*
	* The Update function that should be called
	* Currently does nothing
	*/
	virtual void Update(JOB_TYPES t, bool & flag, BaseContent* ptr = nullptr);

	void HandlePlayer(BaseContent * ptr);

	void ChangeScene(BaseContent * ptr);

	/*
	* The Close function that should be called
	* Currently does nothing
	*/
	virtual void Close();

private:
	
};

