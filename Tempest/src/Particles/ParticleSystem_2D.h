


#ifndef PARTICLE_SYSTEM_2D_H
#define PARTICLE_SYSTEM_2D_H

#include "Core.h"

#include <glm.hpp>
#include <queue>

//#include "../../Editor/src/Extern/imgui/imgui.h"


// Note, glm::vectors are used to utilise the mix function
struct ParticleSystem_2D
{
	enum class ParticleType
	{
		Circle = 0
		, Square = 1
	};

	struct Particle
	{
		Particle();
		Particle(glm::vec2 pos, glm::vec2 vec, glm::vec4 colour, float size, float lifeTime, bool isActive, ParticleType particleType);

		glm::vec2 m_position;
		glm::vec2 m_velocity;
		
		//float m_rotation;

		glm::vec4 m_colour;
		float m_size;
		float m_lifeTime;
		float m_lifeRemaining;
		bool  m_isActive;

		ParticleType m_type;
	};

	struct Emitter
	{
		glm::vec2 m_position;

		// Particles Management
		std::vector<Particle> m_particles;
		std::queue<short> m_available_ParticleSlots;

		Emitter();
		void Update(const float dt);

		// Test Function
		void Emit(const int particleAmount);
		//void EmitSquare(const int particleAmount);

		glm::vec2 m_startVelocity, m_endVelocity;
		glm::vec2 m_velocityVariation;

		float m_sizeBegin, m_sizeEnd;
		float m_sizeVariation;		

		glm::vec4 m_colourBegin, m_colourEnd;

		float m_lifeTime;							
		//bool m_loop						
		int m_maxParticles;
		int m_particleIndex;
		short m_rateOvertime; // How much particles to spawn per unit of time

		ParticleType m_type;

		bool  m_clockwise;
		float m_xOffset, m_yOffset;
	};

	struct Handler
	{
		Handler(std::shared_ptr<Emitter> emitter)
			: m_weakEmmitters{ emitter }
		{}

		std::weak_ptr<Emitter> m_weakEmmitters;

		void reset()
		{
			m_weakEmmitters.reset();
		}
		[[nodiscard]] bool IsDead()
		{
			return m_weakEmmitters.expired();
		}
	};

	Handler Register(glm::vec2 pos)
	{
		auto emitter = std::make_shared<Emitter>();
		emitter->m_position = pos;
		m_emitters.push_back(emitter);

		return Handler(emitter); // Weak ptr, refer to Handler
	}

	void Update(const float dt);

	auto get_emitters() const
	{
		return m_emitters;
	}

private:
	std::vector <std::shared_ptr<Emitter>> m_emitters;
};

#endif // !PARTICLE_SYSTEM_2D_H
