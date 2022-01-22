


#ifndef PARTICLE_SYSTEM_2D_H
#define PARTICLE_SYSTEM_2D_H

#include "Core.h"
#include "Emitter.h"

//#include "../../Editor/src/Extern/imgui/imgui.h"

struct ParticleSystem_2D
{
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
