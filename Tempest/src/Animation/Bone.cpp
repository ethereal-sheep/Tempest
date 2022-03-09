#include "Bone.h"

namespace Tempest
{
	Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
		: m_Name{name}, m_ID{ID}, m_LocalTransform{1.f}
	{
		m_NumPositions = channel->mNumPositionKeys;

		for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
		{
			aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
			float timeStamp = channel->mPositionKeys[positionIndex].mTime;
			KeyPosition data;
			data.m_Position = AssimpHelper::GetGLMVec(aiPosition);
			data.m_TimeStamp = timeStamp;
			m_Positions.push_back(data);
		}

		m_NumRotations = channel->mNumRotationKeys;
		for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
		{
			aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
			float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
			KeyRotation data;
			data.m_Orientation = AssimpHelper::GetGLMQuat(aiOrientation);
			data.m_TimeStamp = timeStamp;
			m_Rotations.push_back(data);
		}

		m_NumScalings = channel->mNumScalingKeys;
		for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
		{
			aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
			float timeStamp = channel->mScalingKeys[keyIndex].mTime;
			KeyScale data;
			data.m_Scale = AssimpHelper::GetGLMVec(scale);
			data.m_TimeStamp = timeStamp;
			m_Scales.push_back(data);
		}
	}

	void Bone::Update(float animationTime)
	{
		glm::mat4 translation = InterpolatePosition(animationTime);
		glm::mat4 rotation = InterpolateRotation(animationTime);
		glm::mat4 scale = InterpolateScaling(animationTime);
		m_LocalTransform = translation * rotation * scale;
	}

	int Bone::GetPositionIndex(float animationTime)
	{
		for (int index = 0; index < m_NumPositions - 1; ++index)
		{
			if (animationTime < m_Positions[index + 1].m_TimeStamp)
				return index;
		}
	}

	int Bone::GetRotationIndex(float animationTime)
	{
		for (int index = 0; index < m_NumRotations - 1; ++index)
		{
			if (animationTime < m_Rotations[index + 1].m_TimeStamp)
				return index;
		}
	}

	int Bone::GetScaleIndex(float animationTime)
	{
		for (int index = 0; index < m_NumScalings - 1; ++index)
		{
			if (animationTime < m_Scales[index + 1].m_TimeStamp)
				return index;
		}
	}

	float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		float scaleFactor = 0.0f;
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;
		scaleFactor = midWayLength / framesDiff;
		return scaleFactor;
	}

	glm::mat4 Bone::InterpolatePosition(float animationTime)
	{
		if (1 == m_NumPositions)
			return glm::translate(glm::mat4(1.0f), m_Positions[0].m_Position);

		int p0Index = GetPositionIndex(animationTime);
		if (p0Index == m_Positions.size() - 1)
			p0Index = 0;
		int p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Positions[p0Index].m_TimeStamp, m_Positions[p1Index].m_TimeStamp, animationTime);
		glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].m_Position, m_Positions[p1Index].m_Position, scaleFactor);
		return glm::translate(glm::mat4(1.0f), finalPosition);
	}

	glm::mat4 Bone::InterpolateRotation(float animationTime)
	{
		if (1 == m_NumRotations)
		{
			auto rotation = glm::normalize(m_Rotations[0].m_Orientation);
			return glm::toMat4(rotation);
		}

		int p0Index = GetRotationIndex(animationTime);
		if (p0Index == m_Rotations.size() - 1)
			p0Index = 0;
		int p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Rotations[p0Index].m_TimeStamp, m_Rotations[p1Index].m_TimeStamp, animationTime);
		glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].m_Orientation, m_Rotations[p1Index].m_Orientation, scaleFactor);
		finalRotation = glm::normalize(finalRotation);
		return glm::toMat4(finalRotation);
	}

	glm::mat4 Bone::InterpolateScaling(float animationTime)
	{
		if (1 == m_NumScalings)
			return glm::scale(glm::mat4(1.0f), m_Scales[0].m_Scale);

		int p0Index = GetScaleIndex(animationTime);
		if (p0Index == m_Scales.size() - 1)
			p0Index = 0;
		int p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Scales[p0Index].m_TimeStamp, m_Scales[p1Index].m_TimeStamp, animationTime);
		glm::vec3 finalScale = glm::mix(m_Scales[p0Index].m_Scale, m_Scales[p1Index].m_Scale, scaleFactor);
		return glm::scale(glm::mat4(1.0f), finalScale);
	}
}