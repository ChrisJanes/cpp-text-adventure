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

	int get_health() const { return health; }
	void take_damage(int d, bool block);
	int get_damage(bool bonus) const;
	void set_health(int n) { health = n; }

	vector<Item*> get_inventory() const { return inventory; }

private:
	vector<Item*> inventory;
	int health{ 10 };
	int attack{ 1 };
	int defense{ 0 };
};