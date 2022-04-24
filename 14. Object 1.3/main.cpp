#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
//#include "stb_image.h"
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
double xmouse = 0, ymouse = 0;

void reshape(GLFWwindow* window)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    wwindowsize = w;
    hwindowsize = h;
    glViewport(0, 0, wwindowsize, hwindowsize);
    
}

void key(GLFWwindow* window, int key, int scanmode, int state, int mods)
{
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);
}
void cursor(GLFWwindow* window, double x, double y) {
    xmouse = x;
    ymouse = y;
}

int main()
{
    
    if (!glfwInit()) {
        return -1;
    }


    GLFWwindow* window;
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(wwindowsize, hwindowsize, "GLFW and GLEW", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    

    if (glewInit() != GLEW_OK) {    
        cout << "\033[3;40;31m[Error: glew =try> move glewinit() after window init]" << endl;
        return -1;
    }
    glfwSetKeyCallback(window, key);
    glfwSetCursorPosCallback(window, cursor);
    
    GLCall(glEnable(GL_POINT_SMOOTH));
    GLCall(glHint(GL_POINT_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_LINE_SMOOTH));
    GLCall(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_POLYGON_SMOOTH));
    GLCall(glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));

    float sze = 0.5;
    float positions[] = {
        -sze, -sze, 0,  1,0,0,
        sze, -sze, 0,   1,0,0,
        sze, sze, 0,    1,0,0,
        -sze, sze, 0,   1,0,0,
    };
    unsigned int indices[] = {
        0,1,2,
        0,2,3,
    };
    Object patrat;
    patrat.Init(positions, sizeof(positions), indices, sizeof(indices));
    patrat.VertexAttribpointer(0, 0, 3, 6, GL_FLOAT);
    patrat.VertexAttribpointer(1, 3, 3, 6, GL_FLOAT);
    patrat.Shader("Shaders/Basic.shader");

    while (!glfwWindowShouldClose(window))
    {
        //glClearColor(0.07f, 0.13f, 0.17f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);


        patrat.Bind(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        




        glfwSwapBuffers(window);
        glfwPollEvents();
        //this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }
    exit(0);
    glfwTerminate();
    return 0;
}