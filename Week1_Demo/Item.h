#pragma once
#include <string>

class Item
{
public:
	Item(std::string n, std::string d) : name(n), description(d) {}

	std::string name;
	std::string description;
};