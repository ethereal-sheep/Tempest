#pragma once

#include <glew.h>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
#include <string>
#include <fstream>

namespace Tempest
{
	class Shader
	{
	private:

		GLuint id;
		std::string LoadShaderSource(const std::string& fileName);

		GLuint LoadShader(GLenum type, const std::string& fileName);
		void LinkProgram(GLuint vertexShader, GLuint fragmentShader);

	public:

		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

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


