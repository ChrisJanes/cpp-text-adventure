#include "../headers/Player.h"
#include <algorithm>


string Player::get_items_string()
{
	// builds a string of all the items in the players inventory
	string itemList = "";
	for (int i = 0; i < inventory.size(); ++i)
	{
		itemList += inventory[i]->get_item_name();

		// add a comma if it's not the last item
		if (i < inventory.size() - 1)
			itemList += ", ";
	}

	return itemList;
}

void Player::take_damage(int d, bool block)
{
	int damage = d;
	// only reduce damage by defense if this is a block move
	if ( block )
		damage = std::max(0, d - defense);
	health = std::max(0, health - damage);
}

int Player::get_damage(bool bonus) const
{
	int damage = attack;

	// accumulate damage from items when asked for
	if (bonus)
	{
		for (int i = 0; i < inventory.size(); ++i)
		{
			Item* current = inventory[i];
			CombatItem* item = dynamic_cast<CombatItem*>(current);
			if (item != nullptr) damage += item->get_attack_bonus();
		}
	}

	return damage;
}

void Player::take_item(Item* item)
{
	inventory.push_back(item);
}

Item* Player::drop_item(string item)
{
	// find the item by name and store the index
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

	// if we found an item, remove it from the inventory
	inventory.erase(inventory.begin() + dropIndex);

	return toDrop;
}