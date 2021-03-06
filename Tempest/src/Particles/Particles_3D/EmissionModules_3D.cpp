/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "EmissionModules_3D.h"

MainModule_3D::MainModule_3D()
	: m_duration{ 10.0f }
	, m_looping{ false }
	, m_maxParticles{ 500 }
	, m_simulationSpeed{ 0.016f }
	, m_preWarm{ false }
{}

GameObjectModule_3D::GameObjectModule_3D()
	: m_position{ 0.0f, 0.0f, 0.0f }
	, m_velocity{ 0.0f, 0.0f, 0.0f }
	, m_active{ true }
{}

ParticleArchetypeModule_3D::ParticleArchetypeModule_3D()
	: m_minSpawnPos{ 0.0f, 0.0f, 0.0f }
	, m_maxSpawnPos{ 0.0f, 0.0f, 0.0f }
	
	, m_SpawnRotation{ 0, 0, 0 }

	, m_velocityStart{ 0.0f, 0.0f, 0.0f }
	, m_velocityEnd { 0.0f, 0.0f, 0.0f }
	, m_velocityVariation { 3.0f, 1.0f, 0.0f }

	, m_scaleBegin{ 20.0f, 20.0f, 20.0f }
	, m_scaleEnd{ 0.0f, 0.0f, 0.0f }
	, m_scaleVariation{ 0.3f, 0.3f, 0.3f }

	, m_colourBegin{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f }
	, m_colourEnd{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }
	//, m_colourVariation { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }

	, m_lifeTime{ 2.0f }
	//, m_lifeVariation{ 5.0f }
	, m_rebirth { false }
	, m_gravity { false }
{}

EmissionModule_3D::EmissionModule_3D()
	: m_burstTime{ 0.f }
	, m_burstInterval{ 0.f }
	, m_burstCycle{ 0 }
	, m_burstCount{ 0 }

	, m_rateOverTime{ 0 }
	, m_spawnTimeInterval{ 0.0f }
	, m_spawnCountTimer{ m_spawnTimeInterval }

	, m_burstCountTimer {0.0f}
	, m_startBurstCycle { false }

{}

RendererModule_3D::RendererModule_3D()
	: m_renderingPath{ "Models/Cube.a" }
	, m_emissiveLighting { false }
{}