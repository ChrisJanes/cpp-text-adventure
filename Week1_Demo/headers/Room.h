#pragma once
#include <vector>
#include <string>
#include "Item.h"
#include "Player.h"
#include "Enemy.h"

using std::string;

class RoomExit;


// these could be tidied up, potentially put them into a map?
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

// feels like combat could be moved into it's own class somewhere, Room might be getting too busy.
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

	void add_enemy(Enemy* en);
	void add_exit(string dir, Room &room);
	void add_item(Item* item);
	Room* run_commands(Player &player);
	bool should_quit{ false };
	bool has_won{ false };
	void print_description();
private:
	int id;
	FightResult handle_combat(Player& player);

	void get_command();
	bool print_item(Item* item, string name);

	Enemy* enemy{ nullptr };

	string commandString;
	string formatExits();
	string description;
	std::vector<RoomExit> exits;
	std::vector<Item*> inventory;
	bool isExplored{ false };
};