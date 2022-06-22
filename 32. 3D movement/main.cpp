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
#include "Form.h"


#define PI 3.14159265359

using namespace std;


double wwindowsize = 1200 , hwindowsize = 700;
int cnt = 90;
double xpos = 0, zpos = 0;
double angle = cnt * PI / 180;
double xlat, ylat;
double height = 10;
double yaxes = height - 1;

void reshape(GLFWwindow* window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    wwindowsize = w;
    hwindowsize = h;
    glViewport(0, 0, wwindowsize, hwindowsize);
}
void cursor(GLFWwindow* window, double x, double y) {
    double xval = x - (wwindowsize / 2);
    double yval = y - (hwindowsize / 2);
    if (xval > 0) {
        cnt += 2;
        angle = cnt * PI / 180;
    }
    else if (xval < 0) {
        cnt -= 2;
        angle = cnt * PI / 180;
    }

    if (yval > 0) {
        //yaxes -= 0.1;
    }
    else if (yval < 0) {
        //yaxes += 0.1;
    }

    glfwSetCursorPos(window, wwindowsize / 2, hwindowsize / 2);
}


void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);

    if (key == GLFW_KEY_LEFT && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        cnt-=2;
        angle = cnt * PI / 180;
    }
    if (key == GLFW_KEY_RIGHT && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        cnt+=2;
        angle = cnt * PI / 180;
    }
    if (key == GLFW_KEY_UP && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        yaxes += 0.1;
    }
    if (key == GLFW_KEY_DOWN && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        yaxes -= 0.1;
    }

    if (key == GLFW_KEY_W && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        zpos += sin(angle);
        xpos += cos(angle);
    }
    if (key == GLFW_KEY_S && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        zpos -= sin(angle);
        xpos -= cos(angle);
    }
}


int main() {

    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window;
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
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
    GLCall(glEnable(GL_MULTISAMPLE));
    GLCall(glEnable(GL_STENCIL_TEST));


    glfwSetKeyCallback(window, key);
    glfwSetCursorPosCallback(window, cursor);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetCursorPos(window, wwindowsize / 2, hwindowsize / 2);


    float plan_size = 100000;

    Object* plan = new Object;
    plan->VectorPositions.push_back(-plan_size / 2);
    plan->VectorPositions.push_back(0);
    plan->VectorPositions.push_back(-plan_size / 2);

    plan->VectorPositions.push_back(0);
    plan->VectorPositions.push_back(0);

    plan->VectorPositions.push_back(plan_size / 2);
    plan->VectorPositions.push_back(0);
    plan->VectorPositions.push_back(-plan_size / 2);

    plan->VectorPositions.push_back(1);
    plan->VectorPositions.push_back(0);

    plan->VectorPositions.push_back(plan_size / 2);
    plan->VectorPositions.push_back(0);
    plan->VectorPositions.push_back(plan_size / 2);

    plan->VectorPositions.push_back(1);
    plan->VectorPositions.push_back(1);

    plan->VectorPositions.push_back(-plan_size / 2);
    plan->VectorPositions.push_back(0);
    plan->VectorPositions.push_back(plan_size / 2);

    plan->VectorPositions.push_back(0);
    plan->VectorPositions.push_back(1);
    plan->DynamicInit(GL_STATIC_DRAW);
    plan->VertexAttribpointer(0, 0, 3, 5, GL_FLOAT);
    plan->VertexAttribpointer(1, 3, 2, 5, GL_FLOAT);
    plan->Shader("Shaders/Basic.shader");
    plan->Texture("Textures/lateralgrass.jpg", GL_RGB, false);
    plan->Ortho(0, wwindowsize, 0, hwindowsize, 100, -100);
    plan->Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);
    plan->Perspective(glm::radians(45.0), 1 , 0.1, 100);
    plan->LookAt(glm::vec3(0, 1, 0), glm::vec3(0, 0, -5), glm::vec3(0, 1, 0));
    plan->LookAt(glm::vec3(xpos, height, zpos), glm::vec3(cos(angle) + xpos, yaxes, sin(angle) + zpos), glm::vec3(0, 1, 0));

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);


        plan->LookAt(glm::vec3(xpos, height, zpos), glm::vec3(cos(angle) + xpos, yaxes, sin(angle) + zpos), glm::vec3(0, 1, 0));
        plan->Bind(GL_QUADS, 0, 4);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}