#pragma once

// opt
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// std
#include <iostream>
#include <sstream>
#include <fstream>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragPath);
    operator GLuint() { return m_shaderProgram; }

private:
    int m_shaderProgram;
};