#include "DrawShader.h"
#include <fstream>
#include "Logger/Log.h"

namespace Tempest
{
	std::string DrawShader::LoadShaderSource(const std::string& fileName)
	{
		std::string temp = "";
		std::string src = "";

		std::ifstream in_file;

		in_file.open(fileName.c_str());

		if (in_file.is_open())
		{
			while (std::getline(in_file, temp))
				src += temp + "\n";
		}
		else
		{
			LOG("Failed to open shader file");
		}

		in_file.close();

		return src;
	}

	/*
		Create a shader for linking
	*/
	GLuint DrawShader::LoadShader(GLenum type, const std::string& fileName)
	{
		char infoLog[512];
		GLint success;

		GLuint shader = glCreateShader(type);
		std::string str_src = LoadShaderSource(fileName.c_str());
		const GLchar* src = str_src.c_str();
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			LOG("Failed to compile shader");
			LOG(fileName);
		}

		return shader;
	}

	/*
		Create a program for OpenGL to link and use the shader
	*/
	void DrawShader::LinkProgram(GLuint fragmentShader)
	{
		char infoLog[512];
		GLint success;

		id = glCreateProgram();

		glAttachShader(id, fragmentShader);

		glLinkProgram(id);

		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			LOG("Failed to link program");
		}

		glUseProgram(0);
	}

	/*
		Creates an object with a shader program loaded
	*/
	DrawShader::DrawShader(const std::string& fragmentSrc)
	{
		GLuint fragment_Shader = 0;

		fragment_Shader = LoadShader(GL_FRAGMENT_SHADER, fragmentSrc);

		LinkProgram(fragment_Shader);

		//End
		glDeleteShader(fragment_Shader);
	}

	/*
		Delete the program from memory
	*/
	DrawShader::~DrawShader()
	{
		glDeleteProgram(id);
	}

	//---------------------------------------------------------------------------------
	//				Setting Uniform Functions for Linking with Shaders
	//---------------------------------------------------------------------------------
	void DrawShader::Bind()
	{
		glUseProgram(id);
	}

	void DrawShader::Unbind()
	{
		glUseProgram(0);
	}

	void DrawShader::Set1i(GLint value, const GLchar* name)
	{
		glUniform1i(glGetUniformLocation(id, name), value);
	}

	void DrawShader::Set1f(GLfloat value, const GLchar* name)
	{
		glUniform1f(glGetUniformLocation(id, name), value);
	}

	void DrawShader::SetVec2f(glm::fvec2 value, const GLchar* name)
	{
		glUniform2fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
	}

	void DrawShader::SetVec3f(glm::fvec3 value, const GLchar* name)
	{
		glUniform3fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
	}

	void DrawShader::SetVec4f(glm::fvec4 value, const GLchar* name)
	{
		glUniform4fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
	}

	void DrawShader::SetMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose)
	{
		glUniformMatrix3fv(glGetUniformLocation(id, name), 1, transpose, glm::value_ptr(value));
	}

	void DrawShader::SetMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name), 1, transpose, glm::value_ptr(value));
	}

	void DrawShader::SetIntArray(int* values, GLuint count, const std::string& name)
	{
		glUniform1iv(glGetUniformLocation(id, name.c_str()), count, values);
	}
}