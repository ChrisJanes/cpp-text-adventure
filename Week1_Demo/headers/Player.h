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

	int GetHealth() const { return M_Health; }
	void TakeDamage(int d, bool block);
	int GetDamage(bool bonus) const;
	void SetHealth(int n) { M_Health = n; }

	vector<Item*> get_inventory() const { return M_Inventory; }

private:
	vector<Item*> M_Inventory;
	int M_Health{ 10 };
	int M_Attack{ 1 };
	int M_Defense{ 0 };
};