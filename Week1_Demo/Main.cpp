#include "std_lib_facilities.h"

/*
5--3--6
   |
   1--2
 	  |
	  4
*/

// forward declare all room printing functions
void roomOne();
void roomTwo();
void roomThree();
void roomFour();
void roomFive();
void roomSix();

string playerHands = "";
string r1Item = "";
string r2Item = "";
string r3Item = "";
string r4Item = "trowel";
string r5Item = "";
string r6Item = "";

bool r1Explored = false;
bool r2Explored = false;
bool r3Explored = false;
bool r4Explored = false;
bool r5Explored = false;
bool r6Explored = false;

int playerXp = 0;
int playerLevel = 0;

const vector<string> NorthDir{ "north", "n"};
const vector<string> SouthDir{ "south", "s" };
const vector<string> EastDir{ "east", "e" };
const vector<string> WestDir{ "west", "w" };
const vector<string> QuitCmd{ "quit", "q", "exit"};
const vector<string> TakeCmd{ "take", "t", "pickup", "pick-up" };
const vector<string> DropCmd{ "drop", "d", "throw"};
const vector<string> UseCmd{ "use", "u" };

void gameOver() {
	cout << "game over, you won.\n";
}

void printSeparator()
{
	cout << "\n*********************************************************************\n";
}

string getCommand()
{
	cout << "What do you want to do?\n";
	string command;
	cin >> command;
	if (!cin) error("error handling user input.\n");
	return command;
}

void doLevelUp()
{
	int currentLevel = playerLevel;
	++playerXp;
	if (playerXp == 1)
		playerLevel = 1;
	else if (playerXp == 3)
		playerLevel = 2;
	else if (playerXp == 6)
		playerLevel = 3;

	if (currentLevel != playerLevel)
	{
		printSeparator();
		cout << "\nYou have reached level " << playerLevel << ", revel in your growing power!\n";
	}
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

// room one goes to rooms 3 (north) and 2 (east)
void roomOne()
{
	printSeparator();
	cout << "You find yourself in a dark hallway, there are exits to the north and east.\n";
	if (r1Item != "")
	{
		cout << "There is a " << r1Item << " here.\n";
	}

	bool commandHandled = false;
	while (!commandHandled)
	{
		if (r1Explored == false)
		{
			doLevelUp();
			r1Explored = true;
		}	

		string command = getCommand();

		// now handle commands
		if (checkCommand(command, NorthDir))
		{
			cout << "You leave the room to the north.\n";
			commandHandled = true;
			roomThree();
		}
		else if (checkCommand(command, EastDir))
		{
			cout << "You leave the room to the east\n";
			commandHandled = true;
			roomTwo();
		}
		else if (checkCommand(command, QuitCmd))
		{
			return;
		}
		else
		{
			cout << "Unknown command (" << command << "), case is important\n";
		}
	}
}

// R2 goes to rooms 1 (west) and 4 (south)
void roomTwo()
{
	if (r2Explored == false)
	{
		doLevelUp();
		r2Explored = true;
	}
	printSeparator();
	cout << "You find yourself in a shabby, unkempt garden, there are exits to the south and west.\n";

	if (r2Item != "")
	{
		cout << "There is a " << r2Item << " here.\n";
	}

	bool commandHandled = false;
	while (!commandHandled)
	{
		string command = getCommand();

		if (checkCommand(command, SouthDir))
		{
			cout << "You leave the room to the south.\n";
			roomFour();
		}
		else if (checkCommand(command, WestDir))
		{
			cout << "You leave the room to the west\n";
			roomOne();
		}
		else if (checkCommand(command, DropCmd))
		{
			if (r2Item == "")
			{
				r2Item = playerHands;
				playerHands = "";
			}
			else
			{
				cout << "There is no space for a new item here.\n";
			}
		}
		else if (checkCommand(command, UseCmd)) {
			if (playerHands == "trowel")
			{
				cout << "You dig a hole with the trowel and find a mug.\n";
				playerHands = "mug";
				r2Item = "trowel";
			}
		}
		else if (checkCommand(command, QuitCmd))
		{
			return;
		}
		else
		{
			cout << "Unknown command (" << command << "), case is important\n";
		}
	}
}

// R3 goes to rooms 1 (south), 5 (east) and 6 (west)
void roomThree()
{
	if (r3Explored == false)
	{
		doLevelUp();
		r3Explored = true;
	}
	printSeparator();
	cout << "You find yourself in a grotty kitchen, dishes are piled on the side, there are exits to the east, south and west.\n";
	if (r3Item != "")
	{
		cout << "There is a " << r3Item << " here.\n";
	}

	bool commandHandled = false;
	while (!commandHandled)
	{
		string command = getCommand();

		if (checkCommand(command, SouthDir))
		{
			cout << "You leave the room to the south.\n";
			commandHandled = true;
			roomOne();
		}
		else if (checkCommand(command, EastDir))
		{
			cout << "You leave the room to the east\n";
			commandHandled = true;
			roomFive();
		}
		else if (checkCommand(command, WestDir))
		{
			cout << "You leave the room to the west\n";
			commandHandled = true;
			roomSix();
		}
		else if (checkCommand(command, QuitCmd))
		{
			return;
		}
		else
		{
			cout << "Unknown command (" << command << "), case is important\n";
		}
	}
}

// R4 goes to room 2 (north)
void roomFour()
{
	if (r4Explored == false)
	{
		doLevelUp();
		r4Explored = true;
	}
	printSeparator();
	cout << "You are in a rotten, tumbledown shed, there is an exit to the north.\n";
	if (r4Item != "")
	{
		cout << "There is a " << r4Item << " here.\n";
	}
	bool commandHandled = false;
	while (!commandHandled)
	{
		string command = getCommand();

		if (checkCommand(command, NorthDir))
		{
			cout << "You leave the room to the north.\n";
			commandHandled = true;
			roomTwo();
		}
		else if (checkCommand(command, QuitCmd))
		{
			return;
		}
		else if (checkCommand(command, TakeCmd))
		{
			playerHands = r4Item;
			r4Item = "";
		}
		else if (checkCommand(command, DropCmd))
		{
			if (r4Item == "")
			{
				r4Item = playerHands;
				playerHands = "";
			}
			else
			{
				cout << "There is no space for a new item here.\n";
			}
		}
		else if (checkCommand(command, UseCmd)) {
			cout << "There is nothing to use that with here.\n";
		}
		else
		{
			cout << "Unknown command (" << command << "), case is important\n";
		}
	}
}

// R5 goes to room 3 (east)
void roomFive()
{
	if (r5Explored == false)
	{
		doLevelUp();
		r5Explored = true;
	}
	printSeparator();
	cout << "You find yourself in a living room, the furniture is mildewed and damp, there is an exit to the west.\n";
	if (r5Item != "")
	{
		cout << "There is a " << r5Item << " here.\n";
	}
	bool commandHandled = false;
	while (!commandHandled)
	{
		string command = getCommand();

		if (checkCommand(command, WestDir))
		{
			cout << "You leave the room to the west\n";
			commandHandled = true;
			roomThree();
		}
		else if (checkCommand(command, QuitCmd))
		{
			return;
		}
		else
		{
			cout << "Unknown command (" << command << "), case is important\n";
		}
	}
}

// R6 goes to room 3 (west)
void roomSix()
{
	if (r6Explored == false)
	{
		doLevelUp();
		r6Explored = true;
	}
	printSeparator();
	cout << "You find yourself in a larder, tins of food rust on shelves, there is an exit to the east.\n";
	if (r6Item != "")
	{
		cout << "There is a " << r6Item << " here.\n";
	}
	bool commandHandled = false;
	while (!commandHandled)
	{
		string command = getCommand();

		if (checkCommand(command, EastDir))
		{
			cout << "You leave the room to the east\n";
			commandHandled = true;
			roomThree();
		}
		else if (checkCommand(command, QuitCmd))
		{
			return;
		}
		else
		{
			cout << "Unknown command (" << command << "), case is important\n";
		}
	}
}

void printPlayerWelcome(string playerName)
{
	cout << "Welcome " << playerName << " to this dungeon thing.\n";
	cout << "This text adventure game will truly test your wits to the limit.\n\nOr will it?\n";
}

int main()
{
	string playerName;
	cout << "Adventurer, please enter your name: ";
	cin >> playerName;

	printPlayerWelcome(playerName);
	roomOne();

	return 0;
}