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
double xpos = 0, ypos = 0;
float cubsize = 0.5;

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
    
}

int main()
{
    
    if (!glfwInit()) {
        return -1;
    }


    GLFWwindow* window;
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(wwindowsize, hwindowsize, "GLFW and GLEW", NULL, NULL);
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



    float cubpositions[] = {
        //positionws        color      texture coord   
        -cubsize, -cubsize, cubsize,    1,0,0,   1.0,0.0,   0,0,2*cubsize,
        -cubsize, cubsize, cubsize,     1,1,0,   1.0,1.0,   0,0,2*cubsize,
        cubsize, cubsize, cubsize,      0,0,1,   0.0,1.0,   0,0,2*cubsize,
        cubsize, -cubsize, cubsize,     0,1,0,   0.0,0.0,   0,0,2*cubsize,
    
        cubsize, -cubsize, -cubsize,    1,0,0,   1.0,0.0,   2*cubsize,0,0,
        cubsize, cubsize, -cubsize,     1,1,0,   1.0,1.0,   2*cubsize,0,0,
        cubsize, cubsize, cubsize,      0,0,1,   0.0,1.0,   2*cubsize,0,0,
        cubsize, -cubsize, cubsize,     0,1,0,   0.0,0.0,   2*cubsize,0,0,

        -cubsize, -cubsize, -cubsize,    1,0,0,   0.0,0.0,   0,0,-2*cubsize,
        cubsize, -cubsize, -cubsize,     0,1,0,   1.0,0.0,   0,0,-2*cubsize,
        cubsize, cubsize, -cubsize,      0,0,1,   1.0,1.0,   0,0,-2*cubsize,
        -cubsize, cubsize, -cubsize,     1,1,0,   0.0,1.0,   0,0,-2*cubsize,

        -cubsize, -cubsize, -cubsize,    1,0,0,   0.0,0.0,   -2*cubsize, 0, 0,
        -cubsize, -cubsize, cubsize,     0,1,0,   1.0,0.0,   -2*cubsize, 0, 0,
        -cubsize, cubsize, cubsize,      0,0,1,   1.0,1.0,   -2*cubsize, 0, 0,
        -cubsize, cubsize, -cubsize,     1,1,0,   0.0,1.0,   -2*cubsize, 0, 0,

        -cubsize, cubsize, -cubsize,     1,0,0,   1.0,0.0,   0,2*cubsize,0,
        cubsize, cubsize, -cubsize,      1,1,0,   1.0,1.0,   0,2*cubsize,0,
        cubsize, cubsize, cubsize,       0,0,1,   0.0,1.0,   0,2*cubsize,0,
        -cubsize, cubsize, cubsize,      0,1,0,   0.0,0.0,   0,2*cubsize,0,

        -cubsize, -cubsize, -cubsize,     1,0,0,   0.0,0.0,   0,-2*cubsize, 0,
        -cubsize, -cubsize, cubsize,      0,1,0,   1.0,0.0,   0,-2*cubsize, 0,
        cubsize, -cubsize, cubsize,       0,0,1,   1.0,1.0,   0,-2*cubsize, 0,
        cubsize, -cubsize, -cubsize,      1,1,0,   0.0,1.0,   0,-2*cubsize, 0,
    };
    Object cub;
    cub.Init(cubpositions, sizeof(cubpositions));
    cub.VertexAttribpointer(0, 0, 3, 11, GL_FLOAT);
    cub.VertexAttribpointer(1, 3, 3, 11, GL_FLOAT);
    cub.VertexAttribpointer(2, 6, 2, 11, GL_FLOAT);
    cub.VertexAttribpointer(3, 8, 3, 11, GL_FLOAT);
    cub.Shader("Shaders/Basic.shader");
    cub.Texture("Textures/image.jpg", GL_RGB, false);
    cub.Perspective(glm::radians(45.0), wwindowsize/ hwindowsize, 0.1, 100);
    cub.Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    
    glm::vec4 light(1.0, 1.0, 1.0, 1.0);
    glm::vec3 lightpos(0, 0, 3);

    int llight = glGetUniformLocation(cub.shader, "lightcolor");
    glUniform3f(llight, light.x, light.y, light.z);

    int lpl = glGetUniformLocation(cub.shader, "lightPos");
    glUniform3f(lpl, lightpos.x, lightpos.y, lightpos.z);


    while (!glfwWindowShouldClose(window))
    {
        //glClearColor(0.07f, 0.13f, 0.17f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);

        cub.Rotate(glm::radians(0.5), glm::vec3(1, 1, 1), false, true);
        cub.Bind(GL_QUADS, 0, 24);




        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }
    exit(0);
    glfwTerminate();
    return 0;
}