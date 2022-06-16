#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <thread>
#include "Shader.h"
#include "error.h"
#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <vector>
#include "Form.h"
//#include "ObjFile.h"
#define PI 3.14159265359


using namespace std;


double wwindowsize = 1200, hwindowsize = 700;


void reshape(GLFWwindow* window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    if (w != wwindowsize || h != hwindowsize) {
        wwindowsize = w;
        hwindowsize = h;
        glViewport(0, 0, wwindowsize, hwindowsize);
    }
}

void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);

}

void mouse(GLFWwindow* window, int button, int action, int mods) {

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
    glfwSetMouseButtonCallback(window, mouse);
    glfwSetWindowRefreshCallback(window, reshape);

    GLCall(glEnable(GL_POINT_SMOOTH));
    GLCall(glHint(GL_POINT_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_LINE_SMOOTH));
    GLCall(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_POLYGON_SMOOTH));
    GLCall(glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));

    
    ObjFile obj;
    //obj.OpenFile("Models/cube.mtl");
    obj.Location = "Textures/";
    obj.LoadModel("Models/cube.obj", "Models/cube.mtl");
    Object* models = new Object[obj.KitVector.size()];

    for (size_t i = 0; i < obj.KitVector.size(); ++i) {
        models[i].Vertices = obj.KitVector[i].Vertices;
        models[i].Indices = obj.KitVector[i].Indices;
        models[i].Config(GL_DYNAMIC_DRAW);
        models[i].VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
        models[i].VertexAttribpointer(1, 3, 2, 8, GL_FLOAT);
        models[i].VertexAttribpointer(2, 5, 3, 8, GL_FLOAT);
        models[i].Shader("Shaders/Basic.shader");
        models[i].Texture(obj.KitVector[i].texture, GL_RGB, false);
        models[i].Ortho(0, wwindowsize, 0, hwindowsize, 100, -100);
        models[i].Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);
        models[i].Perspective(glm::radians(45.0), 1, 0.1, 100);
        models[i].LookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
        models[i].Scale(glm::vec3(100, 100, 100), false);
    }
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for (size_t i = 0; i < obj.KitVector.size(); ++i) {
            models[i].Bind(GL_TRIANGLES);
            models[i].Rotate(glm::radians(1.0), glm::vec3(0, 1, 0), true);
        }
        
        



        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    exit(0);
    glfwTerminate();
    return 0;
}