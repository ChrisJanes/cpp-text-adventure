#include "Item.h"

bool Item::can_use(Room* currentRoom)
{
	if(currentRoom != target_room)
		return false;

	return true;
}

std::ostream& operator<<(std::ostream& ost, CombatItem& item)
{
	int attack = item.get_attack_bonus();
	int defense = item.get_defense_bonus();
	int health = item.get_health_bonus();
	return ost << item.get_item_name() << ", " << item.get_item_description() << "\n\tattack: " << attack << "\n\tdefense: " << defense << "\n\thealth: " << health;
}

std::ostream& operator<<(std::ostream& ost, Item& item)
{
	return ost << item.get_item_name() << ", " << item.get_item_description() << '\n';
}