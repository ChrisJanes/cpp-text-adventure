#include "../headers/FileStructs.h"

#include <exception>

// checks if the istream is in a fail state
// if it is, clear to good and check for the terminator character
// if not found, throw exception
void end_of_loop(std::istream& ist, char term, const std::string& message)
{
    if (ist.fail()) 
    {
        ist.clear();
        char ch;
        if (ist >> ch && ch == term) return;
        throw std::exception(message.c_str());
    }
}

// read from one marker character to another and build a string with the contents
void build_string(std::istream &is, char marker, std::string &output)
{
    char ch;
    char start_des;
    is >> start_des;
    if (start_des == marker) 
    {
        while (is.get(ch) && ch != marker)
        {
            output += ch;
        }
    }
    else {
        is.unget();
        is.clear(std::ios_base::failbit);
    }
}

std::istream& operator>>(std::istream& is, ExitFile& e)
{
    // check upfront for a valid opening bracket
    char ch;
    is >> ch;
    if (ch != '{') 
    {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }

    // read the values
    std::string exit_marker;
    int room_id;
    char dir;
    char ch2;
    is >> exit_marker >> room_id >> dir >> ch2;
    
    // check for a valid final character
    if (!is || ch2 != '}') throw std::exception("bad exit file");

    // store the found values
    e.dir = dir;
    e.id = room_id;

    return is;
}

std::istream& operator>>(std::istream& is, ItemId& i)
{
    char ch;
    is >> ch;
    if (ch != '(') 
    {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }
    
    // these are just close enough to identical as a bunch of other operator calls as to be frustratingly not.
    std::string item_marker;
    int item_id;
    char ch2;
    is >> item_marker >> item_id >> ch2;

    if (!is || ch2 != ')') throw std::exception("bad item id");

    i.id = item_id;

    return is;
}

std::istream& operator>>(std::istream& is, EnemyId& e)
{
    char ch;
    is >> ch;
    if (ch != '<') 
    {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }

    std::string item_marker;
    int enemy_id;
    char ch2;
    is >> item_marker >> enemy_id >> ch2;

    if (!is || ch2 != '>') throw std::exception("bad item id");

    e.id = enemy_id;

    return is;
}

// format in file: {room id "description goes here" {exit direction id} {exit direction id} {item id} {enemy id}}
std::istream& operator>>(std::istream& is, RoomFile& r)
{
    char ch;
    is >> ch;
    if (ch != '{') 
    {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }

    std::string room_marker;
    std::string description;
    int id;
    is >> room_marker >> id;

    // get the description string
    build_string(is, '"', description);

    // read all the exits
    for (ExitFile re; is >> re;)
    {
        r.exits.push_back(re);
    }

    // we need to clear the istream flags here so the next bit works, implies there's an issue
    // with the >> operator overloads?
    is.clear();

    for (ItemId i; is >> i;)
    {
        r.items.push_back(i.id);
    }

    // and again.
    is.clear();

    for (EnemyId e; is >> e;)
    {
        r.enemies.push_back(e.id);
    }

    // if we're in the fail state, check if there's a closing brace or error.
    end_of_loop(is, '}', "bad end of room");

    // store the last couple of values
    r.description = description;
    r.id = id;

    return is;
}

// >> CombatStats seems to work, but is also broken
// so that's good.
std::istream& operator>>(std::istream& is, CombatStats& c)
{
    char ch;
    is >> ch;
    if (ch != '(') 
    {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }

    std::string item_marker;
    int attack;
    int defense;
    int health;
    char ch2;

    is >> item_marker >> attack >> defense >> health >> ch2;

    if (!is || ch2 != ')') throw std::exception("bad combat block");

    c.attack = attack;
    c.defense = defense;
    c.health = health;

    // this has a habit of returning the stream in a weird state.
    return is;
}

// format in file:
// {item id name "description" room_id "use text" use_id victory {combat att def hp}}
std::istream& operator>>(std::istream& is, ItemFile& i)
{
    char ch;
    is >> ch;
    if (ch != '{') 
    {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }

    std::string item_marker;
    int id;
    std::string name;
    std::string description;
    int room_id;
    std::string use_text;
    int use_id;
    bool victory;
    CombatStats combat;
    
    is >> item_marker >> id >> name;

    build_string(is, '"', description);
    
    is >> room_id;

    build_string(is, '"', use_text);

    is >> use_id >> victory;

    is >> combat;

    is.clear(std::ios_base::failbit);

    end_of_loop(is, '}', "bad end of item");

    i.id = id;
    i.description = description;
    i.name = name;
    i.room_id = room_id;
    i.use_text = use_text;
    i.use_id = use_id;
    i.victory = victory;
    i.combat = combat;
    if (combat.attack > 0 || combat.defense > 0 || combat.health > 0) i.isCombat = true;

    return is;
}


// {enemy id name "description" {combat health damage defense} drop_id}
std::istream& operator>>(std::istream& is, EnemyFile& e)
{
    char ch;
    is >> ch;
    if (ch != '{') 
    {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }

    std::string marker;
    int id;
    std::string name;
    std::string description;
    CombatStats combat;
    int drop_id;

    is >> marker >> id >> name;

    build_string(is, '"', description);

    is >> drop_id;

    is >> combat;

    is.clear(std::ios_base::failbit);

    end_of_loop(is, '}', "bad enemy");

    e.id = id;
    e.combat = combat;
    e.name = name;
    e.description = description;
    e.drop_id = drop_id;

    return is;
}