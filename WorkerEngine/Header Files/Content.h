#pragma once
#include <SDL.h>

class BaseContent
{
public:
	virtual ~BaseContent() {};
};

class RenderCameraContent : public BaseContent
{
public:
	GameObject * camera;

	RenderCameraContent(GameObject * c) : camera{c} {}
	~RenderCameraContent()
	{ 
		camera = nullptr; 
	}
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

class FileLoadedContent : public BaseContent
{
public:
	std::vector<GameObject*> _objects;

	FileLoadedContent(std::vector<GameObject*> o) { _objects = o; }
	~FileLoadedContent() {}
};