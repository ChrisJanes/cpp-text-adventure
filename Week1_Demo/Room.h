#pragma once
#include <vector>
#include <string>
#include "Item.h"
#include "Player.h"

using std::string;

class RoomExit;

class Room
{
public:
	Room() : roomItem(nullptr) {}
	Room(string desc) : description(desc), roomItem(nullptr) {}

	void AddExit(string dir, Room &room);
	void AddItem(Item* item);
	Room* RunCommands(Player &player);
	bool shouldQuit;
	// You find yourself in a dark hallway, there are exits to the north and east.
	void PrintDescription();
private:
	void getCommand();
	
	string commandString;
	string formatExits();
	string description;
	std::vector<RoomExit> exits;
	Item *roomItem;
	bool isExplored{ false };
};