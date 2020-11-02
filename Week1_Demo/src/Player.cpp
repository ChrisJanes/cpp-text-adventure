#include "../headers/Player.h"
#include <algorithm>


string Player::GetItems() 
{
	string itemList = "";
	for (int i = 0; i < inventory.size(); ++i)
	{
		itemList += inventory[i]->get_item_name();

		if (i < inventory.size() - 1)
			itemList += ", ";
	}

	return itemList;
}

void Player::take_damage(int d, bool block)
{
	int damage = d;
	if ( block )
		damage = std::max(0, d - defense);
	health = std::max(0, health - damage);
}

int Player::get_damage(bool bonus) const
{
	int damage = attack;

	if (bonus)
	{
		for (int i = 0; i < inventory.size(); ++i)
		{
			Item* current = inventory[i];
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
	inventory.push_back(item);
}

Item* Player::DropItem(string item)
{
	Item* toDrop = nullptr;
	int dropIndex = -1;
	for (int i = 0; i < inventory.size(); ++i)
	{
		if (inventory[i]->get_item_name() == item)
		{
			toDrop = inventory[i];
			dropIndex = i;
			break;
		}
	}

	if (toDrop == nullptr) return toDrop;

	inventory.erase(inventory.begin() + dropIndex);

	return toDrop;
}