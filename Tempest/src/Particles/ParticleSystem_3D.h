/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef PARTICLE_SYSTEM_2D_H
#define PARTICLE_SYSTEM_2D_H

#include "Core.h"
#include "Emitter.h"

// Forward Declarations
struct Trail_WaypointEmitter_3D;
struct ExplosionEmitter_3D;
struct CircularMotionEmitter_2D;
struct ImVec2;

struct ParticleSystem_3D
{
	static ParticleSystem_3D& GetInstance();

	//struct Handler
	//{
	//	Handler(std::shared_ptr<Emitter> emitter)
	//		: m_weakEmmitters{ emitter }
	//	{}

	//	std::weak_ptr<Emitter> m_weakEmmitters;

	//	void reset()
	//	{
	//		m_weakEmmitters.reset();
	//	}
	//	[[nodiscard]] bool IsDead()
	//	{
	//		return m_weakEmmitters.expired();
	//	}
	//};

	//Handler Register(glm::vec3 pos)
	//{
	//	auto emitter = std::make_shared<Emitter>();
	//	emitter->m_GM.m_position = pos;
	//	m_emitters.push_back(emitter);

	//	return Handler(emitter); // Weak ptr, refer to Handler
	//}

	//Emitter& Register(glm::vec3 pos)
	//{
	//	auto emitter = std::make_shared<Emitter>();
	//	emitter->m_GM.m_position = pos;
	//	m_emitters.push_back(emitter);

	//	return *emitter.get(); // Bad local variable, please change this code
	//}

	void Update();
	auto get_emitters() const
	{
		return m_emitters;
	}

	const std::weak_ptr<Trail_WaypointEmitter_3D> CreateButtonEmitter(glm::vec3 topLeftPos, glm::vec3 buttonSize);
	const std::weak_ptr<Trail_WaypointEmitter_3D> CreateButtonEmitter(ImVec2 topLeftPos, ImVec2 buttonSize);

	void ReuseButtonEmitter(const std::shared_ptr<Trail_WaypointEmitter_3D>& emitter, glm::vec3 topLeftPos, glm::vec3 buttonSize);
	void ReuseButtonEmitter(const std::shared_ptr<Trail_WaypointEmitter_3D>& emitter, ImVec2 topLeftPos, ImVec2 buttonSize);

	const std::weak_ptr<ExplosionEmitter_3D> CreateExplosionEmitter_3D(glm::vec3 spawnPos);
	const std::weak_ptr<ExplosionEmitter_3D> CreateExplosionEmitter_3D(ImVec2 spawnPos);

	void ReuseExplosionEmitter_3D(const std::shared_ptr<ExplosionEmitter_3D>& emitter, glm::vec3 spawnPos);
	void ReuseExplosionEmitter_3D(const std::shared_ptr<ExplosionEmitter_3D>& emitter, ImVec2 spawnPos);

	const std::weak_ptr<CircularMotionEmitter_2D> CreateCircularMotionEmitter_2D(glm::vec3 centrePos, float radius);
	const std::weak_ptr<CircularMotionEmitter_2D> CreateCircularMotionEmitter_2D(ImVec2 centrePos, float radius);

	void ReuseCircularMotionEmitter_2D(const std::shared_ptr<CircularMotionEmitter_2D>& emitter, ImVec2 centrePos, float radius);
	void ReuseCircularMotionEmitter_2D(const std::shared_ptr<CircularMotionEmitter_2D>& emitter, glm::vec3 centrePos, float radius);

private:
	ParticleSystem_3D();
	std::vector <std::shared_ptr<Emitter>> m_emitters;

	// Emitter Management
	std::set<short> m_UniqueEmitterSlots;
	std::queue<short> m_availableEmitterSlots;

	// Helper functions
	void AddEmitter(const std::shared_ptr<Emitter> emitter);
};

#endif // !PARTICLE_SYSTEM_2D_H
