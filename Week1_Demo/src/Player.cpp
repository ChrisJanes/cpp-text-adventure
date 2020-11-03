#include "../headers/Player.h"
#include <algorithm>


string Player::GetItems() 
{
	string itemList;
	for (uint32_t i = 0; i < M_Inventory.size(); ++i)
	{
		itemList += M_Inventory[i]->get_item_name();

		if (i < M_Inventory.size() - 1)
			itemList += ", ";
	}

	return itemList;
}

void Player::TakeDamage(int d, bool block)
{
	int damage = d;
	if ( block )
		damage = std::max(0, d - M_Defense);
	M_Health = std::max(0, M_Health - damage);
}

int Player::GetDamage(bool bonus) const
{
	int damage = M_Attack;

	if (bonus)
	{
		for (uint32_t i = 0; i < M_Inventory.size(); ++i)
		{
			Item* current = M_Inventory[i];
			CombatItem* item = dynamic_cast<CombatItem*>(current);
			if (item != nullptr) {
				damage += item->get_attack_bonus();
			}
		}
	}

	return damage;
}

void Player::TakeItem(Item* item)
{
	M_Inventory.push_back(item);
}

Item* Player::DropItem(string item)
{
	Item* toDrop = nullptr;
	int dropIndex = -1;
	for (uint32_t i = 0; i < M_Inventory.size(); ++i)
	{
		if (M_Inventory[i]->get_item_name() == item)
		{
			toDrop = M_Inventory[i];
			dropIndex = i;
			break;
		}
	}

	if (toDrop == nullptr) return toDrop;

	M_Inventory.erase(M_Inventory.begin() + dropIndex);

	return toDrop;
}