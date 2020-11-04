#include "../headers/Player.h"
#include <algorithm>


string Player::GetItems() 
{
	string itemList;
	for (uint32_t i = 0; i < m_Inventory.size(); ++i)
	{
		itemList += m_Inventory[i]->get_item_name();

		if (i < m_Inventory.size() - 1)
			itemList += ", ";
	}

	return itemList;
}

void Player::TakeDamage(int d, bool block)
{
	int damage = d;
	if ( block )
		damage = std::max(0, d - m_Defense);
	m_Health = std::max(0, m_Health - damage);
}

int Player::GetDamage(bool bonus) const
{
	int damage = m_Attack;

	if (bonus)
	{
		for (uint32_t i = 0; i < m_Inventory.size(); ++i)
		{
			Item* current = m_Inventory[i];
			CombatItem* item = dynamic_cast<CombatItem*>(current);
			if (item != nullptr) {
				damage += item->GetAttackBonus();
			}
		}
	}

	return damage;
}

void Player::TakeItem(Item* item)
{
	m_Inventory.push_back(item);
}

Item* Player::DropItem(string item)
{
	Item* toDrop = nullptr;
	int dropIndex = -1;
	for (uint32_t i = 0; i < m_Inventory.size(); ++i)
	{
		if (m_Inventory[i]->get_item_name() == item)
		{
			toDrop = m_Inventory[i];
			dropIndex = i;
			break;
		}
	}

	if (toDrop == nullptr) return toDrop;

	m_Inventory.erase(m_Inventory.begin() + dropIndex);

	return toDrop;
}