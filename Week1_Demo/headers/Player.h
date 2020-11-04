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

	[[nodiscard]] int GetHealth() const { return m_Health; }
	void TakeDamage(int d, bool block);
	[[nodiscard]] int GetDamage(bool bonus) const;
	void SetHealth(const int n) { m_Health = n; }

	[[nodiscard]] vector<Item*> GetInventory() const { return m_Inventory; }

private:
	vector<Item*> m_Inventory;
	int m_Health{ 10 };
	int m_Attack{ 1 };
	int m_Defense{ 0 };
};