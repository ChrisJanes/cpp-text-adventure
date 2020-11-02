#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>

#include "Room.h"
#include "RoomExit.h"
#include "Player.h"
#include "Item.h"
#include "Enemy.h"

#include "FileStructs.h"

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

struct Exit : std::runtime_error {
	Exit() : std::runtime_error("Exit") {}
};

// error() simply disguises throws:
inline void error(const string& s)
{
	throw std::runtime_error(s);
}

void loadEnemies(const string &filename, std::vector<EnemyFile> &enemyData) {
	std::ifstream is{ filename };
	if (!is) error("failed to load enemies file");

	while (is)
	{
		EnemyFile e;
		if (is >> e) enemyData.push_back(e);
	}
}

void loadItems(const string& filename, std::vector<ItemFile>& itemData) {
	std::ifstream is{ filename };
	if (!is) error("failed to load items file");

	while (is)
	{
		ItemFile f;
		if (is >> f) itemData.push_back(f);
	}
}

void loadRooms(const string& filename, std::vector<RoomFile>& roomData) {
	std::ifstream is{ filename };
	if (!is) error("failed to load room file");

	while (is)
	{
		RoomFile r;
		if(is >> r) roomData.push_back(r);
	}
}

vector<Room> rooms;

void buildRooms(std::vector<RoomFile>& roomData, std::vector<ItemFile>& itemData, std::vector<EnemyFile>& enemyData )
{
	vector<Item*> items;
	for (ItemFile itemLoad : itemData)
	{
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

	for (ItemFile itemLoad : itemData)
	{
		if (itemLoad.isCombat || itemLoad.use_id == 0) continue;

		Item* currentItem = items[itemLoad.id - 1];
		currentItem->set_result_Item(items[itemLoad.use_id - 1]);
	}

	for (RoomFile room : roomData)
	{
		Room newRoom{ room.id, room.description };
		for (int enemy_id : room.enemies)
		{
			for (EnemyFile enemyStats : enemyData)
			{
				Item* drop_item = enemyStats.drop_id == 0 ? nullptr : items[enemyStats.drop_id - 1];
				Enemy* enemy = new Enemy{ enemyStats.name, enemyStats.description, enemyStats.combat.health, enemyStats.combat.attack, enemyStats.combat.defense, drop_item };
				newRoom.AddEnemy(enemy);
			}
		}

		for (int item_id : room.items)
		{
			newRoom.AddItem(items[item_id - 1]);
		}
		rooms.push_back(newRoom);
	}

	for (RoomFile room : roomData)
	{
		Room* currentRoom = &rooms[room.id - 1];

		for (ExitFile exit : room.exits)
		{
			Room* exitRoom = &rooms[exit.id - 1];
			std::string direction;

			switch (exit.dir) {
			case 'n': direction = "north"; break;
			case 'e': direction = "east"; break;
			case 's': direction = "south"; break;
			case 'w': direction = "west"; break;
			}

			currentRoom->AddExit(direction, *exitRoom);
		}
	}

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
		cout << e.what() << '\n';
		return 1;
	}

	currentRoom = &rooms[0];

	while (true)
	{
		printSeparator();
		currentRoom->PrintDescription();
		Room* newRoom = currentRoom->RunCommands(player);

		if (currentRoom->shouldQuit)
			break;

		if (currentRoom->hasWon)
			break;

		if (newRoom != nullptr)
		{
			system("cls");
			currentRoom = newRoom;
		}
	}

	if (currentRoom->hasWon)
		gameOver("You won!");
	else
		gameOver("Thanks for playing");

	return 0;
}