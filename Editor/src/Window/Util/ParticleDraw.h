


#ifndef PARTICLE_DRAW_H
#define PARTICLE_DRAW_H

#include "Instance/Instance.h"

#include "../../Tempest/src/Particles/ParticleSystem_2D.h"
#include "../../Tempest/src/Logger/Logger.h"

namespace Tempest
{
	class ParticleDraw : public Window
	{
		const char* window_name() override
		{
			return "";
		}

		void show(Instance&) override
		{
			auto drawlist = ImGui::GetForegroundDrawList();

			//LOG_INFO("Draw Particles");

			for (auto& emitter : ParticleSystem_2D::GetInstance().get_emitters())
			{
				// Render the particle
				if (emitter->m_GM.m_active)
					for (auto& particle : emitter->m_particles)
					{
						if (particle.m_isActive)
						{
							ImVec2 pos;
							pos.x = particle.m_position.x;
							pos.y = particle.m_position.y;

							ImVec4 colour;
							colour.x = particle.m_colour.r;
							colour.y = particle.m_colour.g;
							colour.z = particle.m_colour.b;
							colour.w = particle.m_colour.a;

							ImVec2 min, max;
							min.x = pos.x - particle.m_size * 0.5f;
							max.x = pos.x + particle.m_size * 0.5f;

							min.y = pos.y - particle.m_size * 0.5f;
							max.y = pos.y + particle.m_size * 0.5f;

							if (particle.m_type == ParticleType::Circle)
								drawlist->AddCircleFilled(pos, particle.m_size, ImGui::GetColorU32({ colour }));
							else if (particle.m_type == ParticleType::Square)
								drawlist->AddRectFilled(min, max, ImGui::GetColorU32({ colour }));
						}
					}
			}
		}
	};
}

#endif // !PARTICLE_DRAW_H




