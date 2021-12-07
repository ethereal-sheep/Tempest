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
#include "ShapePBR.h"

namespace Tempest
{
    GLfloat cubeVertices[] =
    {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };


    GLfloat planeVertices[] =
    {
        0.5f,  -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        0.5f,  -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        0.5f,  -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f
    };


    GLfloat quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };


    ShapePBR::ShapePBR()
    {

    }

    ShapePBR::~ShapePBR()
    {

    }


    void ShapePBR::setShape(std::string type, glm::vec3 position)
    {
        this->shapeType = type;
        this->shapePosition = position;
        this->shapeScale = glm::vec3(1.0f, 1.0f, 1.0f);
        this->shapeAngle = 0;
        this->shapeRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);

        glGenVertexArrays(1, &shapeVAO);
        glGenBuffers(1, &shapeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->shapeVBO);

        if (type == "cube")
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        else if (type == "plane")
            glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        else if (type == "quad")
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glBindVertexArray(this->shapeVAO);

        if (type == "quad")
        {
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        }

        else
        {
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        }

        glBindVertexArray(0);
    }

    // not used
    void ShapePBR::drawShape(const tuptr<Shader>& lightingShader, glm::mat4& view, glm::mat4& projection, Camera& camera)
    {
        lightingShader->Bind();

        lightingShader->SetMat4fv(view, "view");
        lightingShader->SetMat4fv(projection, "projection");
        lightingShader->SetVec3f(camera.GetPosition(), "viewPos");

        glm::mat4 model;
        model = glm::translate(model, this->shapePosition);
        model = glm::scale(model, this->shapeScale);
        model = glm::rotate(model, this->shapeAngle, this->shapeRotationAxis);
        lightingShader->SetMat4fv(model, "model");

        glBindVertexArray(this->shapeVAO);

        if (this->shapeType == "cube")
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (this->shapeType == "plane")
            glDrawArrays(GL_TRIANGLES, 0, 6);
        else if (this->shapeType == "quad")
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindVertexArray(0);
    }


    void ShapePBR::drawShape()
    {
        glBindVertexArray(this->shapeVAO);

        if (this->shapeType == "cube")
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (this->shapeType == "plane")
            glDrawArrays(GL_TRIANGLES, 0, 6);
        else if (this->shapeType == "quad")
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindVertexArray(0);
    }


    std::string ShapePBR::getShapeType()
    {
        return this->shapeType;
    }


    glm::vec3 ShapePBR::getShapePosition()
    {
        return this->shapePosition;
    }


    GLfloat ShapePBR::getShapeAngle()
    {
        return this->shapeAngle;
    }


    glm::vec3 ShapePBR::getShapeRotationAxis()
    {
        return this->shapeRotationAxis;
    }


    glm::vec3 ShapePBR::getShapeScale()
    {
        return this->shapeScale;
    }


    GLuint ShapePBR::getShapeVAO()
    {
        return this->shapeVAO;
    }


    void ShapePBR::setShapePosition(glm::vec3 position)
    {
        this->shapePosition = position;
    }


    void ShapePBR::setShapeAngle(GLfloat angle)
    {
        this->shapeAngle = angle;
    }


    void ShapePBR::setShapeRotationAxis(glm::vec3 rotationAxis)
    {
        this->shapeRotationAxis = rotationAxis;
    }


    void ShapePBR::setShapeScale(glm::vec3 scale)
    {
        this->shapeScale = scale;
    }

}