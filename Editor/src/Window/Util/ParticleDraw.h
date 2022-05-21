


#ifndef PARTICLE_DRAW_H
#define PARTICLE_DRAW_H

#include "Instance/Instance.h"

#include "../../Tempest/src/Particles/Particles_2D/EmitterSystem_2D.h"
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

			for (auto& emitter : EmitterSystem_2D::GetInstance().get_emitters())
			{
				// Render the particle
				if (emitter->m_GM.m_active)
					for (auto& particle : emitter->m_particles)
					{
						if (particle.m_isActive)
						{
							// Centre Point
							ImVec2 pos;
							pos.x = particle.m_position.x;
							pos.y = particle.m_position.y;

							// Colour
							ImVec4 colour;
							colour.x = particle.m_colour.r;
							colour.y = particle.m_colour.g;
							colour.z = particle.m_colour.b;
							colour.w = particle.m_colour.a;

							// Min and Max
							ImVec2 min, max;
							min.x = pos.x - particle.m_size * 0.5f;
							min.y = pos.y - particle.m_size * 0.5f;

							max.x = pos.x + particle.m_size * 0.5f;
							max.y = pos.y + particle.m_size * 0.5f;

							auto rotatePoint = [](float cx, float cy, float radian, ImVec2 point)
							{
								float s = sin(radian);
								float c = cos(radian);

								// translate point back to origin:
								point.x -= cx;
								point.y -= cy;

								// rotate point
								//float xnew = point.x * c + point.y * s;
								//float ynew = point.x * -s + point.y * c;
								float xnew = point.x * c - point.y * s;
								float ynew = point.x * s + point.y * c;

								// translate point back:
								point.x = xnew + cx;
								point.y = ynew + cy;
								return point;
							};

							// 4 points
							ImVec2 btmLeft, btmRight, topLeft, topRight;

							btmLeft.x = min.x;
							btmLeft.y = min.y;

							btmRight.x = max.x;
							btmRight.y = min.y;

							topLeft.x = min.x;
							topLeft.y = max.y;

							topRight.x = max.x;
							topRight.y = max.y;

							// Rotate the 4 points
							btmLeft  = rotatePoint(pos.x, pos.y, particle.m_rotation, btmLeft);
							btmRight = rotatePoint(pos.x, pos.y, particle.m_rotation, btmRight);
							topLeft  = rotatePoint(pos.x, pos.y, particle.m_rotation, topLeft);
							topRight = rotatePoint(pos.x, pos.y, particle.m_rotation, topRight);

							if (particle.m_type == ParticleType::Circle)
								drawlist->AddCircleFilled(pos, particle.m_size, ImGui::GetColorU32({ colour }));
							else if (particle.m_type == ParticleType::Square)
							{
								//drawlist->AddRectFilled(min, max, ImGui::GetColorU32({ colour }));
								drawlist->AddQuadFilled(btmLeft, btmRight, topRight, topLeft, ImGui::GetColorU32({ colour }));
								//drawlist->AddQuadFilled(topLeft, topRight, btmRight, btmLeft, ImGui::GetColorU32({ colour }));
							}
						}
					}
			}
		}
	};
}

#endif // !PARTICLE_DRAW_H




