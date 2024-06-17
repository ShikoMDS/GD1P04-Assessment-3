/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : Shader.cpp
Description : Implementations for Shader class
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* VertexPath, const char* FragmentPath)
{
	// Retrieve the vertex/fragment source code from filePath
	std::string VertexCode;
	std::string FragmentCode;
	std::ifstream VShaderFile;
	std::ifstream FShaderFile;
	// Ensure ifstream objects can throw exceptions
	VShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	FShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Open files
		VShaderFile.open(VertexPath);
		FShaderFile.open(FragmentPath);
		std::stringstream VShaderStream, FShaderStream;
		// Read file's buffer contents into streams
		VShaderStream << VShaderFile.rdbuf();
		FShaderStream << FShaderFile.rdbuf();
		// Close file handlers
		VShaderFile.close();
		FShaderFile.close();
		// Convert stream into string
		VertexCode = VShaderStream.str();
		FragmentCode = FShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* VShaderCode = VertexCode.c_str();
	const char* FShaderCode = FragmentCode.c_str();

	// Compile shaders
	unsigned int Vertex, Fragment;
	int Success;
	char InfoLog[512];

	// Vertex shader
	Vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(Vertex, 1, &VShaderCode, nullptr);
	glCompileShader(Vertex);
	glGetShaderiv(Vertex, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(Vertex, 512, nullptr, InfoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLog << '\n';
	}

	// Fragment shader
	Fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(Fragment, 1, &FShaderCode, nullptr);
	glCompileShader(Fragment);
	glGetShaderiv(Fragment, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(Fragment, 512, nullptr, InfoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << InfoLog << '\n';
	}

	// Shader program
	Id = glCreateProgram();
	glAttachShader(Id, Vertex);
	glAttachShader(Id, Fragment);
	glLinkProgram(Id);
	glGetProgramiv(Id, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(Id, 512, nullptr, InfoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLog << '\n';
	}

	// Delete the shaders as they are now linked and no longer necessary
	glDeleteShader(Vertex);
	glDeleteShader(Fragment);
}

void Shader::use() const
{
	glUseProgram(Id);
}

void Shader::setBool(const std::string& Name, const bool Value) const
{
	glUniform1i(glGetUniformLocation(Id, Name.c_str()), static_cast<int>(Value));
}

void Shader::setInt(const std::string& Name, const int Value) const
{
	glUniform1i(glGetUniformLocation(Id, Name.c_str()), Value);
}

void Shader::setFloat(const std::string& Name, const float Value) const
{
	glUniform1f(glGetUniformLocation(Id, Name.c_str()), Value);
}

void Shader::setVec3(const std::string& Name, const glm::vec3& Value) const
{
	glUniform3fv(glGetUniformLocation(Id, Name.c_str()), 1, &Value[0]);
}

void Shader::setVec3(const std::string& Name, const float X, const float Y, const float Z) const
{
	glUniform3f(glGetUniformLocation(Id, Name.c_str()), X, Y, Z);
}

void Shader::setMat4(const std::string& Name, const glm::mat4& Mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(Id, Name.c_str()), 1, GL_FALSE, &Mat[0][0]);
}

void Shader::setMaterial(const std::string& Name, const Material& Material) const
{
	setVec3(Name + ".ambient", Material.Ambient);
	setVec3(Name + ".diffuse", Material.Diffuse);
	setVec3(Name + ".specular", Material.Specular);
	setFloat(Name + ".shininess", Material.Shininess);
}

void Shader::setLight(const std::string& Name, const Light& Light) const
{
	setVec3(Name + ".position", Light.Position);
	setVec3(Name + ".ambient", Light.Ambient);
	setVec3(Name + ".diffuse", Light.Diffuse);
	setVec3(Name + ".specular", Light.Specular);
}

void Shader::checkCompileErrors(const unsigned int Shader, const std::string& Type)
{
	int Success;
	char InfoLog[1024];
	if (Type != "PROGRAM")
	{
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(Shader, 1024, nullptr, InfoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << Type << "\n" << InfoLog <<
				"\n -- --------------------------------------------------- -- " <<
				'\n';
		}
	}
}

void Shader::checkLinkErrors(const unsigned int Program)
{
	int Success;
	char InfoLog[1024];
	glGetProgramiv(Program, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(Program, 1024, nullptr, InfoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << InfoLog <<
			"\n -- --------------------------------------------------- -- " <<
			'\n';
	}
}
