


#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Core.h"

#include "Math/elsVector2.h"
#include "Math//elsVector4.h"

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
		els::vec2f m_position;
		els::vec2f m_velocity;
		
		els::vec4f m_colour;
		float m_size;
		float m_lifeTime;
		float m_lifeRemaining;
	};

	struct Emitter
	{
		void Update(const float dt);

		els::vec2f m_position;
		// The particle it emitts
		std::vector<Particle> m_particles;

		// Data the User can manipulate
		// Velocity Controls (3)
		els::vec2f m_startVelocity, m_endVelocity;	// Velocity of particles
		els::vec2f m_velocityVariation;				// Different velocity spawn

		// Size Controls (3)
		float m_startSize, m_endSize;			// Size transition of particles
		float m_sizeVariation;					// Different size spawning

		// Colour Controls (2)
		els::vec4f m_colourBegin, m_colourEnd;	// Colour transition of particles

		// Other controls (...)
		float m_lifeTime;		// Life time of the particles
		bool m_loop;			// Can the emitter loop spawn
		int m_maxParticles;		// Max amount of particles an emitter can have 

		float m_rateOvertime;	// How much particles to spawn per unit of time
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

	Handler Register()
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
