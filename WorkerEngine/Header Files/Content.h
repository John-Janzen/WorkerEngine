#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "../Scenes/Header Files/SceneTypes.h"

class BaseContent
{
public:
	virtual ~BaseContent() {};
};

class ChangeSceneContent : public BaseContent
{
public:
	SCENE sceneName;

	ChangeSceneContent(const SCENE & n) : sceneName(n) {}
	~ChangeSceneContent() {}
};

class EngineObjectContent : public BaseContent
{
public:
	GameObject * obj;

	EngineObjectContent(GameObject * o) : obj {o} {}
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
	std::vector<GameObject*> objects;
	GameObject * camera;

	RenderLoadContent(std::vector<GameObject*> o, GameObject * c) : objects{ o }, camera{c} { }
	~RenderLoadContent() { objects.clear(); camera = nullptr; }
};

class RenderUpdateContent : public BaseContent
{
public:
	std::vector<GameObject*> objects;
	GameObject * camera;

	RenderUpdateContent(std::vector<GameObject*> o, GameObject * cam) : objects{ o }, camera{cam} { }
	~RenderUpdateContent() { objects.clear(); camera = nullptr; }
};

class InputContent : public BaseContent
{
public:
	SDL_Event Event;

	InputContent(SDL_Event e) : Event {e} {}
	~InputContent() {}
};

class InputIPContent : public BaseContent
{
public:
	GameObject * player;

	InputIPContent(GameObject * p) : player{ p } {}
	~InputIPContent() { player = nullptr; }
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