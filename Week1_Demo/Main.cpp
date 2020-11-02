#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>

#include "headers/Room.h"
#include "headers/RoomExit.h"
#include "headers/Player.h"
#include "headers/Item.h"
#include "headers/Enemy.h"

#include "headers/FileStructs.h"

/*
5--3--6
   |
   1--2
 	  |
	  4
*/

using std::vector;
using std::cout;
using std::cin;

void gameOver(string message)
{
	cout << "Game Over\n" << message << '\n';
}

void printSeparator()
{
	cout << "\n*********************************************************************\n";
}

// should probably move this somewhere else.
struct Exit : std::runtime_error {
	Exit() : std::runtime_error("Exit") {}
};

// error() simply disguises throws:
inline void error(const string& s)
{
	throw std::runtime_error(s);
}

// load file data for enemies, items and rooms.
// these functions just load intermediary data which must then be processed to produce actual objects

void loadEnemies(const string &filename, std::vector<EnemyFile> &enemyData) 
{
	std::ifstream is{ filename };
	if (!is) error("failed to load enemies file");

	while (is)
	{
		EnemyFile e;
		if (is >> e) enemyData.push_back(e);
	}
}

void loadItems(const string& filename, std::vector<ItemFile>& itemData) 
{
	std::ifstream is{ filename };
	if (!is) error("failed to load items file");

	while (is)
	{
		ItemFile f;
		if (is >> f) itemData.push_back(f);
	}
}

void loadRooms(const string& filename, std::vector<RoomFile>& roomData) 
{
	std::ifstream is{ filename };
	if (!is) error("failed to load room file");

	while (is)
	{
		RoomFile r;
		if(is >> r) roomData.push_back(r);
	}
}

vector<Room> rooms;

// use the intermediary data loaded from the files and build all the required objects

void buildRooms(std::vector<RoomFile>& roomData, std::vector<ItemFile>& itemData, std::vector<EnemyFile>& enemyData )
{
	// create all the items first, this is just a first pass to construct the initial objects
	vector<Item*> items;
	for (ItemFile itemLoad : itemData)
	{
		// instantiate the right sort of item object
		if (itemLoad.isCombat)
		{
			CombatItem* item = new CombatItem{ itemLoad.id, itemLoad.name, itemLoad.description, itemLoad.combat.attack, itemLoad.combat.health, itemLoad.combat.defense };
			items.push_back(item);
		}
		else {
			Item* item = new Item{itemLoad.id, itemLoad.name, itemLoad.description, itemLoad.use_text};
			if (itemLoad.victory) item->make_victory_item();
			items.push_back(item);
		}
	}

	// once all the items are created, re-iterate the item data to setup the "use result" links (Items that create other Items when used).
	for (ItemFile itemLoad : itemData)
	{
		if (itemLoad.isCombat || itemLoad.use_id == 0) continue;

		Item* currentItem = items[itemLoad.id - 1];
		currentItem->set_result_Item(items[itemLoad.use_id - 1]);
	}

	// iterate all the rooms and create objects
	for (RoomFile room : roomData)
	{
		Room newRoom{ room.id, room.description };
		// add enemies where necessary - the current Room object only allows for a single enemy, the file format supports multiple enemies, go figure.
		for (int enemy_id : room.enemies)
		{
			for (EnemyFile enemyStats : enemyData)
			{
				Item* drop_item = enemyStats.drop_id == 0 ? nullptr : items[enemyStats.drop_id - 1];
				Enemy* enemy = new Enemy{ enemyStats.name, enemyStats.description, enemyStats.combat.health, enemyStats.combat.attack, enemyStats.combat.defense, drop_item };
				newRoom.add_enemy(enemy);
			}
		}

		// refer to the existing items to add the right items to rooms
		for (int item_id : room.items)
		{
			newRoom.add_item(items[item_id - 1]);
		}
		rooms.push_back(newRoom);
	}

	// now all the rooms are built, we can create the exit links by using the id reference values
	for (RoomFile room : roomData)
	{
		Room* currentRoom = &rooms[room.id - 1];

		for (ExitFile exit : room.exits)
		{
			Room* exitRoom = &rooms[exit.id - 1];
			std::string direction;

			switch (exit.dir) 
			{
			case 'n': direction = "north"; break;
			case 'e': direction = "east"; break;
			case 's': direction = "south"; break;
			case 'w': direction = "west"; break;
			}

			currentRoom->add_exit(direction, *exitRoom);
		}
	}

	// and we can setup usable items target room values
	for (ItemFile itemLoad : itemData)
	{
		if (itemLoad.room_id == 0) continue;

		Item* currentItem = items[itemLoad.id - 1];
		currentItem->set_target_room(&rooms[itemLoad.room_id - 1]);
	}
}

int main()
{
	Player player;
	Room* currentRoom = nullptr;
	// try to load data files
	try {
		std::vector<RoomFile> roomData;
		std::vector<ItemFile> itemData;
		std::vector<EnemyFile> enemyData;

		loadRooms("rooms.txt", roomData);
		loadItems("items.txt", itemData);
		loadEnemies("enemies.txt", enemyData);

		buildRooms(roomData, itemData, enemyData);
	}
	catch (std::exception& e)
	{
		// we bail out if it goes wrong, we could instantiate a default room instead?
		cout << e.what() << '\n';
		return 1;
	}

	// starting room is always the first room loaded
	currentRoom = &rooms[0];

	// game loop
	while (true)
	{
		printSeparator();
		currentRoom->print_description();
		Room* newRoom = currentRoom->run_commands(player);

		if (currentRoom->should_quit || currentRoom->has_won)
			break;

		if (newRoom != nullptr)
		{
			system("cls");
			currentRoom = newRoom;
		}
	}

	if (currentRoom->has_won)
		gameOver("You won!");
	else
		gameOver("Thanks for playing");

	return 0;
}