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

void game_over(const string &message)
{
	cout << "Game Over\n" << message << '\n';
}

void print_separator()
{
	cout << "\n*********************************************************************\n";
}

struct Exit final: std::runtime_error
{
	Exit() : std::runtime_error("Exit") {}
};

// error() simply disguises throws:
inline void error(const string& s)
{
	throw std::runtime_error(s);
}


template<typename t>
void load_data_from_file(const string& filename, std::vector<t>& Data, const std::string& message)
{
	std::ifstream is{ filename };
	if (!is) error(message);

	while (is)
	{
		t r;
		if (is >> r) Data.push_back(r);
	}
}

void load_enemies(const string &filename, std::vector<EnemyFile> &enemyData)
{
	/*std::ifstream is{ filename };
	if (!is) error("failed to load enemies file");

	while (is)
	{
		EnemyFile e;
		if (is >> e) enemyData.push_back(e);
	}*/
	load_data_from_file<EnemyFile>(filename, enemyData, "failed to load enemies file");
}

void load_items(const string& filename, std::vector<ItemFile>& itemData)
{
	/*std::ifstream is{ filename };
	if (!is) error("failed to load items file");

	while (is)
	{
		ItemFile f;
		if (is >> f) itemData.push_back(f);
	}*/
	load_data_from_file<ItemFile>(filename, itemData, "failed to load item file");
}

void load_rooms(const string& filename, std::vector<RoomFile>& roomData)
{
	/*std::ifstream is{ filename };
	if (!is) error("failed to load room file");

	while (is)
	{
		RoomFile r;
		if(is >> r) roomData.push_back(r);
	}*/
	load_data_from_file<RoomFile>(filename, roomData, "failed to load room file");
}



vector<Room> ROOMS;

void build_rooms(std::vector<RoomFile>& roomData, std::vector<ItemFile>& itemData, std::vector<EnemyFile>& enemyData )
{
	vector<Item*> items;
	for (const ItemFile& itemLoad : itemData)
	{
		if (itemLoad.M_IsCombat)
		{
			CombatItem* item = new CombatItem{ itemLoad.M_Id, itemLoad.M_Name, itemLoad.M_Description, itemLoad.M_Combat.M_Attack, itemLoad.M_Combat.M_Health, itemLoad.M_Combat.M_Defense };
			items.push_back(item);
		}
		else {
			Item* item = new Item{itemLoad.M_Id, itemLoad.M_Name, itemLoad.M_Description, itemLoad.M_UseText};
			if (itemLoad.M_Victory) item->MakeVictoryItem();
			items.push_back(item);
		}
	}

	for (const ItemFile& itemLoad : itemData)
	{
		if (itemLoad.M_IsCombat || itemLoad.M_UseId == 0) continue;

		Item* currentItem = items[itemLoad.M_Id - 1];
		currentItem->SetResultItem(items[itemLoad.M_UseId - 1]);
	}

	for (const RoomFile& room : roomData)
	{
		Room newRoom{ room.M_Id, room.description };
		for (int enemy_id : room.enemies)
		{
			for (const EnemyFile& enemyStats : enemyData)
			{
				Item* drop_item = enemyStats.M_DropId == 0 ? nullptr : items[enemyStats.M_DropId - 1];
				Enemy* enemy = new Enemy{ enemyStats.M_Name, enemyStats.M_Description, enemyStats.M_Combat.M_Health, enemyStats.M_Combat.M_Attack, enemyStats.M_Combat.M_Defense, drop_item };
				newRoom.AddEnemy(enemy);
			}
		}

		for (int item_id : room.items)
		{
			newRoom.AddItem(items[item_id - 1]);
		}
		ROOMS.push_back(newRoom);
	}

	for (const RoomFile& room : roomData)
	{
		Room* currentRoom = &ROOMS[room.M_Id - 1];

		for (ExitFile exit : room.exits)
		{
			Room* exitRoom = &ROOMS[exit.M_Id - 1];
			std::string direction;

			switch (exit.M_Dir) {
			case 'n': direction = "north"; break;
			case 'e': direction = "east"; break;
			case 's': direction = "south"; break;
			case 'w': direction = "west"; break;
			}

			currentRoom->AddExit(direction, *exitRoom);
		}
	}

	for (const ItemFile& item_load : itemData)
	{
		if (item_load.M_RoomId == 0) continue;

		Item* currentItem = items[item_load.M_Id - 1];
		currentItem->set_target_room(&ROOMS[item_load.M_RoomId - 1]);
	}
}

int main()
{
	Player player;
	Room* current_room = nullptr;
	try {
		std::vector<RoomFile> room_data;
		std::vector<ItemFile> item_data;
		std::vector<EnemyFile> enemy_data;

		load_rooms("Build\\rooms.txt", room_data);
		load_items("Build\\items.txt", item_data);
		load_enemies("Build\\enemies.txt", enemy_data);

		build_rooms(room_data, item_data, enemy_data);
	}
	catch (std::exception& e)
	{
		cout << e.what() << '\n';
		return 1;
	}

	current_room = &ROOMS[0];

	while (true)
	{
		print_separator();
		current_room->PrintDescription();
		Room* newRoom = current_room->RunCommands(player);

		if (current_room->M_ShouldQuit)
			break;

		if (current_room->M_HasWon)
			break;

		if (newRoom != nullptr)
		{
			system("cls");
			current_room = newRoom;
		}
	}

	if (current_room->M_HasWon)
		game_over("You won!");
	else
		game_over("Thanks for playing");

	return 0;
}