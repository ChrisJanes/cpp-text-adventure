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

void Room::GetCommand()
{
	cout << "What do you want to do?\n";
	cin >> m_CommandString;
	//if (!cin) error("error handling user input.\n");
}

bool Room::PrintItem(Item* item, const string& name)
{
	if (item->get_item_name() == name)
	{
		if (dynamic_cast<CombatItem*>(item))
			cout << *static_cast<CombatItem*>(item);
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
	m_CommandString = "";
	GetCommand();
	bool handledCommand = false;

	if (m_CommandString.empty()) return nullptr;

	for (RoomExit exit : m_Exits) {
		if (m_CommandString == exit.direction)
		{
			return &exit.exitLocation;
		}
	}

	if (checkCommand(m_CommandString, USE_CMD))
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
				AddItem(output);

			m_Inventory.erase(std::remove(m_Inventory.begin(), m_Inventory.end(), used_item), m_Inventory.end());
			break;
		}
		case UseResult::Victory:
			M_HasWon = true;
			break;
		}

		return nullptr;
	}

	if (checkCommand(m_CommandString, COMBAT_CMD))
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
			if (m_Enemy->get_drop_item() != nullptr)
			{
				AddItem(m_Enemy->get_drop_item());
			}

			m_Enemy = nullptr;

			break;
		case FIGHT_RESULT::FLEE:
			cout << "You ran from an unwinnable situation, maybe next time?\n";
			break;
		}

		return nullptr;
	}

	if (checkCommand(m_CommandString, INV_CMD))
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

	if (checkCommand(m_CommandString, TAKE_CMD))
	{
		handledCommand = true;
		if (m_Inventory.empty())
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

	if (checkCommand(m_CommandString, LOOK_CMD))
	{
		string look_str;
		cout << "What do you want to look at?\n";
		cin >> look_str;

		if (look_str == ROOM) {
			PrintDescription();
			return nullptr;
		}

		for (Item* item : player.get_inventory())
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

	if (checkCommand(m_CommandString, QUIT_CMD))
	{
		M_ShouldQuit = true;
		return nullptr;
	}

	if (checkCommand(m_CommandString, DROP_CMD))
	{
		handledCommand = true;

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
			m_Inventory.push_back(toDrop);
		}
	}

	if (handledCommand == false)
		cout << "Unknown command\n";
	
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
		cout << "An " << m_Enemy->get_name() << " stands before you, it looks angry. Fight if you think you can win.\n";
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
	const int enemy_health = m_Enemy->get_health();

	while (player.GetHealth() > 0) {
		cout << "Your opponent stands opposite you, it has " << m_Enemy->get_health() << " health\n";
		cout << "You have " << player.GetHealth() << " health\n";
		cout << "What do you want to do? (fight, block or flee): ";
		string cmd;
		cin >> cmd;
		if (!cin) break;
		if (cmd == FIGHT) {
			const int p_damage = player.GetDamage(true);
			const int e_damage = m_Enemy->get_damage();

			m_Enemy->take_damage(p_damage);
			if (m_Enemy->is_dead())
			{
				player.SetHealth(health);
				return FIGHT_RESULT::VICTORY;
			}

			player.TakeDamage(e_damage, false);
			continue;
		}

		if (cmd == BLOCK) {
			const int e_damage = m_Enemy->get_damage();
			player.TakeDamage(e_damage, true);
			if (player.GetHealth() == 0) break;

			const int p_damage = player.GetDamage(false);
			m_Enemy->take_damage(p_damage);
			if (!m_Enemy->is_dead())
			{
				player.SetHealth(health);
				return FIGHT_RESULT::VICTORY;
			}
		}

		if (cmd == FLEE)
		{
			m_Enemy->set_health(enemy_health);
			return FIGHT_RESULT::FLEE;
		}
	}

	player.SetHealth(health);
	m_Enemy->set_health(enemy_health);

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

			exit_str += m_Exits[i].direction;

			if (m_Exits.size() > 2 &&  i < m_Exits.size() - 2)
				exit_str += ", ";
		}
		return exit_str;
	}
	else {
		return "there is an exit to the " + m_Exits[0].direction;
	}
}