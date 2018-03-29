#pragma once
#include <SDL.h>

#include "GameObject.h"

class BaseContent
{
public:
	virtual ~BaseContent() {};
};

class EngineObjectContent : public BaseContent
{
public:
	GameObject * obj;
	int width, height;

	EngineObjectContent(GameObject * o, int w, int h) : obj {o}, width (w), height (h) {}
	~EngineObjectContent() { obj = nullptr; }
};

class RenderCameraContent : public BaseContent
{
public:
	float moveX, moveY, moveZ;

	RenderCameraContent(float mX, float mY, float mZ) : moveX(mX), moveY(mY), moveZ(mZ) {}
	~RenderCameraContent() {}
};

class RenderLoadContent : public BaseContent
{
public:
	std::vector<GameObject*> * objects;
	GameObject * camera;

	RenderLoadContent(std::vector<GameObject*> * o, GameObject * c) : objects{ o }, camera{c} { }
	~RenderLoadContent() { objects = nullptr; camera = nullptr; }
};

class RenderUpdateContent : public BaseContent
{
public:
	std::vector<GameObject*> * objects;

	RenderUpdateContent(std::vector<GameObject*> * o) : objects{ o } { }
	~RenderUpdateContent() { objects = nullptr; }
};

class InputContent : public BaseContent
{
public:
	SDL_Event * Event;

	InputContent(SDL_Event * e) : Event {e} {}
	~InputContent() { Event = nullptr; }
};

class FileToLoadContent : public BaseContent
{
public:
	std::string path;

	FileToLoadContent(std::string s) : path{ s } {}
	~FileToLoadContent() {}
};

class FileLoadOBJContent : public BaseContent
{
public:
	std::string data, name;

	FileLoadOBJContent(std::string p, std::string n) : data(p), name(n) {}
	~FileLoadOBJContent() {}
};

class FileLoadedContent : public BaseContent
{
public:
	GameObject * object;

	FileLoadedContent(GameObject * o) : object { o } { }
	~FileLoadedContent() { object = nullptr; }
};

class FileIndividualContent : public BaseContent
{
public:
	std::string info;

	FileIndividualContent(std::string vs) : info { vs } {}
	~FileIndividualContent() {}
};

class IntPassContent : public BaseContent
{
public :
	int num;

	IntPassContent(int n) : num (n) {}
	~IntPassContent() {}
};