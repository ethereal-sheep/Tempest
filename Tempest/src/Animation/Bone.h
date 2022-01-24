#pragma once
#include "TMath.h"
#include "Util.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Tempest
{
	struct BoneInfo
	{
		int id;

		glm::mat4 offset;

	};

	struct KeyPosition
	{
		glm::vec3 position;
		float timeStamp;
	};

	struct KeyRotation
	{
		glm::quat orientation;
		float timeStamp;
	};

	struct KeyScale
	{
		glm::vec3 scale;
		float timeStamp;
	};

	class Bone
	{
	public:

		Bone(const std::string& name, int ID, const aiNodeAnim* channel);
		void Update(float time);
		int GetPositionIndex(float time);
		int GetRotationIndex(float time);
		int GetScaleIndex(float time);

		glm::mat4 GetLocalTransform() { return m_LocalTransform; }
		std::string GetBoneName() const { return m_Name; }
		int GetBoneID() { return m_ID; }

	private:

		float GetScaleFactor(float t0, float t1, float time);
		glm::mat4 InterpolatePosition(float time);
		glm::mat4 InterpolateRotation(float time);
		glm::mat4 InterpolateScale(float time);

		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;
		std::vector<KeyScale> m_Scales;
		int m_NumPositions;
		int m_NumRotations;
		int m_NumScalings;

		glm::mat4 m_LocalTransform;
		std::string m_Name;
		int m_ID;
	};
}