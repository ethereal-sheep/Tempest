/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "FrameRateManager.h"
#include "Logger/Log.h"

#define DEFAULT_MAX_FPS 60

FrameRateManager& FrameRateManager::GetInstance()
{
	static FrameRateManager instance;
	return instance;
}

FrameRateManager::FrameRateManager()
	: m_dt{ 0.0f }
	, m_displayFPS{ 0 }
	, m_timeScale{ 1.0f }
	, m_startTime{ clock() }
	, m_endTime{ clock() }
	, m_rFPS{ 0 }
	, m_maxFPS{ DEFAULT_MAX_FPS }
	, m_fixedDeltaTime{ 1.0f / DEFAULT_MAX_FPS }
	, mv_fixedDeltaTime{ 1.0f / DEFAULT_MAX_FPS }
	, m_perSecond{ 0.0f }
	, m_countFPS{ 0 }
	, m_dtAccumulate{ 0.0f }
	, m_fdtUpdateNo{ 0 }
{

}

void FrameRateManager::FrameStart()
{
	// Start of a new frame
	m_startTime = clock();

	// Accumulated dt for fixed_dt calculation later
	m_dtAccumulate += m_dt;

	// If the amount of dt accumulated exceeds a FDT
	while (m_dtAccumulate >= m_fixedDeltaTime)
	{
		// Decrease accumulation based on FDT
		m_dtAccumulate -= m_fixedDeltaTime;

		// Amount of times systems relying on FDT to update that frame
		++m_fdtUpdateNo;

		mv_fixedDeltaTime *= m_timeScale;
	}
}

void FrameRateManager::FrameEnd()
{
	// Reset
	m_fdtUpdateNo = 0;

	// End of the current frame
	m_endTime = clock();

	// Calculate dt
	m_dt = static_cast<float>(m_endTime - m_startTime) / CLOCKS_PER_SEC;

	// For fast and slow motion
	m_dt *= m_timeScale;

	// The real frames per second instead of basing the latest dt per frame
	++m_countFPS;

	// Frames per second based on dt
	m_displayFPS = static_cast<unsigned>(1.0f / m_dt);

	// Timer to reset the count of frames per second
	m_perSecond += m_dt;

	// Reset the counter for m_rFPS
	if (m_perSecond > 1.0f)
	{
		m_rFPS = m_countFPS;
		m_countFPS = 0;
		m_perSecond = 0.0f;
	}

	// Processor completes frame faster than required
	while (m_dt < static_cast<float>(m_fixedDeltaTime))
	{
		m_endTime = clock();

		//_dt = static_cast<float>(_startTime - _endTime) / CLOCKS_PER_SEC;
		m_dt = static_cast<float>(m_endTime - m_startTime) / CLOCKS_PER_SEC;

		m_displayFPS = 60;
	}

	//LOG_INFO("DT: {0}", m_dt);
}

float FrameRateManager::GetDT() const
{
	return m_dt;
}

float FrameRateManager::GetFDT() const
{
	return m_fixedDeltaTime;
}

float FrameRateManager::GetVFDT() const
{
	return mv_fixedDeltaTime;
}

unsigned FrameRateManager::GetFPS() const
{
	return m_displayFPS;
}

unsigned FrameRateManager::GetRFPS() const
{
	return m_rFPS;
}

float FrameRateManager::GetTimeScale() const
{
	return m_timeScale;
}

unsigned FrameRateManager::GetFDT_UpdateNo() const
{
	return m_fdtUpdateNo;
}

void FrameRateManager::SetMaxFPS(unsigned maxFPS)
{
	m_maxFPS = maxFPS;
	m_fixedDeltaTime = 1.0f / m_maxFPS;
}

void FrameRateManager::SetTimeScale(float timeScale)
{
	m_timeScale = timeScale;
}
