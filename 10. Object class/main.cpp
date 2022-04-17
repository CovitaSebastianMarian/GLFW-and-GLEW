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
double xpos = 100, ypos = 100;
float p = 0;
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
    if (key == GLFW_KEY_W && state == GLFW_PRESS) p+=0.5;
    if (key == GLFW_KEY_S && state == GLFW_PRESS) p-=0.5;
}



int main()
{
    
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(wwindowsize, hwindowsize, "GLFW and GLEW", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK) {    
        cout << "\033[3;40;31m[Error: glew =try> move glewinit() after window init]" << endl;
        return -1;
    }
    glfwSetKeyCallback(window, key);
    
    GLCall(glEnable(GL_POINT_SMOOTH));
    GLCall(glHint(GL_POINT_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_LINE_SMOOTH));
    GLCall(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_POLYGON_SMOOTH));
    GLCall(glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));



    float sze = 100;
    float positions[] = {
        //position          color      texture coord
        -sze, -sze, 0,      1,0,0,     0,1,
        sze, -sze, 0,       0,1,0,     1,1,
        sze,sze,0,          0,0,1,     1,0,
        -sze, sze, 0,       1,1,0,     0,0,
    };
    Object obj;
    obj.Init(positions, sizeof(positions));
    obj.VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
    obj.VertexAttribpointer(1, 3, 3, 8, GL_FLOAT);
    obj.VertexAttribpointer(2, 6, 2, 8, GL_FLOAT);
    obj.Shader("Basic.shader");
    obj.Texture("image.jpg", true);
    obj.Perspective(glm::radians(45.0), 1.0, 0.1, 100.0);
    obj.Camera(glm::vec3(0, 0.5, 0.5), glm::vec3(0, 0, -0.5), glm::vec3(0, 1, 0));
    obj.Ortho(0.0, wwindowsize, hwindowsize, 0.0, 100.0, -100.0);
    obj.Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);
    obj.Rotate(glm::radians(45.0), glm::vec3(0, 0, 1), false);
    obj.Scale(glm::vec3(1,1,1), false);
    
    Object brick;
    brick.Init(positions, sizeof(positions));
    brick.VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
    brick.VertexAttribpointer(1, 3, 3, 8, GL_FLOAT);
    brick.VertexAttribpointer(2, 6, 2, 8, GL_FLOAT);
    brick.Shader("Basic.shader");
    brick.Texture("brick.jpg", true);
    brick.Perspective(glm::radians(45.0), 1.0, 0.1, 100.0);
    brick.Camera(glm::vec3(0, 0.5, 0.5), glm::vec3(0, 0, -0.5), glm::vec3(0, 1, 0));
    brick.Ortho(0.0, wwindowsize, hwindowsize, 0.0, 100.0, -100.0);
    brick.Translate(glm::vec3(wwindowsize /2, hwindowsize /2, 0.1), false);
    brick.Rotate(glm::radians(45.0), glm::vec3(0, 0, 1), false);
    brick.Scale(glm::vec3(0.5, 0.5, 0.5), false);




    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);

        obj.Bind();
        brick.Bind();


        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }

    glfwTerminate();
    return 0;
}
