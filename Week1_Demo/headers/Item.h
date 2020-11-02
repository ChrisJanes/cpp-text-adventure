#pragma once
#include <string>
#include <iostream>

class Room;

enum class UseResult
{
	Consume, Victory
};

class Item
{
public:
	Item(int i, std::string n, std::string d) : id{ i }, name(n), description(d) {}

	Item(int i, std::string n, std::string d, std::string u) 
		: id{ i }, name(n), description(d), use_text(u) {}
	
	virtual ~Item() {};

	std::string get_item_name() const { return name; }
	std::string get_item_description() const { return description; }
	std::string get_use_text() const { return use_text; }
	void set_target_room(Room* rm) { target_room = rm; }

	bool can_use(Room* currentRoom);
	UseResult get_use_result() { return use_result; }
	void make_victory_item() { use_result = UseResult::Victory; }
	Item* get_result_item() { return use_result_item; }
	void set_result_Item(Item* itm) { use_result_item = itm; }
	void set_use_text(std::string& text) { use_text = text; }

private:
	int id;
	std::string name;
	std::string description;
	std::string use_text{ " " };
	Room* target_room{ nullptr };
	Item* use_result_item{ nullptr };
	UseResult use_result{ UseResult::Consume };
};

class CombatItem : public Item
{
public:
	CombatItem(int i, std::string n, std::string d, int attack, int health, int defense) 
		: Item(i, n, d), attack_bonus(attack), health_bonus(health), defense_bonus(defense) {}

	int get_attack_bonus() const { return attack_bonus; }
	int get_defense_bonus() const { return defense_bonus; }
	int get_health_bonus() const { return health_bonus; }

private:	
	int attack_bonus{ 0 };
	int health_bonus{ 0 };
	int defense_bonus{ 0 };
};

std::ostream& operator<<(std::ostream& ost, CombatItem& item);
std::ostream& operator<<(std::ostream& ost, Item& item);