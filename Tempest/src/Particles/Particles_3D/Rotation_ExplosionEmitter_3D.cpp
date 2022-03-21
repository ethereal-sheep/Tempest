


#include "Rotation_ExplosionEmitter_3D.h"
#include "Logger/Log.h"

#define PI 3.141592653589793238f

Rotation_ExplosionEmitter_3D::Rotation_ExplosionEmitter_3D()
{
	m_GM.m_position = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_GM.m_active = true;

	m_MM.m_duration = 0.3f;
	m_MM.m_preWarm = true;
	Emitter_3D::UpdateMaxParticle(1000);

	m_EM.m_spawnTimeInterval = 3.0f;
	m_EM.m_spawnCountTimer = m_EM.m_spawnTimeInterval;
	m_EM.m_rateOverTime = 20;
	
	// Particle Architype values - without consideration for default ctor
	m_PAM.m_gravity = true;
	m_PAM.m_startVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_endVelocity = glm::vec3{ 0.f, 0.f, 0.0f };
	m_PAM.m_velocityVariation = glm::vec3{ 10.0f, 10.0f, 10.0f };

	m_PAM.m_scaleBegin = glm::vec3{ 0.1f, 0.1f, 0.1f };
	m_PAM.m_scaleEnd = glm::vec3{ 0.0f, 0.0f, 0.0f };
	m_PAM.m_scaleVariation = glm::vec3{ 0.3f, 0.3f, 0.3f };

	m_PAM.m_colourBegin = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };
	m_PAM.m_colourEnd = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };

	m_PAM.m_lifeTime = 0.3f;
	m_RM.m_renderingPath = "Models/Cube.a";

}

void Rotation_ExplosionEmitter_3D::ParticleSetUp(Particle_3D& particle)
{
	// Position, velcoity and base variables are all done here
	ExplosionEmitter_3D::ParticleSetUp(particle);

	// Set the rotation

	// Shift the particle back by emitter to origin point
	auto shiftedPosition = particle.m_position - m_GM.m_position;

	// Dot Product
	float dotProduct_X_Axis = particle.m_velocity.x * 1;
	float dotProduct_Y_Axis = particle.m_velocity.y * 1;
	float dotProduct_Z_Axis = particle.m_velocity.z * 1;

	// Magnitude of First Vector
	float magnitude_ParticleVelocity = std::sqrtf((particle.m_velocity.x * particle.m_velocity.x) +
												  (particle.m_velocity.y * particle.m_velocity.y) +
												  (particle.m_velocity.z * particle.m_velocity.z));

	// Magnitude of Second Vector - Unit Vector (Axis)
	float magnitude_Axis = 1;

	// Calculate angle to rotate in degrees
	float radian_X = std::acosf(dotProduct_X_Axis / magnitude_ParticleVelocity * magnitude_Axis);
	float radian_Y = std::acosf(dotProduct_Y_Axis / magnitude_ParticleVelocity * magnitude_Axis);
	float radian_Z = std::acosf(dotProduct_Z_Axis / magnitude_ParticleVelocity * magnitude_Axis);

	//LOG_INFO("Velocity X: {0}", particle.m_velocity.x);
	//LOG_INFO("Velocity Y: {0}", particle.m_velocity.y);
	//LOG_INFO("Velocity Z: {0}", particle.m_velocity.z);


	//LOG_INFO("Degree X: {0}", radian_X * 180.0f / 3.14159f);
	//LOG_INFO("Degree Y: {0}", radian_Y * 180.0f / 3.14159f);
	//LOG_INFO("Degree Z: {0}", radian_Z * 180.0f / 3.14159f);

	/*
	* Rotation X = Pitch = Y = Left Right
	* Rotation Y = Yaw   = Z = Up Down
	* Rotation Z = Roll  = X = Front Back
	*/

	particle.m_rotation.x = radian_X * 180.0f / 3.14159f;
	particle.m_rotation.y = radian_Y * 180.0f / 3.14159f;
	particle.m_rotation.z = radian_Z * 180.0f / 3.14159f;
}