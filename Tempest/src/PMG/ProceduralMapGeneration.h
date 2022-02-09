




#ifndef PROCEDURAL_MAP_GENERATION_H
#define PROCEDURAL_MAP_GENERATION_H

#include <vector>
#include <unordered_set>

enum class CellType
{
	null = -1,
	floor = -2,
	door = -3,


	wall = 0,
	wall_1 = 1,
	wall_2 = 2,
	wall_3 = 3,
	wall_4 = 4,
	wall_5 = 5,
	wall_6 = 6,
	wall_7 = 7,
	wall_8 = 8,
	wall_9 = 9
};

enum class Direction
{
	Null = 0
	, North = 1
	, South = -1
	, East = 2
	, West = -2
};

struct AABB
{
	// For AABB Collision
	int m_minX = 0;
	int m_minY = 0;
	int m_maxX = 0;
	int m_maxY = 0;

	// Should only be updated once
	bool m_update = false;
};

struct Room
{
	// Required data for construction of the room
	unsigned int m_height = 0;
	unsigned int m_width = 0;

	// Required data for generating the room visually
	std::vector<CellType> m_cellType;
	std::unordered_set<Direction> m_doorDirs;

	// Hacky way - The available options of placing a door
	std::vector<Direction> m_openDoorDirs;

	// Test for overlapping areas
	AABB m_position;

	int m_index = 0;
};

struct UserData
{
	// User can define/control these data
	unsigned int m_roomAmount;
	unsigned int m_maximumRoomHeight;
	unsigned int m_maximumRoomWidth;
};

namespace HouseRules
{
	constexpr unsigned int m_minimumRoomWidth = 5;
	constexpr unsigned int m_minimumRoomHeight = 5;
	//constexpr unsigned int m_maxDoors = 4;
}

struct Building
{
	// Maximum room size within this building
	unsigned int m_maximumRoomWidth;
	unsigned int m_maximumRoomHeight;

	// Data Containers for constructing/mapping the building
	std::vector<Room> m_rooms;
	std::vector<CellType> visual_Map;

	Building(unsigned int roomAmount, unsigned int maxRoomHeight, unsigned int maxRoomWidth);

	// Helper Functions
	void ConstructRoom(Room& room);			// Constructs an empty room
	void ConstructDoor(Room& currRoom, Room& adjRoom);	// RNG door for the room


	//std::vector<Room> CustomSearchAlgorithm() const; // To find the room from top left to btm right.
	std::vector<Room> CustomSortAlgorithm() const; // To find the room from top left to btm right.

	void Print();

	// Procedural Map Generation
	void ProceduralBuildingGeneration();
};


void Test_PMG();

#endif // !PROCEDURAL_MAP_GENERATION_H
