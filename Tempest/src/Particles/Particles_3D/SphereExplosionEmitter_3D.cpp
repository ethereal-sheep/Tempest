



#include "SphereExplosionEmitter_3D.h"

void SphereExplosionEmitter_3D::ParticleSetUp(Particle_3D& particle)
{
	// Base setup of explosion - Default setup + RNG spawnRange + RNG spawn Velocity
	ExplosionEmitter_3D::ParticleSetUp(particle);

	//// Rotate based on spawn velocity
	//short spawnRotation = std::rand() % 7;

	//// Assume clockwise rotation of the axis
	//switch (spawnRotation)
	//{
	//case 0:
	//	particle.m_rotation = 0; 
	//	break;
	//case 1:
	//	// X Axis -> Negative X
	//	// Y Axis -> Negative Y
	//	// Z Axis -> Negative Z
	//	particle.m_rotation = 270;
	//	break;

	//case 2:
	//	// Z Axis -> Negative Z
	//	particle.m_rotation = 180;
	//	break;
	//case 3:
	//	// Y Axis -> Negative Y
	//	particle.m_rotation = 180;
	//	break;
	//case 4:
	//	// Y Axis -> Negative Y
	//	// Z Axis -> Negative Z
	//	particle.m_rotation = 270;
	//	break;
	//case 5:
	//	// X Axis -> Negative X
	//	particle.m_rotation = 180;
	//	break;
	//case 6:
	//	// X Axis -> Negative X
	//	// Z Axis -> Negative Z
	//	particle.m_rotation = 270;
	//	break;
	//default:
	//	break;
	//}

	//particle.m_rotation = 60;
}