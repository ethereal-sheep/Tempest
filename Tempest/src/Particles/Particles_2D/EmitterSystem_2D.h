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
#include "Emitter_2D.h"

// Forward Declarations
struct ImVec2;

struct WaypointEmitter_2D;
struct ExplosionEmitter_2D;
struct CircularMotionEmitter_2D;
struct RandomMinMaxSpawnPointEmitter_2D;

struct LineEmitter_2D;

struct EmitterSystem_2D
{
	static EmitterSystem_2D& GetInstance();

	//struct Handler
	//{
	//	Handler(std::shared_ptr<Emitter_2D> emitter)
	//		: m_weakEmmitters{ emitter }
	//	{}

	//	std::weak_ptr<Emitter_2D> m_weakEmmitters;

	//	void reset()
	//	{
	//		m_weakEmmitters.reset();
	//	}
	//	[[nodiscard]] bool IsDead()
	//	{
	//		return m_weakEmmitters.expired();
	//	}
	//};

	//Handler Register(glm::vec2 pos)
	//{
	//	auto emitter = std::make_shared<Emitter_2D>();
	//	emitter->m_GM.m_position = pos;
	//	m_emitters.push_back(emitter);

	//	return Handler(emitter); // Weak ptr, refer to Handler
	//}

	//Emitter_2D& Register(glm::vec2 pos)
	//{
	//	auto emitter = std::make_shared<Emitter_2D>();
	//	emitter->m_GM.m_position = pos;
	//	m_emitters.push_back(emitter);

	//	return *emitter.get(); // Bad local variable, please change this code
	//}

	void Update();
	auto get_emitters() const
	{
		return m_emitters;
	}

	void Reset()
	{
		m_emitters.clear();
	}

	// ----------------------- Screenspace Particle_2D -------------

	void CreateButtonEmitter(std::weak_ptr<WaypointEmitter_2D>& emitter, ImVec2 topLeftPos, ImVec2 buttonSize);
	void CreateButtonEmitter(std::weak_ptr<WaypointEmitter_2D>& emitter, glm::vec2 topLeftPos, glm::vec2 buttonSize);

	void CreateExplosionEmitter(std::weak_ptr<ExplosionEmitter_2D>& emitter, glm::vec2 spawnPos);
	void CreateExplosionEmitter(std::weak_ptr<ExplosionEmitter_2D>& emitter, ImVec2 spawnPos);

	void CreateCircularMotionEmitter_2D(std::weak_ptr<CircularMotionEmitter_2D>& emitter, glm::vec2 centrePos, float radius);
	void CreateCircularMotionEmitter_2D(std::weak_ptr<CircularMotionEmitter_2D>& emitter, ImVec2 centrePos, float radius);

	void CreateBackgroundEmitter(std::weak_ptr<RandomMinMaxSpawnPointEmitter_2D>& emitter, ImVec2 min, ImVec2 max);
	void CreateBackgroundEmitter(std::weak_ptr<RandomMinMaxSpawnPointEmitter_2D>& emitter, glm::vec2 min, glm::vec2 max);

	void CreateLineEmitter(std::weak_ptr<LineEmitter_2D>& emitter, ImVec2 startPoint, ImVec2 endPoint);
	void CreateLineEmitter(std::weak_ptr<LineEmitter_2D>& emitter, glm::vec2 startPoint, glm::vec2 endPoint);

	//--------------------------------------------------------------

private:
	EmitterSystem_2D();

	std::vector <std::shared_ptr<Emitter_2D>> m_emitters;

	// Emitter_2D Management
	std::set<short> m_UniqueEmitterSlots;
	std::queue<short> m_availableEmitterSlots;

	// Helper functions
	void AddEmitter(const std::shared_ptr<Emitter_2D> emitter);
};

#endif // !PARTICLE_SYSTEM_2D_H
