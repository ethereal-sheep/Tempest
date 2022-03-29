

#include "RandomSpawnPointEmitter_2D.h"
#include "../Random.h"


RandomSpawnPointEmitter_2D::RandomSpawnPointEmitter_2D()
	: m_Min { 0.0f, 0.0f }
	, m_Max { 0.0f, 0.0f }
{

}


glm::vec2 RandomSpawnPointEmitter_2D::RandomPoint()
{
	// The in between distance of the 2D Axis
	glm::vec2 distance = m_Max - m_Min;

	// A random point between the axis 
	glm::vec2 randomDistance = Random::Float() * distance;

	// Pick the random point
	glm::vec2 randomPoint_2D = m_Min + randomDistance;

	return randomPoint_2D;
}