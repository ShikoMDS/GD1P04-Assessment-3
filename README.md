# Assignment 3  
  
OpenGL project demonstrating a 3D scene with focus on lighting created by Ayoub for GD1P04 3D Graphics Programming Summative 3.  
  
  
## Table of Contents  
  
- [Description](#Description)  
- [Features](#Features)  
- [Requirements](#Requirements)  
- [Installation](#Installation)  
- [Controls](#Controls)  
- [Issues](#Issues)
- [Disclaimer](#Disclaimer)  
- [Credits](#Credits)  
  
  
## Description  
  
This OpenGL project demonstrates a 3D scene with instanced rendering, a skybox, lighting, and camera controls.  
A Visual Studio 2022 solution is provided, however "main.cpp" and individual source files "/src/" and include (header) files "/include/" can be accessed individually in their respective folders.  
Dependencies and resources are situated in their respective folders.  
  
  
## Features  
- Instanced Rendering: Efficiently renders a number of instances of a model with unique transformations
- Lighting: Three different lighting options compatible with each other
- Camera: Allows for movement through the scene  
  
  
## Requirements  
  
- Visual Studio 2022 (v143)  
- Windows SDK 10.0  
- C++ Language Standard: ISO C++20 Standard (/std:c++20)  
- C Language Standard: ISO C17 (2018) Standard (/std:c17)  
- Windows x64  
- OpenGL dependencies (GLEW, GLFW, GLM)  
- STB dependency  
- tinyobjloader dependency  
  
  
## Installation  
  
Download and extract from the .zip file.  
This program can be run from the .exe file in the release folder. Only relevant resources are provided in their respective folder and dependencies are provided.  
This program can also be run by opening the "Assignment 3.sln" file in the source code folder and built in Debug or Release mode with Windows x64.  
  
  
## Controls  
  
The program has been designed to be operated with standard keyboard and mouse controls.  

#### Camera Movement  
- Up Arrow: Zoom in (shortens distance to 0, 0, 0)  
- Down Arrow: Zoom out (increases distance from 0, 0, 0)  
- Left Arrow: Rotation clockwise around (0, 0, 0) along the (X, Z) plane  
- Right Arrow: Rotation anti-clockwise around (0, 0, 0) along the (X, Z) plane  
- Left Shift: Increases camera movement speed while held  
- Spacebar: Toggles automatic camera rotation  
  
#### Functions  
- 1: Toggle point lights  
- 2: Toggle directional lighting  
- 3: Toggle spotlight  
- 4: Toggles wire frame mode  
- 5: Toggles cursor visibility and camera "look" movement  
  
  
## Issues  

Reflection model was not implemented. Had issues with the project overall and have left out due to time constraints.    
DLL ISSUES - If there are issues with .dll files not being found, they must be moved into the correct directories.  
  
  
## Disclaimer  
  
This program is as complete as possible for submission. Most errors should not be present and warnings should only come from STB and tinyobjloader. Some warnings I have not fixed up, such as conversions.  
All code was written by Shiko based off my own knowledge from classes with lecturers and self driven research of the C++ coding language.  
This program has been cleaned for submission. Known issues have been searched for and fixed, unknown bugs may still be present.  
.vs/Assignment 3/v17/.suo is included for ease of solution loadup purposes due to the solution being manually configured. The solution should start with x64 Release, with main.cpp and LightManager.h in side by side docks, and VertexShader.vert in a separate window.  
  
  
## Credits  
  
Shikomisen (Ayoub) 2024
Media Design School  
GD1P04 - 3D Graphics Programming  
Written for Summative 3  
