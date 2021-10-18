/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include <glew.h>
#include <string>

#include "TMath.h"

namespace Tempest
{
	class Shader
	{
	private:

		GLuint id;
		std::string LoadShaderSource(const std::string& fileName);

		GLuint LoadShader(GLenum type, const std::string& fileName);
		void LinkProgram(GLuint vertexShader, GLuint fragmentShader);
		void LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geomShader);
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geomSrc);
		~Shader();

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		void Bind();
		void Unbind();
		void Set1i(GLint value, const GLchar* name);
		void Set1f(GLfloat value, const GLchar* name);
		void SetVec2f(glm::fvec2 value, const GLchar* name);
		void SetVec3f(glm::fvec3 value, const GLchar* name);
		void SetVec4f(glm::fvec4 value, const GLchar* name);
		void SetMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE);
		void SetMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);
		void SetIntArray(int* values, GLuint count, const std::string& name);
	};
}


