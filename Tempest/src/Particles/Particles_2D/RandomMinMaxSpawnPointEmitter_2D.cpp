

#include "RandomMinMaxSpawnPointEmitter_2D.h"
#include "../Random.h"

RandomMinMaxSpawnPointEmitter_2D::RandomMinMaxSpawnPointEmitter_2D()
	: m_Min { 0.0f }
	, m_Max { 0.0f }
{

}

glm::vec2 RandomMinMaxSpawnPointEmitter_2D::RandomPoint()
{
	// A random point between the axis 
	glm::vec2 distance = m_Max - m_Min;

	// Pick the random point
	glm::vec2 randomPoint_2D = m_Min;
	randomPoint_2D.x += Random::Float() * distance.x;
	randomPoint_2D.y += Random::Float() * distance.y;

	return randomPoint_2D;
}

void RandomMinMaxSpawnPointEmitter_2D::Emit(const int particleAmount)
{
	// Emit only if enough particle
	if (particleAmount > 0 && m_available_ParticleSlots.size() > 0)
	{
		for (short i = 0; i < particleAmount; ++i)
		{
			// Initailisation of the particle
			Particle_2D particle;
			Emitter_2D::ParticleSetUp(particle);

			particle.m_position = RandomPoint();

			short spawnSector = std::rand() % 4;
			short directionX = 1;
			short directionY = 1;

			switch (spawnSector)
			{
			case 0:
				directionX = -directionX;
				directionY = directionY;
				break;
			case 1:
				directionX = directionX;
				directionY = directionY;
				break;
			case 2:
				directionX = -directionX;
				directionY = -directionY;
				break;
			case 3:
				directionX = directionX;
				directionY = -directionY;
				break;

			default:
				break;
			}

			particle.m_velocityBegin.x *= directionX;
			particle.m_velocityEnd.x *= directionX;

			particle.m_velocityBegin.y *= directionY;
			particle.m_velocityEnd.y *= directionY;


			// Allocation of particle
			m_particles[m_available_ParticleSlots.front()] = particle;
			m_available_ParticleSlots.pop();

			if (m_available_ParticleSlots.size() <= 0)
				break;
		}
	}
}