


#ifndef PARTICLE_SYSTEM_2D_H
#define PARTICLE_SYSTEM_2D_H

#include "Core.h"

#include "Math/elsVector2.h"
#include "Math//elsVector4.h"

#include <glm.hpp>

//#include "../../Editor/src/Extern/imgui/imgui.h"

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
		
		float m_rotation;

		glm::vec4 m_colour;
		float m_size;
		float m_lifeTime;
		float m_lifeRemaining;
		bool  m_isActive;

		// For square drawlist
		/*float minX, maxX;
		float minY, maxY;*/

		ParticleType m_type;
	};

	struct Emitter
	{
		Emitter();
		void Update(const float dt);

		// Test Function
		void Emit(const int particleAmount, ParticleType particleType);
		//void EmitSquare(const int particleAmount);

		glm::vec2 m_position;
		
		// The particle it emitts
		std::vector<Particle> m_particles;

		//------------------------------
		// Data the User can manipulate - This idea is put on hold for now. TO be handled by backend programemrs.
		// Velocity Controls (3)
		glm::vec2 m_startVelocity, m_endVelocity;	// Velocity of particles
		glm::vec2 m_velocityVariation;				// Different velocity spawn

		// Size Controls (3)
		float m_sizeBegin, m_sizeEnd;			// Size transition of particles
		float m_sizeVariation;					// Different size spawning

		// Colour Controls (2)
		glm::vec4 m_colourBegin, m_colourEnd;	// Colour transition of particles

		// Other controls (...)
		float m_lifeTime;				// Life time of the particles
		//bool m_loop;					// Can the emitter loop spawn
		int m_maxParticles;				// Max amount of particles an emitter can have 

		//float m_rateOvertime;			// How much particles to spawn per unit of time
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
