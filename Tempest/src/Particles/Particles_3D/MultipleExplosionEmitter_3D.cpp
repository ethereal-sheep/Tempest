

#include "MultipleExplosionEmitter_3D.h"
#include "ExplosionEmitter_3D.h"
#include "ParticleSystem_3D.h"

MultipleExplosionEmitter_3D::MultipleExplosionEmitter_3D(int explosionEmitterAmount)
	: m_explosionEmitterAmount{ explosionEmitterAmount }
{}


void MultipleExplosionEmitter_3D::Emit(const int explosionEmitterAmount)
{
	// Emit only if enough particle
	for (short i = 0; i < m_explosionEmitterAmount; ++i)
	{
		short spawnSector = std::rand() % 7;
		short directionX = (std::rand() % 40);
		short directionY = (std::rand() % 40);
		short directionZ = (std::rand() % 40);

		// Spawn Velocity Direction
		switch (spawnSector)
		{
		case 0:
			directionX = directionX;
			directionY = directionY;
			directionZ = directionZ;
			break;
		case 1:
			directionX = -directionX;
			directionY = -directionY;
			directionZ = -directionZ;
			break;

		case 2:
			directionX = directionX;
			directionY = directionY;
			directionZ = -directionZ;
			break;
		case 3:
			directionX = directionX;
			directionY = -directionY;
			directionZ = directionZ;
			break;
		case 4:
			directionX = directionX;
			directionY = -directionY;
			directionZ = -directionZ;
			break;
		case 5:
			directionX = -directionX;
			directionY = directionY;
			directionZ = directionZ;
			break;
		case 6:
			directionX = -directionX;
			directionY = directionY;
			directionZ = -directionZ;
			break;

		default:
			break;
		}

		// Creation of explosion emitter
		const std::shared_ptr<ExplosionEmitter_3D> tempEmitter = ParticleSystem_3D::GetInstance().CreateTestEmitter(glm::vec3{ 0.0f, 0.0f, 0.0f }).lock();

		// Emitter's velocity
		tempEmitter->m_GM.m_velocity = glm::vec3{ directionX, directionY, directionZ };
	}

	// Performs emit only once
	m_explosionEmitterAmount = 0;
}