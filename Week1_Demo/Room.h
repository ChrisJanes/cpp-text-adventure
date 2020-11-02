#pragma once
#include <vector>
#include <string>
#include "Item.h"
#include "Player.h"
#include "Enemy.h"

using std::string;

class RoomExit;

const vector<string> NorthDir{ "north", "n" };
const vector<string> SouthDir{ "south", "s" };
const vector<string> EastDir{ "east", "e" };
const vector<string> WestDir{ "west", "w" };
const vector<string> QuitCmd{ "quit", "q", "exit" };
const vector<string> TakeCmd{ "take", "t", "pickup", "pick-up" };
const vector<string> DropCmd{ "drop", "d", "throw" };
const vector<string> UseCmd{ "use", "u" };
const vector<string> InvCmd{ "inventory", "i", "bag", "hands" };
const vector<string> CombatCmd{ "fight", "hit", "f" };
const vector<string> LookCmd{ "look", "l", "study" };

const string fight = "fight";
const string block = "block";
const string flee = "flee";
const string room = "room";

enum class FightResult
{
	Victory, Loss, Flee
};

class Room
{
public:
	Room() {}
	Room(int i, string desc) : id{ i }, description{ desc } {}

	void AddEnemy(Enemy* en);
	void AddExit(string dir, Room &room);
	void AddItem(Item* item);
	Room* RunCommands(Player &player);
	bool shouldQuit{ false };
	bool hasWon{ false };
	void PrintDescription();
private:
	int id;
	FightResult handleCombat(Player& player);

	void getCommand();
	bool print_item(Item* item, string name);

	Enemy* enemy{ nullptr };

	string commandString;
	string formatExits();
	string description;
	std::vector<RoomExit> exits;
	std::vector<Item*> inventory;
	bool isExplored{ false };
};