#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

#define INCLUDE_SHADER_LIBRARY 0;
struct ShaderProgramSource
{
	string VertexSource;
	string FragmentSource;
};

static ShaderProgramSource ParseShader(const char* &file)
{
	ifstream fin(file);

	if (!fin.is_open()) {
		cout << "\033[3;40;31m[Error::Shader: " << file << " we can't open it]\033[0m" << endl;
	}

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	stringstream ss[2];
	string line;
	ShaderType type = ShaderType::NONE;
	while (getline(fin, line))
	{
		if (line.find("#shader") != string::npos)
		{
			if (line.find("vertex") != string::npos) type = ShaderType::VERTEX;
			else if (line.find("fragment") != string::npos) type = ShaderType::FRAGMENT;
		}
		else 
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}


static unsigned int CompileShader(unsigned int type, const string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) 
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		cout << "Faliled to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << message << endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}


static int CreateShader(const string& vertexShader, const string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}