#pragma once
#include <string>

class Room;

class RoomExit
{
public:
	RoomExit(string dir,Room& exit) : M_Direction(std::move( dir)), M_ExitLocation(exit) {}
	string M_Direction;
	Room &M_ExitLocation;
};