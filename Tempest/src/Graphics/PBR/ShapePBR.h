/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \author		Tiong Jun Ming, Jerome (j.tiong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "Core.h"
#include "glew.h"
#include "glm.hpp"

#include "Graphics/OpenGL/Camera.h"
#include "Graphics/OpenGL/Shader.h"
namespace Tempest
{
	class ShapePBR
	{
    public:
        std::string shapeType;
        GLfloat shapeAngle;
        glm::vec3 shapePosition;
        glm::vec3 shapeRotationAxis;
        glm::vec3 shapeScale;
        GLuint shapeVAO, shapeVBO, shapeEBO, shapeDiffuseID, shapeSpecularID;

        ShapePBR();
        ~ShapePBR();
        void setShape(std::string type, glm::vec3 position);
        void drawShape(const tuptr<Shader>& lightingShader, glm::mat4& view, glm::mat4& projection, Camera& camera);
        void drawShape();
        std::string getShapeType();
        glm::vec3 getShapePosition();
        GLfloat getShapeAngle();
        glm::vec3 getShapeRotationAxis();
        glm::vec3 getShapeScale();
        GLuint getShapeVAO();
        void setShapePosition(glm::vec3 position);
        void setShapeAngle(GLfloat angle);
        void setShapeRotationAxis(glm::vec3 rotationAxis);
        void setShapeScale(glm::vec3 scale);
	};
}

