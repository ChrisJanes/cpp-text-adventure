#pragma once
#include <vector>
#include <string>
#include "Item.h"

using std::string;
using std::vector;

class Player {
public:
	void TakeItem(Item* item);
	Item* DropItem(string item);
	string GetItems();
private:
	vector<Item*> inventory;
};