/**********************************************************************************
* \author		Chue Jun Hao(c.junhao@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef FrameRateManager_H
#define FrameRateManager_H

#include <time.h>

class FrameRateManager
{
public:
	static FrameRateManager& GetInstance();

	void FrameStart();
	void FrameEnd();

	// Getters
	float GetDT() const;
	float GetFDT() const;
	float GetVFDT() const;

	unsigned GetFPS() const;
	unsigned GetRFPS() const;
	float GetTimeScale() const;

	unsigned GetFDT_UpdateNo() const;

	// Setter
	void SetMaxFPS(unsigned maxFPS);
	void SetTimeScale(float timeScale);

private:
	FrameRateManager();

	// Used by Engine
	float m_dt;				// The delta time per frame
	unsigned m_displayFPS;	// The FPS dependant on the m_dt - Not the real frame per second
	float m_timeScale;		// For fast and slow motion

	// Internal Calculations
	clock_t m_startTime;	// Time when the frame start
	clock_t m_endTime;		// Time when the frame end

	// Do we really need this?
	unsigned m_maxFPS;		// Max Frame per second

	// Count the real frame per second that actually was ran
	unsigned m_rFPS;			// Real Frame per second
	unsigned m_countFPS;		// Counter for rFPS
	float m_perSecond;			// Timer for every second passed

	// Fixed DT
	float m_fixedDeltaTime;		// Fixed delta time - FDT
	float mv_fixedDeltaTime;	// Volatile FDT
	float m_dtAccumulate;		// Accumulation of deltaTime 
	unsigned m_fdtUpdateNo;		// Amount of times for run systems using FDT
};

#endif // !FrameRateManager_H

#define T_FrameRateManager FrameRateManager::GetInstance()