/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Util.h"
#include "Core.h"
#include "TMath.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "AssimpHelper.h"

namespace Tempest
{
	struct BoneInfo
	{
		int m_ID;				// ID for FinalBoneMatrix
		glm::mat4 m_Offset;		// Offset Matrix to transform model space to bone space
	};

	struct KeyPosition
	{
		glm::vec3 m_Position;
		float m_TimeStamp;
	};

	struct KeyRotation
	{
		glm::quat m_Orientation;
		float m_TimeStamp;
	};

	struct KeyScale
	{
		glm::vec3 m_Scale;
		float m_TimeStamp;
	};

	class Bone
	{
	private:

		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
		glm::mat4 InterpolatePosition(float animationTime);
		glm::mat4 InterpolateRotation(float animationTime);
		glm::mat4 InterpolateScaling(float animationTime);

	public:
		Bone(const std::string& name, int ID, const aiNodeAnim* channel);
		void Update(float animationTime);

		int GetPositionIndex(float animationTime);
		int GetRotationIndex(float animationTime);
		int GetScaleIndex(float animationTime);

		glm::mat4 GetLocalTransform() { return m_LocalTransform; }
		std::string GetBoneName() const { return m_Name; }
		int GetBoneID() const { return m_ID; }

	private:

		tvector<KeyPosition> m_Positions;
		tvector<KeyRotation> m_Rotations;
		tvector<KeyScale> m_Scales;

		int m_NumPositions;
		int m_NumRotations;
		int m_NumScalings;

		glm::mat4 m_LocalTransform;
		std::string m_Name;
		int m_ID;
	};
}