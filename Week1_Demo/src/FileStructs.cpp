#include "../headers/FileStructs.h"

#include <exception>

void end_of_loop(std::istream& ist, char term, const std::string& message)
{
    if (ist.fail()) {
        ist.clear();
        char ch;
        if (ist >> ch && ch == term) return;
        throw std::exception(message.c_str());
    }
}

void build_string(std::istream &is, char term, std::string &output)
{
    char ch;
    char start_des;
    is >> start_des;
    if (start_des == term) {
        while (is.get(ch) && ch != term)
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
    char ch;
    is >> ch;
    if (ch != '{') {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }

    std::string exit_marker;
    int room_id;
    char dir;
    char ch2;
    is >> exit_marker >> room_id >> dir >> ch2;
    
    if (!is || ch2 != '}') throw std::exception("bad exit file");

    e.M_Dir = dir;
    e.M_Id = room_id;

    return is;
}

std::istream& operator>>(std::istream& is, ItemId& i)
{
    char ch;
    is >> ch;
    if (ch != '(') {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }

    std::string item_marker;
    int item_id;
    char ch2;
    is >> item_marker >> item_id >> ch2;

    if (!is || ch2 != ')') throw std::exception("bad item id");

    i.M_Id = item_id;

    return is;
}

std::istream& operator>>(std::istream& is, EnemyId& e)
{
    char ch;
    is >> ch;
    if (ch != '<') {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }

    std::string item_marker;
    int enemy_id;
    char ch2;
    is >> item_marker >> enemy_id >> ch2;

    if (!is || ch2 != '>') throw std::exception("bad item id");

    e.M_Id = enemy_id;

    return is;
}

std::istream& operator>>(std::istream& is, RoomFile& r)
{
    char ch;
    is >> ch;
    if (ch != '{') {
        is.unget();
        is.clear(std::ios_base::failbit);
        return is;
    }

    std::string room_marker;
    std::string description;
    int id;
    is >> room_marker >> id;

    build_string(is, '"', description);

    for (ExitFile re; is >> re;)
    {
        r.exits.push_back(re);
    }

    is.clear();

    for (ItemId i; is >> i;)
    {
        r.items.push_back(i.M_Id);
    }

    is.clear();

    for (EnemyId e; is >> e;)
    {
        r.enemies.push_back(e.M_Id);
    }

    end_of_loop(is, '}', "bad end of room");

    r.description = description;
    r.M_Id = id;

    return is;
}

std::istream& operator>>(std::istream& is, CombatStats& c)
{
    char ch;
    is >> ch;
    if (ch != '(') {
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

    c.M_Attack = attack;
    c.M_Defense = defense;
    c.M_Health = health;

    return is;
}

// {item id name "description" room_id "use text" use_id victory {combat att def hp}}

std::istream& operator>>(std::istream& is, ItemFile& i)
{
    char ch;
    is >> ch;
    if (ch != '{') {
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

    i.M_Id = id;
    i.M_Description = description;
    i.M_Name = name;
    i.M_RoomId = room_id;
    i.M_UseText = use_text;
    i.M_UseId = use_id;
    i.M_Victory = victory;
    i.M_Combat = combat;
    if (combat.M_Attack > 0 || combat.M_Defense > 0 || combat.M_Health > 0) i.M_IsCombat = true;

    return is;
}


// {enemy id name "description" {combat health damage defense} drop_id}
std::istream& operator>>(std::istream& is, EnemyFile& e)
{
    char ch;
    is >> ch;
    if (ch != '{') {
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

    e.M_Id = id;
    e.M_Combat = combat;
    e.M_Name = name;
    e.M_Description = description;
    e.M_DropId = drop_id;

    return is;
}