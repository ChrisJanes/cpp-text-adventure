#include "../headers/Room.h"
#include "../headers/RoomExit.h"

#include <iostream>

using std::cout;
using std::cin;

inline bool checkCommand(const string cmd, const vector<string>& commands)
{
	for (const string& dir : commands)
	{
		if (cmd == dir)
			return true;
	}

	return false;
}

string Room::GetCommandFromPlayer()
{
	cout << "What do you want to do?\n";
	string commandString;
	cin >> commandString;
	//if (!cin) error("error handling user input.\n");
	return commandString;
}


string Room::GetCommandParameter()
{
	string commandPeramimterString;
	cin >> commandPeramimterString;
	return commandPeramimterString;
}


bool Room::PrintItem(Item* item, const string& name)
{
	if (item->get_item_name() == name)
	{
		if (dynamic_cast<CombatItem*>(item))
			cout << *dynamic_cast<CombatItem*>(item);
		else
			cout << *item;

		return true;
	}

	return false;
}

void Room::AddEnemy(Enemy* en)
{
	if (m_Enemy != nullptr) return;

	m_Enemy = en;
}

void Room::AddExit(const string& dir, Room &room)
{
	const RoomExit exit{ dir, room };
	m_Exits.push_back(exit);
}

Room* Room::RunCommands(Player& player)
{
	M_ShouldQuit = false;
    const string command_string = GetCommandFromPlayer();
	bool handledCommand = false;

	if (command_string.empty()) return nullptr;

	for (RoomExit exit : m_Exits) {
		if (command_string == exit.M_Direction)
		{
			return &exit.M_ExitLocation;
		}
	}

	if (checkCommand(command_string, USE_CMD))
	{
		
		cout << "What do you want to use?\n";
		const string item_str = GetCommandParameter();

		bool used = false;
		Item* used_item = nullptr;
		for (Item* item : player.GetInventory())
		{
			if (item == nullptr) continue;
			if (item->get_item_name() == item_str)
			{
				if (item->CanUse(this))
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
		
		switch (used_item->GetUseResult())
		{
		case USE_RESULT::CONSUME:
		{
			Item* output = used_item->GetResultItem();
			if (output != nullptr)
				AddItem(output);

			m_Inventory.erase(std::remove(m_Inventory.begin(), m_Inventory.end(), used_item), m_Inventory.end());
			break;
		}
		case USE_RESULT::VICTORY:
			M_HasWon = true;
			break;
		}

		return nullptr;
	}

	if (checkCommand(command_string, COMBAT_CMD))
	{
		if (m_Enemy == nullptr) {
			cout << "There is nothing to fight here\n";
			return nullptr;
		}

        const FIGHT_RESULT res = HandleCombat(player);

		switch (res)
		{
		case FIGHT_RESULT::LOSS:
			cout << "You have taken a battering, you fall to the ground unconscious. Some time passes before you awake.\n";
			break;
		case FIGHT_RESULT::VICTORY:
			cout << "You have defeated your enemy, it falls to the ground unmoving.\n";
			if (m_Enemy->GetDropItem() != nullptr)
			{
				AddItem(m_Enemy->GetDropItem());
			}

			m_Enemy = nullptr;

			break;
		case FIGHT_RESULT::FLEE:
			cout << "You ran from an unwinnable situation, maybe next time?\n";
			break;
		}

		return nullptr;
	}

	if (checkCommand(command_string, INV_CMD))
	{
		const string inv = player.GetItems();
		if (!inv.empty()) {
			cout << "You are carrying these items: " << inv << '\n';
		}
		else {
			cout << "You are not carrying any items.\n";
		}
		return nullptr;
	}

	if (checkCommand(command_string, TAKE_CMD))
	{
		handledCommand = true;
		if (m_Inventory.empty())
			cout << "There is nothing here to take\n";
		else
		{
			
			cout << "What do you want to take?\n";
			const string item_str = GetCommandParameter();
		    //cin >> item_str;
			Item* roomItem = nullptr;
			int dropIndex = 0;

			if (item_str != "all")
			{
				for (Item* item : m_Inventory)
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
				player.TakeItem(roomItem);
				m_Inventory.erase(m_Inventory.begin() + dropIndex);
			}
			else {
				for (Item* item : m_Inventory)
				{
					player.TakeItem(item);
				}

				cout << "You take everything in the room.\n";
				m_Inventory.erase(m_Inventory.begin(), m_Inventory.end());
			}
		}
		
		return nullptr;
	}

	if (checkCommand(command_string, LOOK_CMD))
	{
		
		cout << "What do you want to look at?\n";
		const string look_str = GetCommandParameter();
		//cin >> look_str;

		if (look_str == ROOM) {
			PrintDescription();
			return nullptr;
		}

		for (Item* item : player.GetInventory())
		{
			if (item == nullptr) continue;
			bool found = PrintItem(item, look_str);
			if (found) return nullptr;
		}

		for (Item* item : m_Inventory)
		{
			if (item == nullptr) continue;
			bool found = PrintItem(item, look_str);
			if (found) return nullptr;
		}

		cout << "You cannot see that item anywhere near you.\n";
	}

	if (checkCommand(command_string, QUIT_CMD))
	{
		M_ShouldQuit = true;
		return nullptr;
	}

	if (checkCommand(command_string, DROP_CMD))
	{
		handledCommand = true;

		//string item;
		cout << "You are holding: " << player.GetItems() << "\n";
		cout << "What do you want to drop?\n";
		const string item = GetCommandParameter();
		//cin >> item;

		Item* toDrop = player.DropItem(item);
		if (toDrop == nullptr)
		{
			cout << "You are not holding that item.\n";
			return nullptr;
		}
		else 
		{
			m_Inventory.push_back(toDrop);
		}
	}

	if (handledCommand == false)
	{
		cout << "Unknown command\n";
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	
	return nullptr;
}

void Room::AddItem(Item* item) 
{
	m_Inventory.push_back(item);
}

void Room::PrintDescription() {
	cout << "You find yourself in a " << m_Description << ", " << formatExits() << '\n';
	if (m_Enemy != nullptr) 
	{
		cout << "An " << m_Enemy->GetName() << " stands before you, it looks angry. Fight if you think you can win.\n";
	}
	if (m_Inventory.size() > 1)
		cout << "The following items are present: \n";
	else if (m_Inventory.size() == 1)
		cout << "The room holds a ";

	for (Item* item : m_Inventory) {
		cout << item->get_item_name() << '\n';
	}	
}

FIGHT_RESULT Room::HandleCombat(Player& player) const
{
	const int health = player.GetHealth();
	const int enemy_health = m_Enemy->GetHealth();

	while (player.GetHealth() > 0) {
		cout << "Your opponent stands opposite you, it has " << m_Enemy->GetHealth() << " health\n";
		cout << "You have " << player.GetHealth() << " health\n";
		cout << "What do you want to do? (fight, block or flee): ";
		string cmd;
		cin >> cmd;
		if (!cin) break;
		if (cmd == FIGHT) {
			const int p_damage = player.GetDamage(true);
			const int e_damage = m_Enemy->GetDamage();

			m_Enemy->TakeDamage(p_damage);
			if (m_Enemy->IsDead())
			{
				player.SetHealth(health);
				return FIGHT_RESULT::VICTORY;
			}

			player.TakeDamage(e_damage, false);
			continue;
		}

		if (cmd == BLOCK) {
			const int e_damage = m_Enemy->GetDamage();
			player.TakeDamage(e_damage, true);
			if (player.GetHealth() == 0) break;

			const int p_damage = player.GetDamage(false);
			m_Enemy->TakeDamage(p_damage);
			if (!m_Enemy->IsDead())
			{
				player.SetHealth(health);
				return FIGHT_RESULT::VICTORY;
			}
		}

		if (cmd == FLEE)
		{
			m_Enemy->SetHealth(enemy_health);
			return FIGHT_RESULT::FLEE;
		}
	}

	player.SetHealth(health);
	m_Enemy->SetHealth(enemy_health);

	return FIGHT_RESULT::LOSS;
}

string Room::formatExits() {
	if (m_Exits.size() > 1)
	{
		string exit_str = "there are exits to the ";
		for (int i = 0; i < m_Exits.size(); ++i)
		{
			if (i == m_Exits.size() - 1)
				exit_str += " and ";

			exit_str += m_Exits[i].M_Direction;

			if (m_Exits.size() > 2 &&  i < m_Exits.size() - 2)
				exit_str += ", ";
		}
		return exit_str;
	}
	else {
		return "there is an exit to the " + m_Exits[0].M_Direction;
	}
}