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
double xangle = 0 , yangle = 90;

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
    if (key == GLFW_KEY_W && (state == GLFW_PRESS || state == GLFW_REPEAT)) xangle++;
    if (key == GLFW_KEY_S && (state == GLFW_PRESS || state == GLFW_REPEAT)) xangle--;
    if (key == GLFW_KEY_D && (state == GLFW_PRESS || state == GLFW_REPEAT)) yangle++;
    if (key == GLFW_KEY_A && (state == GLFW_PRESS || state == GLFW_REPEAT)) yangle--;
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

    
    float positions[] = {
        //positions       color    texture
        -100.0, -100.0, 100.0,    1,0,0,   1.0,0.0,
        -100.0, 100.0, 100.0,     1,1,0,   1.0,1.0,
        100.0, 100.0, 100.0,      0,0,1,   0.0,1.0,
        100.0, -100.0, 100.0,     0,1,0,   0.0,0.0,

        100.0, -100.0, -100.0,    1,0,0,   1.0,0.0,
        100.0, 100.0, -100.0,     1,1,0,   1.0,1.0,
        100.0, 100.0, 100.0,      0,0,1,   0.0,1.0,
        100.0, -100.0, 100.0,     0,1,0,   0.0,0.0,

        -100.0, -100.0, -100.0,    1,0,0,   0.0,0.0,
        100.0, -100.0, -100.0,     0,1,0,   1.0,0.0,
        100.0, 100.0, -100.0,      0,0,1,   1.0,1.0,
        -100.0, 100.0, -100.0,     1,1,0,   0.0,1.0,

        -100.0, -100.0, -100.0,    1,0,0,   0.0,0.0,
        -100.0, -100.0, 100.0,     0,1,0,   1.0,0.0,
        -100.0, 100.0, 100.0,      0,0,1,   1.0,1.0,
        -100.0, 100.0, -100.0,     1,1,0,   0.0,1.0,

        -100.0, 100.0, -100.0,     1,0,0,   1.0,0.0,
        100.0, 100.0, -100.0,      1,1,0,   1.0,1.0,
        100.0, 100.0, 100.0,       0,0,1,   0.0,1.0,
        -100.0, 100.0, 100.0,      0,1,0,   0.0,0.0,

        -100.0, -100.0, -100.0,     1,0,0,   0.0,0.0,
        -100.0, -100.0, 100.0,      0,1,0,   1.0,0.0,
        100.0, -100.0, 100.0,       0,0,1,   1.0,1.0,
        100.0, -100.0, -100.0,      1,1,0,   0.0,1.0,
    };

    glm::mat4 camera = glm::lookAt(glm::vec3(0, 0, 2.5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));


    Object lateralcube, upcube, downcube;
    lateralcube.Init(positions, sizeof(positions));
    lateralcube.VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
    lateralcube.VertexAttribpointer(1, 6, 2, 8, GL_FLOAT);
    lateralcube.Shader("Basic.shader");
    lateralcube.Texture("lateralgrass.jpg", GL_RGB, false);
    lateralcube.Ortho(0, wwindowsize, hwindowsize, 0, 100, -100);
    lateralcube.Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);
    lateralcube.Perspective(glm::radians(45.0), 1, 0.1, 100);
    //lateralcube.Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    upcube.Init(positions, sizeof(positions));
    upcube.VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
    upcube.VertexAttribpointer(1, 6, 2, 8, GL_FLOAT);
    upcube.Shader("Basic.shader");
    upcube.Texture("upgrass.jpg", GL_RGB, false);
    upcube.Ortho(0, wwindowsize, hwindowsize, 0, 100, -100);
    upcube.Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);
    upcube.Perspective(glm::radians(45.0), 1, 0.1, 100);
   // upcube.Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    downcube.Init(positions, sizeof(positions));
    downcube.VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
    downcube.VertexAttribpointer(1, 6, 2, 8, GL_FLOAT);
    downcube.Shader("Basic.shader");
    downcube.Texture("dirt.jpg", GL_RGB, true);
    downcube.Ortho(0, wwindowsize, hwindowsize, 0, 100, -100);
    downcube.Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);
    downcube.Perspective(glm::radians(45.0), 1, 0.1, 100);
    //downcube.Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    lateralcube.camera = camera;
    upcube.camera = camera;
    downcube.camera = camera;
    

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);


        lateralcube.Rotate(glm::radians(xangle), glm::vec3(1, 0, 0), false, false);
        lateralcube.Rotate(glm::radians(yangle), glm::vec3(0, 1, 0), true, false);
        upcube.Rotate(glm::radians(xangle), glm::vec3(1, 0, 0), false, false);
        upcube.Rotate(glm::radians(yangle), glm::vec3(0, 1, 0), true, false);
        downcube.Rotate(glm::radians(xangle), glm::vec3(1, 0, 0), false, false);
        downcube.Rotate(glm::radians(yangle), glm::vec3(0, 1, 0), true, false);
        lateralcube.Bind(GL_QUADS, 0, 16);
        upcube.Bind(GL_QUADS, 20, 4);
        downcube.Bind(GL_QUADS, 16, 4);
       
        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }


    glfwTerminate();
    return 0;
}