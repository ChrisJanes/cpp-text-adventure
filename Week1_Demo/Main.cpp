#include <vector>
#include <iostream>
#include <stdexcept>

#include "Room.h"
#include "RoomExit.h"
#include "Player.h"
#include "Item.h"

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

const vector<string> NorthDir{ "north", "n"};
const vector<string> SouthDir{ "south", "s" };
const vector<string> EastDir{ "east", "e" };
const vector<string> WestDir{ "west", "w" };
const vector<string> QuitCmd{ "quit", "q", "exit"};
const vector<string> TakeCmd{ "take", "t", "pickup", "pick-up" };
const vector<string> DropCmd{ "drop", "d", "throw"};
const vector<string> UseCmd{ "use", "u" };

void gameOver(string message)
{
	cout << "Game Over\n" << message << '\n';
}

void printSeparator()
{
	cout << "\n*********************************************************************\n";
}

bool checkCommand(string cmd, const vector<string> commands)
{
	for (const string dir : commands)
	{
		if (cmd == dir) 
			return true;
	}

	return false;
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

	// R5 goes to room 3 (east)
	roomFive.AddExit("east", roomThree);

	// R6 goes to room 3 (west)
	roomSix.AddExit("west", roomThree);

	Item trowel{ "trowel", "it's caked in mud and rust" };
	Item kettle{ "kettle", "it's half full of stagnant water" };

	roomFour.AddItem(&trowel); 
	roomThree.AddItem(&kettle);

	currentRoom = &roomOne;

	while (true)
	{
		printSeparator();
		currentRoom->PrintDescription();
		Room* newRoom = currentRoom->RunCommands(player);

		if (currentRoom->shouldQuit)
			break;

		if (newRoom != nullptr)
			currentRoom = newRoom;
	}


	gameOver("You won!");

	return 0;
}