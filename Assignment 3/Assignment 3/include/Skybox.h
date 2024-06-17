/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : Skybox.h
Description : Definitions for skybox rendering in OpenGL
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#pragma once

#include "Shader.h"

#include <glew.h>
#include <glm.hpp>
#include <vector>

class Skybox
{
public:
	explicit Skybox(const std::vector<std::string>& Faces);

	void draw(const Shader& Shader) const;

private:
	void setupSkybox();
	static unsigned int loadCubeMap(const std::vector<std::string>& Faces);

	unsigned int MVao;
	unsigned int MVbo;
	unsigned int MCubeMapTexture;
};
