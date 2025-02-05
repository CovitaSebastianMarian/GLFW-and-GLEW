#pragma once
#ifndef SEBAPI
#define SEBAPI 1
#define SEBCALL __stdcall
#define SEBALWAYSUSE [[nodiscard]]
#define SEBNORETURN [[noreturn]]
#define SEBNOEXCEPTION noexcept
#define SEBFUNC inline decltype(auto)


#include <GLEW/glew.h>
#include <GLFW/glfw3.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include "stb/stb_image.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>

#endif