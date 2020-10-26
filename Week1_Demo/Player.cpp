#include "Player.h"

string Player::GetItems() 
{
	string itemList = "";
	for (int i = 0; i < inventory.size(); ++i)
	{
		itemList += inventory[i]->name;

		if (i < inventory.size() - 1)
			itemList += ", ";
	}

	return itemList;
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
		if (inventory[i]->name == item)
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