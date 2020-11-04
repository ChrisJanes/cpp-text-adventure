#pragma once
#include "Item.h"
#include <string>
#include <utility>

using std::string;

class Enemy
{
public:
	Enemy(string n, string d) : m_Name(std::move(n)), m_Description(std::move(d)) {}

	Enemy(string n, string des, int h, int d, int df) 
		: m_Name(std::move(n)), m_Description(std::move(des)), m_Health(h), m_Damage(d), m_Defense(df) {}

	Enemy(string n, string des, int h, int d, int df, Item* it) 
		:m_DropItem(it), m_Name(std::move(n)), m_Description(std::move(des)), m_Health(h),  m_Damage(d), m_Defense(df) {}


    [[nodiscard]] string GetDescription() const { return m_Description; }
	[[nodiscard]] string GetName() const { return m_Name; }
	[[nodiscard]] Item* GetDropItem() const   { return m_DropItem; }
	[[nodiscard]] int GetHealth() const { return m_Health; }
	[[nodiscard]] int GetDamage() const { return m_Damage; }
	[[nodiscard]] int GetDefense() const { return m_Defense; }
	[[nodiscard]] void SetHealth(int n) { m_Health = n; }

	void TakeDamage(int d);
	bool IsDead() const { return m_Health == 0; }

private:
	Item* m_DropItem{ nullptr };
	string m_Name;
	string m_Description;
	int m_Health{ 1 };
	int m_Damage{ 1 };
	int m_Defense{ 0 };
};

