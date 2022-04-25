#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>

#define INCLUDE_ERROR_LIBRARY 0;
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define GLCallReturn(x) GLClearError();\
    x;\

using namespace std;

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        cout << "\033[3;41;30m[OPENGL ERROR]\033[3;40;31m(error code:" << error << ") " << function << " " << file << ":" << line << "\033[0m";
        return false;
    }
    return true;
}