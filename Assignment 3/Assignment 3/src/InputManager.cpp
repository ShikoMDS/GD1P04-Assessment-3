/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : InputManager.cpp
Description : Implementations for InputManager class
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#include "InputManager.h"

#include <iostream>

InputManager::InputManager(Camera& Camera, LightManager& LightManager)
	: MCamera(Camera), MLightManager(LightManager), MWireframe(false), MCursorVisible(false),
	  MFirstMouse(true), MLastX(800 / 2.0f), MLastY(600 / 2.0f)
{
	MKeyState = {
		{GLFW_KEY_1, false},
		{GLFW_KEY_2, false},
		{GLFW_KEY_3, false},
		{GLFW_KEY_4, false},
		{GLFW_KEY_5, false}
	};
}

void InputManager::processInput(GLFWwindow* Window, const float DeltaTime)
{
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(Window, true);

	for (auto& [key, pressed] : MKeyState)
	{
		if (glfwGetKey(Window, key) == GLFW_PRESS)
		{
			if (!pressed)
			{
				pressed = true;
				MKeyToggleState[key] = !MKeyToggleState[key];
				if (key == GLFW_KEY_1) MLightManager.togglePointLights();
				if (key == GLFW_KEY_2) MLightManager.toggleDirectionalLight();
				if (key == GLFW_KEY_3) MLightManager.toggleSpotLight();
				if (key == GLFW_KEY_4) toggleWireframeMode();
				if (key == GLFW_KEY_5) toggleCursorVisibility(Window);
			}
		}
		else
		{
			pressed = false;
		}
	}

	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		MCamera.processKeyboard(Forward, DeltaTime);
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
		MCamera.processKeyboard(Backward, DeltaTime);
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
		MCamera.processKeyboard(Left, DeltaTime);
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
		MCamera.processKeyboard(Right, DeltaTime);
	if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
		MCamera.processKeyboard(Up, DeltaTime);
	if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
		MCamera.processKeyboard(Down, DeltaTime);
}

void InputManager::toggleWireframeMode()
{
	MWireframe = !MWireframe;
	if (MWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void InputManager::toggleCursorVisibility(GLFWwindow* Window)
{
	MCursorVisible = !MCursorVisible;
	if (MCursorVisible)
	{
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		MFirstMouse = true;
	}
	else
	{
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void InputManager::frameBufferSizeCallback(GLFWwindow* Window, const int Width, const int Height)
{
	glViewport(0, 0, Width, Height);
}

void InputManager::mouseCallback(GLFWwindow* Window, const double PosX, const double PosY)
{
	if (MCursorVisible) return;

	if (MFirstMouse)
	{
		MLastX = PosX;
		MLastY = PosY;
		MFirstMouse = false;
	}

	const float OffsetX = PosX - MLastX;
	const float OffsetY = MLastY - PosY;

	MLastX = PosX;
	MLastY = PosY;

	MCamera.processMouseMovement(OffsetX, OffsetY);
}

void InputManager::scrollCallback(GLFWwindow* Window, double OffsetX, const double OffsetY) const
{
	MCamera.processMouseScroll(static_cast<float>(OffsetY));
}
