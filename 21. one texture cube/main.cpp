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
double xangle = 0, yangle = 0;

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

    if (key == GLFW_KEY_UP && (state == GLFW_PRESS || state == GLFW_REPEAT)) xangle++;
    if (key == GLFW_KEY_DOWN && (state == GLFW_PRESS || state == GLFW_REPEAT)) xangle--;

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

    float sze = 400;
    float pos[] = {
        -sze / 2, sze / 2, sze / 2,    0.25, 0.75,//0
        -sze / 2, sze / 2, -sze / 2,   0.25, 1,//1
        sze / 2, sze / 2, -sze / 2,    0.5, 1,//2
        sze / 2, sze / 2, sze / 2,     0.5, 0.75,//3
        sze / 2, sze / 2, -sze / 2,    0.75, 0.75,//4
        sze / 2, -sze / 2, -sze / 2,   0.75, 0.5,//5
        sze / 2, -sze / 2, sze / 2,    0.5, 0.5,//6
        sze / 2, -sze / 2, -sze / 2,   0.5, 0.25,//7
        sze / 2, sze / 2, -sze / 2,    0.5, 0,//8
        -sze / 2, sze / 2, -sze / 2,   0.25, 0,//9    
        -sze / 2, -sze / 2, -sze / 2,  0.25, 0.25,//10
        -sze / 2, -sze / 2, sze / 2,   0.25, 0.5,//11
        -sze / 2, -sze / 2, -sze / 2,  0, 0.5,//12
        -sze / 2, sze / 2, -sze / 2,   0, 0.75,//13
    };
    unsigned int indices[] = {
        0,1,2,3,
        0,3,6,11,
        3,4,5,6,
        0,11,12,13,
        6,7,10,11,
        7,8,9,10,
    };


    Object obj;
    obj.Init(pos, sizeof(pos), indices, sizeof(indices));
    obj.VertexAttribpointer(0, 0, 3, 5, GL_FLOAT);
    obj.VertexAttribpointer(1, 3, 2, 5, GL_FLOAT);
    obj.Shader("Shaders/Basic.shader");
    obj.Texture("Textures/cube.jpg", GL_RGB, true);
    obj.Ortho(0, wwindowsize, hwindowsize, 0, 100, -100);
    obj.Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);
    obj.Perspective(glm::radians(45.0), 1, 0.1, 100);
    obj.LookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, -0.1), glm::vec3(0, 1, 0));



    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);



        obj.Rotate(glm::radians(xangle), glm::vec3(1, 0, 0), false);
        obj.Rotate(glm::radians(yangle), glm::vec3(0, 1, 0), true);
        obj.Bind(GL_QUADS, 24, GL_UNSIGNED_INT, 0);




        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
