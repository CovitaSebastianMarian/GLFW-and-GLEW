#pragma once


#define ENGINE_ERROR 0;
#define ASSERT(x) if (!(x)) __debugbreak();

#define GL_LOG_ERROR(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define LOG_ERROR GLLogCall(__FUNCTION__, __FILE__, __LINE__)


static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line)
{

    while (GLenum error = glGetError())
    {
        std::cerr << "\033[3;41;30m[ERROR]: \033[3;42;30mfile: " << file << " function: " << function << " line: " << line << "\033[0m\n";
        std::cerr << "\033[3;41;30m[OPENGL ERROR]\033[3;40;31m(error code:" << error << ") \033[0m\n";
        return false;
    }
    return true;
}