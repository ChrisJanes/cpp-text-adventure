#pragma once

#include <string>
#include <vector>
#include <iostream>

struct ExitFile {
	int id;
	char dir;
};

std::istream& operator>>(std::istream& is, ExitFile& e);

struct ItemId {
	int id;
};

std::istream& operator>>(std::istream& is, ItemId& e);

struct EnemyId {
	int id;
};

std::istream& operator>>(std::istream& is, EnemyId& e);

struct RoomFile {
	int id;
	std::string description;
	std::vector<ExitFile> exits;
	std::vector<int> items;
	std::vector<int> enemies;
};

std::istream& operator>>(std::istream& is, RoomFile& r);

struct CombatStats {
	int attack{ 0 };
	int defense{ 0 };
	int health{ 0 };
};

std::istream& operator>>(std::istream& is, CombatStats& c);

struct ItemFile {
	int id;
	std::string name;
	std::string description;
	int room_id;
	std::string use_text;
	int use_id;
	bool victory{ false };
	bool isCombat{ false };
	CombatStats combat;
};

std::istream& operator>>(std::istream& is, ItemFile& i);

struct EnemyFile {
	int id;
	std::string name;
	std::string description;
	CombatStats combat;
	int drop_id;
};

std::istream& operator>>(std::istream& is, EnemyFile& e);