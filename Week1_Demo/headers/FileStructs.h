#pragma once

#include <string>
#include <vector>
#include <iostream>

struct ExitFile {
	int M_Id;
	char M_Dir;
};

std::istream& operator>>(std::istream& is, ExitFile& e);

struct ItemId {
	int M_Id;
};


std::istream& operator>>(std::istream& is, ItemId& e);

struct EnemyId {
	int M_Id;
};

std::istream& operator>>(std::istream& is, EnemyId& e);

struct RoomFile {
	int M_Id;
	std::string description;
	std::vector<ExitFile> exits;
	std::vector<int> items;
	std::vector<int> enemies;
};

std::istream& operator>>(std::istream& is, RoomFile& r);

struct CombatStats {
	int M_Attack{ 0 };
	int M_Defense{ 0 };
	int M_Health{ 0 };
};

std::istream& operator>>(std::istream& is, CombatStats& c);

struct ItemFile {
	int M_Id;
	std::string M_Name;
	std::string M_Description;
	int M_RoomId;
	std::string M_UseText;
	int M_UseId;
	bool M_Victory{ false };
	bool M_IsCombat{ false };
	CombatStats M_Combat;
};

std::istream& operator>>(std::istream& is, ItemFile& i);

struct EnemyFile {
	int M_Id;
	std::string M_Name;
	std::string M_Description;
	CombatStats M_Combat;
	int M_DropId;
};

std::istream& operator>>(std::istream& is, EnemyFile& e);