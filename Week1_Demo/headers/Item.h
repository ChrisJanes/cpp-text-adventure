#pragma once
#include <string>
#include <iostream>

class Room;

enum class USE_RESULT
{
	CONSUME, VICTORY
};

class Item
{
public:
	Item(const int i, std::string n, std::string d) : m_Id{ i }, m_Name(std::move(n)), m_Description(std::move(d)) {};

	Item(int i, std::string n, std::string d, std::string u)
		: m_Id{ i }, m_Name(std::move(n)), m_Description(std::move(d)), m_UseText(std::move(u)) {};
	
	virtual ~Item() = default;

    [[nodiscard]] std::string get_item_name() const { return m_Name; }
	[[nodiscard]] std::string get_item_description() const { return m_Description; }
	[[nodiscard]] std::string get_use_text() const { return m_UseText; }
	 void set_target_room(Room* rm) { m_TargetRoom = rm; }

	bool CanUse(Room* currentRoom) const;
	[[nodiscard]] USE_RESULT GetUseResult() const { return m_UseResult; }
	void MakeVictoryItem() { m_UseResult = USE_RESULT::VICTORY; }
	[[nodiscard]] Item* GetResultItem() const { return m_UseResultItem; }
	void SetResultItem(Item* itm) { m_UseResultItem = itm; }
	void SetUseText(std::string& text) { m_UseText = text; }

private:
	int m_Id;
	std::string m_Name;
	std::string m_Description;
	std::string m_UseText{ " " };
	Room* m_TargetRoom{ nullptr };
	Item* m_UseResultItem{ nullptr };
	USE_RESULT m_UseResult{ USE_RESULT::CONSUME };
};

class CombatItem final: public Item
{
public:
	CombatItem(const int i, const std::string n, const std::string d, const int attack, const int health, const int defense) 
		: Item(i, n, d), m_AttackBonus(attack), m_HealthBonus(health), m_DefenseBonus(defense) {}

 	[[nodiscard]] int GetAttackBonus() const { return m_AttackBonus; }
	[[nodiscard]] int GetDefenseBonus() const { return m_DefenseBonus; }
	[[nodiscard]] int GetHealthBonus() const { return m_HealthBonus; }

private:	
	int m_AttackBonus{ 0 };
	int m_HealthBonus{ 0 };
	int m_DefenseBonus{ 0 };
};

std::ostream& operator<<(std::ostream& ost, CombatItem& item);
std::ostream& operator<<(std::ostream& ost, Item& item);