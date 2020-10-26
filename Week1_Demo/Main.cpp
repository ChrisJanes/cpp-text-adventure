#include <vector>
#include <iostream>
#include <stdexcept>

#include "Room.h"
#include "RoomExit.h"
#include "Player.h"
#include "Item.h"
#include "Enemy.h"

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

int main()
{
	Room* currentRoom = nullptr;
	Room roomOne("dark hallway");
	Room roomTwo("shabby, unkempt garden");
	Room roomThree("grotty kitchen, dishes are piled on the side");
	Room roomFour("rotten, tumbledown shed");
	Room roomFive("living room, the furniture is mildewed and damp");
	Room roomSix("larder, tins of food rust on shelves");

	Player player;

	CombatItem shield("shield", "made of wood and leather, it might be sturdy", 0, 0, 4);
	Enemy orc("Orc", "it's big and green, like the hulk", 5, 2, 1, dynamic_cast<Item*>(&shield));

	roomOne.AddExit("east", roomTwo);
	roomOne.AddExit("north", roomThree);

	roomTwo.AddExit("west", roomOne);
	roomTwo.AddExit("south", roomFour);

	// R3 goes to rooms 1 (south), 5 (east) and 6 (west)
	roomThree.AddExit("south", roomOne);
	roomThree.AddExit("east", roomFive);
	roomThree.AddExit("west", roomSix);

	// R4 goes to room 2 (north)
	roomFour.AddExit("north", roomTwo);

	// R5 goes to room 3 (west)
	roomFive.AddExit("west", roomThree);

	// R6 goes to room 3 (east)
	roomSix.AddExit("east", roomThree);
	roomSix.AddEnemy(&orc);

	Item key{ "key", "it looks like it will fit the front door", "The key twists in the lock smoothly, the door swings open.", nullptr };
	key.make_victory_item();
	key.set_target_room(&roomOne);
	Item trowel{ "trowel", "it's caked in mud and rust", "you dig through the mud and find a key", &key };
	trowel.set_target_room(&roomTwo);
	Item kettle{ "kettle", "it's half full of stagnant water" };

	CombatItem sword{ "sword", "it's shiny and sharp", 4, 0, 0 };

	roomFour.AddItem(&trowel); 
	roomThree.AddItem(&kettle);
	roomThree.AddItem(&sword);

	currentRoom = &roomOne;

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