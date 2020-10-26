#pragma once
#include <string>

class Room;

class RoomExit
{
public:
	RoomExit(string dir, Room& exit) : direction(dir), exitLocation(exit) {}
	string direction;
	Room &exitLocation;
};