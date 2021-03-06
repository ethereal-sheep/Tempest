/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Shader.h"
#include <fstream>
#include "Logger/Log.h"

namespace Tempest
{
	std::string Shader::LoadShaderSource(const std::string& fileName)
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
			LOG(fileName.c_str());
		}

		in_file.close();

		return src;
	}

	/*
		Create a shader for linking
	*/
	GLuint Shader::LoadShader(GLenum type, const std::string& fileName)
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
	void Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader)
	{
		char infoLog[512];
		GLint success;

		id = glCreateProgram();

		glAttachShader(id, vertexShader);

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
		Create a program for OpenGL to link and use the shader + geom
	*/
	void Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geomShader)
	{
		char infoLog[512];
		GLint success;

		id = glCreateProgram();

		glAttachShader(id, vertexShader);

		glAttachShader(id, fragmentShader);

		glAttachShader(id, geomShader);

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
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		GLuint vertex_Shader = 0;
		GLuint fragment_Shader = 0;

		vertex_Shader = LoadShader(GL_VERTEX_SHADER, vertexSrc);
		fragment_Shader = LoadShader(GL_FRAGMENT_SHADER, fragmentSrc);

		LinkProgram(vertex_Shader, fragment_Shader);

		//End
		glDeleteShader(vertex_Shader);
		glDeleteShader(fragment_Shader);
	}

	/*
	Creates an object with a shader program loaded + geom
*/
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geomSrc)
	{
		GLuint vertex_Shader = 0;
		GLuint fragment_Shader = 0;
		GLuint geom_Shader = 0;
		vertex_Shader = LoadShader(GL_VERTEX_SHADER, vertexSrc);
		fragment_Shader = LoadShader(GL_FRAGMENT_SHADER, fragmentSrc);
		geom_Shader = LoadShader(GL_GEOMETRY_SHADER, geomSrc);
		LinkProgram(vertex_Shader, fragment_Shader, geom_Shader);

		//End
		glDeleteShader(vertex_Shader);
		glDeleteShader(fragment_Shader);
		glDeleteShader(geom_Shader);
	}


	/*
		Delete the program from memory
	*/
	Shader::~Shader()
	{
		glDeleteProgram(id);
	}

	//---------------------------------------------------------------------------------
	//				Setting Uniform Functions for Linking with Shaders
	//---------------------------------------------------------------------------------
	void Shader::Bind()
	{
		glUseProgram(id);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	void Shader::Set1i(GLint value, const GLchar* name)
	{
		glUniform1i(glGetUniformLocation(id, name), value);
	}

	void Shader::Set1f(GLfloat value, const GLchar* name)
	{
		glUniform1f(glGetUniformLocation(id, name), value);
	}

	void Shader::SetVec2f(glm::fvec2 value, const GLchar* name)
	{
		glUniform2fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
	}

	void Shader::SetVec3f(glm::fvec3 value, const GLchar* name)
	{
		glUniform3fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
	}

	void Shader::SetVec4f(glm::fvec4 value, const GLchar* name)
	{
		glUniform4fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
	}

	void Shader::SetMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose)
	{
		glUniformMatrix3fv(glGetUniformLocation(id, name), 1, transpose, glm::value_ptr(value));
	}

	void Shader::SetMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name), 1, transpose, glm::value_ptr(value));
	}

	void Shader::SetIntArray(int* values, GLuint count, const std::string& name)
	{
		glUniform1iv(glGetUniformLocation(id, name.c_str()), count, values);
	}

	GLint Shader::GetUniformLoc(const std::string& name)
	{
		return glGetUniformLocation(id, name.c_str());
	}
}