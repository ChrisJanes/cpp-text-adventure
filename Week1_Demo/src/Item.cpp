#include "../headers/Item.h"

bool Item::CanUse(Room* currentRoom) const
{
	if(currentRoom != m_TargetRoom)
		return false;

	return true;
}

std::ostream& operator<<(std::ostream& ost, CombatItem& item)
{
    const int attack = item.GetAttackBonus();
    const int defense = item.GetDefenseBonus();
    const int health = item.GetHealthBonus();
	return ost << item.get_item_name() << ", " << item.get_item_description() << "\n\tattack: " << attack << "\n\tdefense: " << defense << "\n\thealth: " << health;
}

std::ostream& operator<<(std::ostream& ost, Item& item)
{
	return ost << item.get_item_name() << ", " << item.get_item_description() << '\n';
}