


#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Core.h"

#include "Math/elsVector2.h"
#include "Math//elsVector4.h"

#include <glm.hpp>

// Forward Declaration
struct ParticleSystem;

//class instance
//{
//public:
//	void Update(const float dt) { particleSystem.Update(dt); }
//
//private:
//	ParticleSystem particleSystem;
//};

struct ParticleSystem
{
	struct Particle
	{
		Particle();

		glm::vec2 m_position;
		glm::vec2 m_velocity;
		
		glm::vec4 m_colour;
		float m_size;
		float m_lifeTime;
		float m_lifeRemaining;
	};

	struct Emitter
	{
		Emitter();
		void Update(const float dt);

		glm::vec2 m_position;
		// The particle it emitts
		std::vector<Particle> m_particles;

		// Data the User can manipulate - This idea is put on hold for now. TO be handled by backend programemrs.
		// Velocity Controls (3)
		glm::vec2 m_startVelocity, m_endVelocity;	// Velocity of particles
		glm::vec2 m_velocityVariation;				// Different velocity spawn

		// Size Controls (3)
		float m_startSize, m_endSize;			// Size transition of particles
		float m_sizeVariation;					// Different size spawning

		// Colour Controls (2)
		glm::vec4 m_colourBegin, m_colourEnd;	// Colour transition of particles

		// Other controls (...)
		float m_lifeTime;				// Life time of the particles
		bool m_loop;					// Can the emitter loop spawn
		int m_maxParticles;				// Max amount of particles an emitter can have 

		float m_rateOvertime;			// How much particles to spawn per unit of time
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

	Handler Register() //Weak pointer here - to be change
	{
		auto emitter = std::make_shared<Emitter>();
		m_emitters.push_back(emitter);

		return emitter;
	}

	void Update(const float dt);

	auto get_emitters() const
	{
		return m_emitters;
	}

private:
	std::vector <std::shared_ptr<Emitter>> m_emitters;
};

#endif // !PARTICLE_SYSTEM_H
