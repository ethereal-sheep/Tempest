


#include "ProceduralMapGeneration.h"

// Function
#include <functional>

// std::sort
#include <algorithm>

// Console Print Out
#include <iostream>

void Test_PMG()
{
	// Seed the RNG
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	for (unsigned int i = 0; i < 10; ++i)
	{
		Building tempBuilding_3(10, 15, 5);
		//tempBuilding_3.ConstructRoom();
		//tempBuilding_3.ConstructDoor();
		tempBuilding_3.ProceduralBuildingGeneration();
		//tempBuilding_3.Print();
	}
	std::cout << "Hello World!" << std::endl;
}


Building::Building(unsigned int roomAmount, unsigned int maxRoomHeight, unsigned int maxRoomWidth)
	: m_maximumRoomHeight{ maxRoomHeight }
	, m_maximumRoomWidth{ maxRoomWidth }
{
	// Generate the room sizes
	for (size_t i = 0; i < roomAmount; ++i)
	{
		// RNG room size - minimum size 5
		Room newRoom;

		newRoom.m_index = static_cast<int>(i);

		newRoom.m_openDoorDirs.push_back(Direction::North);
		newRoom.m_openDoorDirs.push_back(Direction::South);
		newRoom.m_openDoorDirs.push_back(Direction::East);
		newRoom.m_openDoorDirs.push_back(Direction::West);

		newRoom.m_width = std::rand() % m_maximumRoomWidth;
		newRoom.m_height = std::rand() % m_maximumRoomHeight;
		//newRoom.m_width = 0;
		//newRoom.m_height = 0;

		// Ensure minimum size
		if (newRoom.m_height < HouseRules::m_minimumRoomHeight)
			newRoom.m_height = HouseRules::m_minimumRoomHeight;

		if (newRoom.m_width < HouseRules::m_minimumRoomWidth)
			newRoom.m_width = HouseRules::m_minimumRoomWidth;

		// Prepare the cells to be filled
		newRoom.m_cellType.resize(static_cast<size_t>(newRoom.m_height) * newRoom.m_width);

		// Store the newly created room
		m_rooms.push_back(newRoom);
	}
}

void Building::ConstructRoom(Room& room)
{
	// Index version for the vector
	size_t roomMaxHeight = room.m_height;
	size_t roomMaxWidth = room.m_width;

	// Constructs a hollow room
	for (size_t h = 0; h < roomMaxHeight; ++h)
	{
		for (size_t w = 0; w < roomMaxWidth; ++w)
		{
			// Sides of the room
			if (h == 0 || w == 0 || h == roomMaxHeight - 1 || w == roomMaxWidth - 1)
				//room.m_cellType[h * roomMaxWidth + w] = CellType::wall;
				room.m_cellType[h * roomMaxWidth + w] = static_cast<CellType>(room.m_index);
			else
				room.m_cellType[h * roomMaxWidth + w] = CellType::floor;
		}
	}
}

void Building::ConstructDoor(Room& currRoom, Room& adjRoom)
{
	std::function<void(Room&, Room&, bool)> doorDirection = [](Room& startRoom, Room& endRoom, bool swap) {

		Direction doorDir = Direction::Null;
		Direction adj_doorDir = Direction::Null;

		bool edgeSpawned = false;

		do
		{
			doorDir = startRoom.m_openDoorDirs[std::rand() % startRoom.m_openDoorDirs.size()];
			adj_doorDir = static_cast<Direction>(-static_cast<int>(doorDir));

			// Check if the adj Direction is available
			if (endRoom.m_doorDirs.find(adj_doorDir) == endRoom.m_doorDirs.end())
				edgeSpawned = true;

		} while (edgeSpawned == false);

		// Once it reaches here, the edge is considered eligible to be spawned
		//std::cout << "Insert Successfully" << std::endl;

		// Insert the door direction
		startRoom.m_doorDirs.insert(doorDir);
		endRoom.m_doorDirs.insert(adj_doorDir);

		// Remove the option after selection
		for (auto itr = startRoom.m_openDoorDirs.begin(); itr != startRoom.m_openDoorDirs.end(); ++itr)
		{
			if (*itr == doorDir)
			{
				startRoom.m_openDoorDirs.erase(itr);
				break;
			}
		}

		for (auto itr = endRoom.m_openDoorDirs.begin(); itr != endRoom.m_openDoorDirs.end(); ++itr)
		{
			if (*itr == adj_doorDir)
			{
				endRoom.m_openDoorDirs.erase(itr);
				break;
			}
		}

		// Index version of Height and width for the vector
		unsigned int roomMaxHeight = startRoom.m_height;
		unsigned int roomMaxWidth = startRoom.m_width;

		unsigned int adj_roomMaxHeight = endRoom.m_height;
		unsigned int adj_roomMaxWidth = endRoom.m_width;

		// Door position
		unsigned int spawnDoorPos = 0;
		unsigned int adjSpawnDoorPos = 0;

		switch (doorDir)
		{
		case Direction::North:

			spawnDoorPos = (std::rand() % (roomMaxWidth - 2)) + 1;
			adjSpawnDoorPos = (adj_roomMaxHeight - 1) * adj_roomMaxWidth + (std::rand() % (adj_roomMaxWidth - 2)) + 1;
			break;

		case Direction::South:

			spawnDoorPos = (roomMaxHeight - 1) * roomMaxWidth + (std::rand() % (roomMaxWidth - 2)) + 1;
			adjSpawnDoorPos = (std::rand() % (adj_roomMaxWidth - 2)) + 1;
			break;

		case Direction::West:

			spawnDoorPos = (std::rand() % (roomMaxHeight - 2) + 1) * roomMaxWidth;
			adjSpawnDoorPos = (std::rand() % (adj_roomMaxHeight - 2) + 1) * adj_roomMaxWidth + adj_roomMaxWidth - 1;
			break;

		case Direction::East:

			//std::cout << "East" << std::endl;
			spawnDoorPos = (std::rand() % (roomMaxHeight - 2) + 1) * roomMaxWidth + roomMaxWidth - 1;
			adjSpawnDoorPos = (std::rand() % (adj_roomMaxHeight - 2) + 1) * adj_roomMaxWidth;
			break;

		default:
			break;
		}

		// Inserts the door into the room
		startRoom.m_cellType[spawnDoorPos] = CellType::door;
		endRoom.m_cellType[adjSpawnDoorPos] = CellType::door;

		/*std::cout << "curRoomIndex: " << startRoom.m_index << std::endl;
		std::cout << "adjRoomIndex: " << endRoom.m_index << std::endl;
		std::cout << "curSpawnDoorPos: " << spawnDoorPos << std::endl;
		std::cout << "adjSpawnDoorPos: " << adjSpawnDoorPos << std::endl;*/

		// Update its AABB Position

		// This should happen once and only for the first room
		if (swap)
		{
			std::cout << "Swap" << std::endl;
			//// Update currRoom
			//if (endRoom.m_position.m_update == false)
			//{
			//	endRoom.m_position.m_minX = 0;
			//	endRoom.m_position.m_minY = 0;

			//	endRoom.m_position.m_maxX = endRoom.m_width;
			//	endRoom.m_position.m_maxY = endRoom.m_height;

			//	endRoom.m_position.m_update = true;
			//}

			//// Update adjacent room
			//if (startRoom.m_position.m_update == false)
			//{
			//	switch (doorDir)
			//	{
			//	case Direction::North:


			//		break;

			//	case Direction::South:
			//		
			//		break;

			//	case Direction::East:
			//		
			//		break;

			//	case Direction::West:
			//		
			//		break;

			//	default:
			//		break;
			//	}
			//}
		}
		else
		{
			// The first room in the map to be generated is the center point

			// Update currRoom
			if (startRoom.m_position.m_update == false)
			{
				startRoom.m_position.m_minX = 0;
				startRoom.m_position.m_maxY = 0;

				startRoom.m_position.m_maxX = startRoom.m_position.m_minX + startRoom.m_width;
				startRoom.m_position.m_minY = startRoom.m_position.m_maxY - startRoom.m_height;

				startRoom.m_position.m_update = true;
			}

			// For position update purpose
			int adj_MinX_Distance = (adjSpawnDoorPos % endRoom.m_width);
			int curr_MinX_Distance = (spawnDoorPos % startRoom.m_width);

			int adj_Max_Y_Distance = (adjSpawnDoorPos / endRoom.m_width);
			int curr_Max_Y_Distance = (spawnDoorPos / startRoom.m_width);

			//std::cout << "spawnDoorPos: " << spawnDoorPos << std::endl;
			//std::cout << "adjSpawnDoorPos: " << adjSpawnDoorPos << std::endl;
			//std::cout << "adj_Max_Y_Distance: " << adj_Max_Y_Distance << std::endl;
			//std::cout << "curr_Max_Y_Distance: " << curr_Max_Y_Distance << std::endl;
			//std::cout << std::endl;

			// Update adjacent room position
			if (endRoom.m_position.m_update == false)
			{
				switch (adj_doorDir)
				{
					// Bottom of the currznt room
				case Direction::North:

					endRoom.m_position.m_maxY = startRoom.m_position.m_minY /*- 1*/;
					endRoom.m_position.m_minY = endRoom.m_position.m_maxY - endRoom.m_height;

					if (curr_MinX_Distance < adj_MinX_Distance)
						endRoom.m_position.m_minX = startRoom.m_position.m_minX - std::abs(curr_MinX_Distance - adj_MinX_Distance);
					else if (curr_MinX_Distance > adj_MinX_Distance)
						endRoom.m_position.m_minX = startRoom.m_position.m_minX + std::abs(curr_MinX_Distance - adj_MinX_Distance);
					else
						endRoom.m_position.m_minX = startRoom.m_position.m_minX;

					endRoom.m_position.m_maxX = endRoom.m_position.m_minX + endRoom.m_width;

					break;
					// On top of the current room
				case Direction::South:

					endRoom.m_position.m_minY = startRoom.m_position.m_maxY /*+ 1*/;
					endRoom.m_position.m_maxY = endRoom.m_position.m_minY + endRoom.m_height;

					if (curr_MinX_Distance < adj_MinX_Distance)
						endRoom.m_position.m_minX = startRoom.m_position.m_minX - std::abs(curr_MinX_Distance - adj_MinX_Distance);
					else if (curr_MinX_Distance > adj_MinX_Distance)
						endRoom.m_position.m_minX = startRoom.m_position.m_minX + std::abs(curr_MinX_Distance - adj_MinX_Distance);
					else
						endRoom.m_position.m_minX = startRoom.m_position.m_minX;

					endRoom.m_position.m_maxX = endRoom.m_position.m_minX + endRoom.m_width;

					break;
					// On the right side of the room
				case Direction::West:

					endRoom.m_position.m_minX = startRoom.m_position.m_maxX /*+ 1*/;
					endRoom.m_position.m_maxX = endRoom.m_position.m_minX + endRoom.m_width;

					if (curr_Max_Y_Distance < adj_Max_Y_Distance)
						endRoom.m_position.m_maxY = startRoom.m_position.m_maxY + std::abs(curr_Max_Y_Distance - adj_Max_Y_Distance);
					else if (curr_Max_Y_Distance > adj_Max_Y_Distance)
						endRoom.m_position.m_maxY = startRoom.m_position.m_maxY - std::abs(curr_Max_Y_Distance - adj_Max_Y_Distance);
					else
						endRoom.m_position.m_maxY = startRoom.m_position.m_maxY;

					endRoom.m_position.m_minY = endRoom.m_position.m_maxY - endRoom.m_height;

					break;

					// On the left side of the room
				case Direction::East:

					endRoom.m_position.m_maxX = startRoom.m_position.m_minX /*- 1*/;
					endRoom.m_position.m_minX = endRoom.m_position.m_maxX - endRoom.m_width;

					if (curr_Max_Y_Distance < adj_Max_Y_Distance)
						endRoom.m_position.m_maxY = startRoom.m_position.m_maxY + std::abs(curr_Max_Y_Distance - adj_Max_Y_Distance);
					else if (curr_Max_Y_Distance > adj_Max_Y_Distance)
						endRoom.m_position.m_maxY = startRoom.m_position.m_maxY - std::abs(curr_Max_Y_Distance - adj_Max_Y_Distance);
					else
						endRoom.m_position.m_maxY = startRoom.m_position.m_maxY;

					endRoom.m_position.m_minY = endRoom.m_position.m_maxY - endRoom.m_height;

					break;

				default:
					break;
				}

				endRoom.m_position.m_update = true;
			}
		}
	}; // End if lambda function

	// Use the room with the smaller pool of directions to select from
	if (currRoom.m_openDoorDirs.size() <= adjRoom.m_openDoorDirs.size())
		doorDirection(currRoom, adjRoom, false);
	else
		doorDirection(adjRoom, currRoom, true);
}

std::vector<Room> Building::CustomSortAlgorithm() const
{
	std::vector<Room> copied_Rooms = m_rooms;

	auto sortRoom = [](Room room_A, Room room_B) {

		if (room_A.m_position.m_maxY == room_B.m_position.m_maxY)
			return room_A.m_position.m_minX < room_B.m_position.m_minX;
		else
			return room_A.m_position.m_maxY > room_B.m_position.m_maxY;
	};

	std::sort(copied_Rooms.begin(), copied_Rooms.end(), sortRoom);

	/*for (auto room : copied_Rooms)
		std::cout << "Room MaxY: " << room.m_position.m_maxY << std::endl;*/

		// Flush buffer - Format purpose s
	std::cout << std::endl;

	//for (auto room : copied_Rooms)
	//{
	//	std::cout << "Room MinX: " << room.m_position.m_minX << std::endl;
	//	std::cout << "Room MaxY: " << room.m_position.m_maxY << std::endl;
	//}

	// Flush buffer - Format purpose 
	std::cout << std::endl;

	return copied_Rooms;
}

void Building::ProceduralBuildingGeneration()
{
	// Construct the rooms
	for (int i = 0; i < m_rooms.size(); ++i)
		ConstructRoom(m_rooms[i]);

	// Connect the rooms with door 
	for (size_t i = 0; i < m_rooms.size() - 1; ++i)
	{
		Room& currRoom = m_rooms[i];
		Room& adj_Room = m_rooms[i + 1];

		//Direction currDoorDir = Direction::Null;
		//Direction adj_DoorDir = Direction::Null;
		ConstructDoor(currRoom, adj_Room);
	}

	//for (auto& room : m_rooms)
	//{
	//	std::cout << "Room Index: " << room.m_index << std::endl;
	//	std::cout << " Min X: " << room.m_position.m_minX << " Max X: " << room.m_position.m_maxX << std::endl;
	//	std::cout << " Min Y: " << room.m_position.m_minY << " Max Y: " << room.m_position.m_maxY << std::endl;
	//}

	std::vector<Room> order_Rooms = CustomSortAlgorithm();

	// Create a vector with enough cell size 
	// const Room& topLeft_Room = order_Rooms.front();
	// const Room& btmRight_Room = order_Rooms.back();

	// Find the width and height of map -- minX, maxX, minY, maxY
	int map_min_X = 0, map_min_Y = 0, map_max_X = 0, map_max_Y = 0;

	for (auto room : order_Rooms)
	{
		if (room.m_position.m_minX < map_min_X)
			map_min_X = room.m_position.m_minX;

		if (room.m_position.m_maxX > map_max_X)
			map_max_X = room.m_position.m_maxX;

		if (room.m_position.m_minY < map_min_Y)
			map_min_Y = room.m_position.m_minY;

		if (room.m_position.m_maxY > map_max_Y)
			map_max_Y = room.m_position.m_maxY;
	}

	// Width and Height of the map
	size_t map_Width = static_cast<size_t>(map_max_X) - map_min_X;
	size_t map_Height = static_cast<size_t>(map_max_Y) - map_min_Y;

	// Just make a big temp vector
	visual_Map.resize(map_Width * map_Height);

	// Initialise visual map
	for (auto& elem : visual_Map)
		elem = CellType::null;

	// Figure out where to place the rooms in the cell

	for (auto room : order_Rooms)
	{
		int minX_Offset = 0, maxY_Offset = 0;

		// Check offset from minX, maxY
		minX_Offset = room.m_position.m_minX - map_min_X;
		maxY_Offset = map_max_Y - room.m_position.m_maxY;

		// Index to start writing from
		size_t startIndex = minX_Offset + maxY_Offset * map_Width;

		if (startIndex < 0)
			startIndex = 0;

		//std::cout << "startIndex: " << startIndex << std::endl;

		size_t itrIndex = startIndex;

		// Incorporate the map into the order Rooms
		for (size_t h = 0; h < room.m_height; ++h)
		{
			// Update the startIndex
			itrIndex = startIndex + h * map_Width;

			for (size_t w = 0; w < room.m_width; ++w)
			{
				// Overwrite the big map data
				CellType currCellType = visual_Map[itrIndex + w];

				// Give priority to certain cells
				/*if (currCellType != CellType::door && currCellType != CellType::wall &&
					currCellType != CellType::wall_1 && currCellType != CellType::wall_2 &&
					currCellType != CellType::wall_3 && currCellType != CellType::wall_4 &&
					currCellType != CellType::wall_5 && currCellType != CellType::wall_6 &&
					currCellType != CellType::wall_7 && currCellType != CellType::wall_8 &&
					currCellType != CellType::wall_9) */
				if (currCellType != CellType::door)
					visual_Map[itrIndex + w] = room.m_cellType[w + h * room.m_width];
			}
		}
	}

	// Debug and see how the map looks like
	for (int h = 0; h < map_Height; ++h)
	{
		for (int w = 0; w < map_Width; ++w)
		{
			// New height to print out
			if (w % map_Width == 0)
				std::cout << "\n";

			std::cout << "[";
			switch (visual_Map[w + h * map_Width])
			{
			case CellType::null:
				std::cout << " ";
				break;

			case CellType::floor:
				std::cout << " ";
				break;

			case CellType::door:
				std::cout << "D";
				break;

			case CellType::wall:
				std::cout << static_cast<int>(CellType::wall);
				break;
			case CellType::wall_1:
				std::cout << static_cast<int>(CellType::wall_1);
				break;
			case CellType::wall_2:
				std::cout << static_cast<int>(CellType::wall_2);
				break;
			case CellType::wall_3:
				std::cout << static_cast<int>(CellType::wall_3);
				break;
			case CellType::wall_4:
				std::cout << static_cast<int>(CellType::wall_4);
				break;
			case CellType::wall_5:
				std::cout << static_cast<int>(CellType::wall_5);
				break;
			case CellType::wall_6:
				std::cout << static_cast<int>(CellType::wall_6);
				break;
			case CellType::wall_7:
				std::cout << static_cast<int>(CellType::wall_7);
				break;
			case CellType::wall_8:
				std::cout << static_cast<int>(CellType::wall_8);
				break;
			case CellType::wall_9:
				std::cout << static_cast<int>(CellType::wall_9);
				break;
			default:
				break;
			}
			std::cout << "]";

			// Print the room cell type
			//std::cout << '[' << static_cast<int>(visual_Map[w + h * map_Width]) << "]";

		}
	}

	// Format Purpose
	std::cout << std::endl;
}

void Building::Print()
{
	// Print out the rooms & details
	for (unsigned int i = 0; i < m_rooms.size(); ++i)
	{
		std::cout << "Room Index: " << i;
		std::cout << " Width: " << m_rooms[i].m_width;
		std::cout << " Height: " << m_rooms[i].m_height;
		std::cout << "\n";

		std::cout << "Min X: " << m_rooms[i].m_position.m_minX << "  ";
		std::cout << "Min Y: " << m_rooms[i].m_position.m_minY << "\n";

		std::cout << "Max X: " << m_rooms[i].m_position.m_maxX << "  ";
		std::cout << "Max Y: " << m_rooms[i].m_position.m_maxY << "\n";

		unsigned doorCount = 0;

		for (auto doorDir : m_rooms[i].m_doorDirs)
		{
			std::cout << "doorCount: " << doorCount++ << " ..... ";

			switch (doorDir)

			{
			case Direction::North:

				std::cout << "North" << std::endl;
				break;

			case Direction::South:

				std::cout << "South" << std::endl;
				break;

			case Direction::West:

				std::cout << "West" << std::endl;
				break;

			case Direction::East:

				std::cout << "East" << std::endl;
				break;

			default:
				break;
			}
		}
	}

	//std::cout << "\n";
	//std::cout << "floorType: " << static_cast<int>(CellType::floor) << std::endl;
	//std::cout << "doorType: " << static_cast<int>(CellType::door) << std::endl;
	//std::cout << "wallType: " << static_cast<int>(CellType::wall) << std::endl;
	//std::cout << "\n";

	// Print out each room
	for (unsigned int i = 0; i < m_rooms.size(); ++i)
	{
		for (unsigned int j = 0; j < m_rooms[i].m_cellType.size(); ++j)
		{
			// New height to print out
			if (j % m_rooms[i].m_width == 0)
				std::cout << "\n";

			// Print the room cell type
			std::cout << '[' << static_cast<int>(m_rooms[i].m_cellType[j]) << "]";
		}

		std::cout << "\n\n";
	}

	// Flush the buffer
	std::cout << std::endl;
}