#pragma once
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include "Item.h"
#include "Player.h"
#include "Enemy.h"

using std::string;

class RoomExit;

const vector<string> NORTH_DIR{ "north", "n" };
const vector<string> SOUTH_DIR{ "south", "s" };
const vector<string> EAST_DIR{ "east", "e" };
const vector<string> WEST_DIR{ "west", "w" };
const vector<string> QUIT_CMD{ "quit", "q", "exit" };
const vector<string> TAKE_CMD{ "take", "t", "pickup", "pick-up" };
const vector<string> DROP_CMD{ "drop", "d", "throw" };
const vector<string> USE_CMD{ "use", "u" };
const vector<string> INV_CMD{ "inventory", "i", "bag", "hands" };
const vector<string> COMBAT_CMD{ "fight", "hit", "f" };
const vector<string> LOOK_CMD{ "look", "l", "study" };

const string FIGHT = "fight";
const string BLOCK = "block";
const string FLEE = "flee";
const string ROOM = "room";


enum class FIGHT_RESULT
{
    VICTORY,
    LOSS,
    FLEE
};


class Room
{
public:
    Room(): m_Id(0)
    {
    }


    Room(const int i, string desc) : m_Id{ i }, m_Description{ std::move(desc)}
    {
    }


    void AddEnemy(Enemy* en);
    void AddExit(const string& dir, Room& room);
    void AddItem(Item* item);
    Room* RunCommands(Player& player);
    bool M_ShouldQuit{false};
    bool M_HasWon{false};
    void PrintDescription();

private:
    int m_Id;
    FIGHT_RESULT HandleCombat(Player& player) const;

    void GetCommand();
    static bool PrintItem(Item* item, const string& name);

    Enemy* m_Enemy{nullptr};

    string m_CommandString;
    string formatExits();
    string m_Description;
    std::vector<RoomExit> m_Exits;
    std::vector<Item*> m_Inventory;
    bool m_IsExplored{false};
};
