#include "../headers/Room.h"
#include "../headers/RoomExit.h"

#include <iostream>

using std::cout;
using std::cin;

// helper function to check a cmd against a vector of valid commands
inline bool checkCommand(string cmd, const vector<string> commands)
{
	for (const string dir : commands)
	{
		if (cmd == dir)
			return true;
	}

	return false;
}

// just get input from the user - should probably pass in the output string here or
// do away with this completely.
void Room::get_command()
{
	cout << "What do you want to do?\n";
	cin >> commandString;
	//if (!cin) error("error handling user input.\n");
}

bool Room::print_item(Item* item, string name)
{
	// print the item stats
	if (item->get_item_name() == name)
	{
		if (dynamic_cast<CombatItem*>(item))
			cout << *(CombatItem*)item;
		else
			cout << *item;

		return true;
	}

	return false;
}

void Room::add_enemy(Enemy* en)
{
	if (enemy != nullptr) return;

	enemy = en;
}

void Room::add_exit(string dir, Room &room)
{
	RoomExit exit{ dir, room };
	exits.push_back(exit);
}

// big scary core function
// a series of calls to "checkCommand", should be refactored nicely probably
// a small but neat parser with grammar could be good here.
Room* Room::run_commands(Player& player)
{
	should_quit = false;
	commandString = "";
	get_command(); // reads into commandString
	bool handledCommand = false;

	if (commandString == "") return nullptr;

	// check against directions
	// todo: make this work with the const direction vectors
	for (RoomExit exit : exits) 
	{
		if (commandString == exit.direction)
		{
			return &exit.exitLocation;
		}
	}

	if (checkCommand(commandString, UseCmd))
	{
		string item_str;
		cout << "What do you want to use?\n";
		cin >> item_str;

		bool used = false;
		Item* used_item = nullptr;
		for (Item* item : player.get_inventory())
		{
			if (item == nullptr) continue;
			if (item->get_item_name() == item_str)
			{
				if (item->can_use(this))
				{
					used = true;
					used_item = item;
					break;
				}
				else {
					cout << "You cannot use that item here\n";
					return nullptr;
				}
			}
		}

		if (!used)
		{
			cout << "You are not holding that item.\n";
			return nullptr;
		}

		cout << used_item->get_use_text() << '\n';
		
		switch (used_item->get_use_result())
		{
		case UseResult::Consume:
		{
			Item* output = used_item->get_result_item();
			if (output != nullptr)
				add_item(output);

			inventory.erase(std::remove(inventory.begin(), inventory.end(), used_item), inventory.end());
			break;
		}
		case UseResult::Victory:
			has_won = true;
			break;
		}

		return nullptr;
	}

	if (checkCommand(commandString, CombatCmd))
	{
		if (enemy == nullptr) 
		{
			cout << "There is nothing to fight here\n";
			return nullptr;
		}

		FightResult res = handle_combat(player);

		switch (res)
		{
		case FightResult::Loss:
			cout << "You have taken a battering, you fall to the ground unconscious. Some time passes before you awake.\n";
			break;
		case FightResult::Victory:
			cout << "You have defeated your enemy, it falls to the ground unmoving.\n";
			if (enemy->get_drop_item() != nullptr)
			{
				add_item(enemy->get_drop_item());
			}

			enemy = nullptr;

			break;
		case FightResult::Flee:
			cout << "You ran from an unwinnable situation, maybe next time?\n";
			break;
		}

		return nullptr;
	}

	if (checkCommand(commandString, InvCmd))
	{
		string inv = player.get_items_string();
		if (inv.size() > 0) 
		{
			cout << "You are carrying these items: " << inv << '\n';
		}
		else {
			cout << "You are not carrying any items.\n";
		}
		return nullptr;
	}

	if (checkCommand(commandString, TakeCmd))
	{
		handledCommand = true;
		if (inventory.size() == 0)
			cout << "There is nothing here to take\n";
		else
		{
			string item_str;
			cout << "What do you want to take?\n";
			cin >> item_str;
			Item* roomItem = nullptr;
			int dropIndex = 0;

			if (item_str != "all")
			{
				for (Item* item : inventory)
				{
					if (item->get_item_name() == item_str)
					{
						roomItem = item;
						break;
					}
					++dropIndex;
				}

				if (roomItem == nullptr)
				{
					cout << "There is no such item here\n";
					return nullptr;
				}

				cout << "You have taken the " << roomItem->get_item_name() << '\n';
				player.take_item(roomItem);
				inventory.erase(inventory.begin() + dropIndex);
			}
			else {
				for (Item* item : inventory)
				{
					player.take_item(item);
				}

				cout << "You take everything in the room.\n";
				inventory.erase(inventory.begin(), inventory.end());
			}
		}
		
		return nullptr;
	}

	if (checkCommand(commandString, LookCmd))
	{
		string look_str;
		cout << "What do you want to look at?\n";
		cin >> look_str;

		if (look_str == room) 
		{
			print_description();
			return nullptr;
		}

		for (Item* item : player.get_inventory())
		{
			if (item == nullptr) continue;
			bool found = print_item(item, look_str);
			if (found) return nullptr;
		}

		for (Item* item : inventory)
		{
			if (item == nullptr) continue;
			bool found = print_item(item, look_str);
			if (found) return nullptr;
		}

		cout << "You cannot see that item anywhere near you.\n";
	}

	if (checkCommand(commandString, QuitCmd))
	{
		should_quit = true;
		return nullptr;
	}

	if (checkCommand(commandString, DropCmd))
	{
		handledCommand = true;

		string item;
		cout << "You are holding: " << player.get_items_string() << "\n";
		cout << "What do you want to drop?\n";
		cin >> item;

		Item* toDrop = player.drop_item(item);
		if (toDrop == nullptr)
		{
			cout << "You are not holding that item.\n";
			return nullptr;
		}
		else 
		{
			inventory.push_back(toDrop);
		}
	}

	if (handledCommand == false)
		cout << "Unknown command\n";
	
	return nullptr;
}

void Room::add_item(Item* item)
{
	inventory.push_back(item);
}

void Room::print_description() 
{
	cout << "You find yourself in a " << description << ", " << formatExits() << '\n';
	if (enemy != nullptr) 
	{
		cout << "An " << enemy->get_name() << " stands before you, it looks angry. Fight if you think you can win.\n";
	}
	if (inventory.size() > 1)
		cout << "The following items are present: \n";
	else if (inventory.size() == 1)
		cout << "The room holds a ";

	for (Item* item : inventory) 
	{
		cout << item->get_item_name() << '\n';
	}	
}

FightResult Room::handle_combat(Player& player)
{
	int health = player.get_health();
	int enemy_health = enemy->get_health();

	while (player.get_health() > 0) 
	{
		cout << "Your opponent stands opposite you, it has " << enemy->get_health() << " health\n";
		cout << "You have " << player.get_health() << " health\n";
		cout << "What do you want to do? (fight, block or flee): ";
		string cmd;
		cin >> cmd;
		if (!cin) break;
		if (cmd == fight) 
		{
			int p_damage = player.get_damage(true);
			int e_damage = enemy->get_damage();

			enemy->take_damage(p_damage);
			if (enemy->is_dead())
			{
				player.set_health(health);
				return FightResult::Victory;
			}

			player.take_damage(e_damage, false);
			continue;
		}

		if (cmd == block) 
		{
			int e_damage = enemy->get_damage();
			player.take_damage(e_damage, true);
			if (player.get_health() == 0) break;

			int p_damage = player.get_damage(false);
			enemy->take_damage(p_damage);
			if (!enemy->is_dead())
			{
				player.set_health(health);
				return FightResult::Victory;
			}
		}

		if (cmd == flee)
		{
			enemy->set_health(enemy_health);
			return FightResult::Flee;
		}
	}

	player.set_health(health);
	enemy->set_health(enemy_health);

	return FightResult::Loss;
}

string Room::formatExits() 
{
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