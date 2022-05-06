#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include "Shader.h"
#include "error.h"
#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>


#define PI 3.14159265359


using namespace std;


double wwindowsize = 1200 , hwindowsize = 700;
double cnt = 0, yangle = 0;

void reshape(GLFWwindow* window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    wwindowsize = w;
    hwindowsize = h;
    glViewport(0, 0, wwindowsize, hwindowsize);
}
bool ok = false;
void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);

    if (key == GLFW_KEY_UP && (state == GLFW_PRESS || state == GLFW_REPEAT)) cnt++;
    if (key == GLFW_KEY_DOWN && (state == GLFW_PRESS || state == GLFW_REPEAT)) cnt--;

    if (key == GLFW_KEY_LEFT && (state == GLFW_PRESS || state == GLFW_REPEAT)) yangle++;
    if (key == GLFW_KEY_RIGHT && (state == GLFW_PRESS || state == GLFW_REPEAT)) yangle--;
}


int main()
{
    
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window;
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(wwindowsize, hwindowsize, "GLFW and GLEW", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {    
        cout << "\033[3;40;31m[Error: glew =try> move glewinit() after window init]" << endl;
        return -1;
    }

    
    GLCall(glEnable(GL_POINT_SMOOTH));
    GLCall(glHint(GL_POINT_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_LINE_SMOOTH));
    GLCall(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_POLYGON_SMOOTH));
    GLCall(glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));
    

    glfwSetKeyCallback(window, key);


    float pos[360 * 5];
    int cnt = -1;
    float sze = 400;
    for (int i = 0; i < 360; ++i) {
        double delta = i * PI / 180;
        pos[++cnt] = cos(delta) * sze / 2;
        pos[++cnt] = sin(delta) * sze / 2;
        pos[++cnt] = 0;

        pos[++cnt] = cos(delta) / 2 + 0.5;
        pos[++cnt] = sin(delta) / 2 + 0.5;
    }
    Object circle;
    circle.Init(pos, sizeof(pos));
    circle.VertexAttribpointer(0, 0, 3, 5, GL_FLOAT);
    circle.VertexAttribpointer(1, 3, 2, 5, GL_FLOAT);
    circle.Texture("Textures/lateralgrass.jpg", GL_RGB, false);
    circle.Shader("Shaders/Basic.shader");
    circle.Ortho(0, wwindowsize, hwindowsize, 0, -1, 1);
    circle.Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);



    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);



        circle.Bind(GL_POLYGON, 0, 360);




        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}