#pragma once
class Component
{
public:
	Component(std::string id) : _typeID{ id } {}
	virtual ~Component() {}

	std::string getID() { return _typeID; }

private:
	std::string _typeID;
};
