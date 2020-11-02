#pragma once
#include <vector>
#include <string>
#include "Item.h"

using std::string;
using std::vector;

class Player {
public:
	void take_item(Item* item);
	Item* drop_item(string item);
	string get_items_string();

	int get_health() const { return health; }
	void take_damage(int d, bool block);
	int get_damage(bool bonus) const;
	void set_health(int n) { health = n; }

	// could be done by reference instead of return by value.
	vector<Item*> get_inventory() const { return inventory; }

private:
	vector<Item*> inventory;
	int health{ 10 };
	int attack{ 1 };
	int defense{ 0 };
};