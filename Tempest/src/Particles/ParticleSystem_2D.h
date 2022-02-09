


#ifndef PARTICLE_SYSTEM_2D_H
#define PARTICLE_SYSTEM_2D_H

#include "Core.h"
#include "Emitter.h"

// Forward Declarations
struct WaypointEmitter;
struct ExplosionEmitter;
struct CircularMotionEmitter;
struct ImVec2;

struct ParticleSystem_2D
{
	static ParticleSystem_2D& GetInstance();

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

	//Handler Register(glm::vec2 pos)
	//{
	//	auto emitter = std::make_shared<Emitter>();
	//	emitter->m_GM.m_position = pos;
	//	m_emitters.push_back(emitter);

	//	return Handler(emitter); // Weak ptr, refer to Handler
	//}

	//Emitter& Register(glm::vec2 pos)
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

	// Not supposed to be here - NOT TO BE SHARED POINTER
	const std::shared_ptr<WaypointEmitter> ButtonEmitter(glm::vec2 topLeftPos, glm::vec2 buttonSize);
	const std::shared_ptr<WaypointEmitter> ButtonEmitter(ImVec2 topLeftPos, ImVec2 buttonSize);

	void ReuseButtonEmitter(const std::shared_ptr<WaypointEmitter>& emitter, glm::vec2 topLeftPos, glm::vec2 buttonSize);
	void ReuseButtonEmitter(const std::shared_ptr<WaypointEmitter>& emitter, ImVec2 topLeftPos, ImVec2 buttonSize);

	const std::shared_ptr<ExplosionEmitter> ExplosionEmitter_2(glm::vec2 spawnPos);
	const std::shared_ptr<ExplosionEmitter> ExplosionEmitter_2(ImVec2 spawnPos);

	void ReuseExplosionEmitter(const std::shared_ptr<ExplosionEmitter>& emitter, glm::vec2 spawnPos);
	void ReuseExplosionEmitter(const std::shared_ptr<ExplosionEmitter>& emitter, ImVec2 spawnPos);

	const std::shared_ptr<CircularMotionEmitter> CircularMotionEmitter_2(glm::vec2 centrePos, float radius);
	const std::shared_ptr<CircularMotionEmitter> CircularMotionEmitter_2(ImVec2 centrePos, float radius);

private:
	ParticleSystem_2D();
	std::vector <std::shared_ptr<Emitter>> m_emitters;

	// Emitter Management
	std::set<short> m_UniqueEmitterSlots;
	std::queue<short> m_availableEmitterSlots;

	// Helper functions
	void AddEmitter(const std::shared_ptr<Emitter> emitter);
};

#endif // !PARTICLE_SYSTEM_2D_H
