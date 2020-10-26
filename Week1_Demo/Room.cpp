#include "Room.h"
#include "RoomExit.h"

#include <iostream>

using std::cout;
using std::cin;

void Room::getCommand()
{
	cout << "What do you want to do?\n";
	cin >> commandString;
	//if (!cin) error("error handling user input.\n");
}

void Room::AddExit(string dir, Room &room)
{
	RoomExit exit{ dir, room };
	exits.push_back(exit);
}

Room* Room::RunCommands(Player& player)
{
	shouldQuit = false;
	commandString = "";
	getCommand();
	bool handledCommand = false;

	if (commandString == "") return nullptr;

	for (RoomExit exit : exits) {
		if (commandString == exit.direction)
		{
			return &exit.exitLocation;
		}
	}

	if (commandString == "take")
	{
		handledCommand = true;
		if (roomItem == nullptr)
			cout << "There is nothing here to take\n";
		else
		{
			cout << "You have taken the " << roomItem->name << '\n';
			player.TakeItem(roomItem);
			roomItem = nullptr;
		}
		
		return nullptr;
	}

	if (commandString == "quit")
	{
		shouldQuit = true;
		return nullptr;
	}

	if (commandString == "drop")
	{
		handledCommand = true;

		if ( roomItem != nullptr) 
		{
			cout << "There is already something in this room, you cannot drop anything further here.\n";
			return nullptr;
		}

		string item;
		cout << "You are holding: " << player.GetItems() << "\n";
		cout << "What do you want to drop?\n";
		cin >> item;

		Item* toDrop = player.DropItem(item);
		if (toDrop == nullptr)
		{
			cout << "You are not holding that item.\n";
			return nullptr;
		}
		else 
		{
			roomItem = toDrop;
		}
	}

	if (handledCommand == false)
		cout << "Unknown command\n";
	
	return nullptr;
}

void Room::AddItem(Item* item) 
{
	if(roomItem == nullptr)
		roomItem = item;
}

void Room::PrintDescription() {
	cout << "You find yourself in a " << description << ", " << formatExits() << '\n';
	if (roomItem != nullptr)
		cout << "There is a " << roomItem->name << " here, " << roomItem->description << '\n';
}

string Room::formatExits() {
	if (exits.size() > 1)
	{
		string exitStr = "there are exits to the ";
		for (int i = 0; i < exits.size(); ++i)
		{
			if (i == exits.size() - 1)
				exitStr += " and ";

			exitStr += exits[i].direction;

			if (exits.size() > 2 &&  i < exits.size() - 2)
				exitStr += ", ";
		}
		return exitStr;
	}
	else {
		return "there is an exit to the " + exits[0].direction;
	}
}