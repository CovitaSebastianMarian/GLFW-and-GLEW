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
#define PI 3.14159265359


using namespace std;


double wwindowsize = 1200 , hwindowsize = 700;
double xangle = 0, yangle = 0, zangle = 0;

void reshape(GLFWwindow* window)
{
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

    if (key == GLFW_KEY_UP && (state == GLFW_PRESS || state == GLFW_REPEAT)) xangle++;
    if (key == GLFW_KEY_DOWN && (state == GLFW_PRESS || state == GLFW_REPEAT)) xangle--;

    if (key == GLFW_KEY_LEFT && (state == GLFW_PRESS || state == GLFW_REPEAT)) yangle++;
    if (key == GLFW_KEY_RIGHT && (state == GLFW_PRESS || state == GLFW_REPEAT)) yangle--;
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


    glm::mat4 camera(1.0);
    camera = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));

    Form f;
    Form::FormDetails* fd = new Form::FormDetails[3];
    Object* Face = new Object[3];
    
    int edges = 50;
    int sze = 200;
    int height = 200;


    fd[0] = f.CreateFace1(sze, sze, height, edges, 45);
    fd[1] = f.CreateFace2(sze, sze, height, edges, 45);
    fd[2] = f.CreateLateralFace(sze, sze, sze, sze, height, edges);

    for (int i = 0; i < 3; ++i) {
        Face[i].VectorIndices = fd[i].indices;
        Face[i].VectorPositions = fd[i].positions;
        Face[i].DynamicInitInd(GL_DYNAMIC_DRAW);
        Face[i].VertexAttribpointer(0, 0, 3, 5, GL_FLOAT);
        Face[i].VertexAttribpointer(1, 3, 2, 5, GL_FLOAT);
        Face[i].Shader("Shaders/Basic.shader");
        Face[i].Texture("Textures/brick.jpg", GL_RGB, false);
        Face[i].Ortho(0, wwindowsize, hwindowsize, 0, 100, -100);
        Face[i].Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);
        Face[i].Perspective(glm::radians(45.0), 1, 0.1, 100);
        Face[i].camera = camera;
    }



    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        fd[0] = f.CreateFace1(sze, sze, height, edges, 45);
        fd[1] = f.CreateFace2(sze, sze, height, edges, 45);
        fd[2] = f.CreateLateralFace(sze, sze, sze, sze, height, edges);
        for (int i = 0; i < 3; ++i) {
            Face[i].VectorIndices = fd[i].indices;
            Face[i].VectorPositions = fd[i].positions;
            Face[i].DynamicReloadInd(GL_DYNAMIC_DRAW);
            Face[i].Rotate(glm::radians(xangle), glm::vec3(1, 0, 0), false);
            Face[i].Rotate(glm::radians(yangle), glm::vec3(0, 1, 0), true);
            Face[i].Rotate(glm::radians(zangle), glm::vec3(0, 0, 1), true);
            Face[i].Bind(GL_TRIANGLES, fd[i].indices_size, GL_UNSIGNED_INT, 0);
        }



        


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    exit(0);
    glfwTerminate();
    return 0;
}