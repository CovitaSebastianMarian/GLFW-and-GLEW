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
int cnt = 0;
double sze = 200;
double xpos1 = 0, ypos1 = 0, xpos2 = wwindowsize, ypos2 = hwindowsize;

void reshape(GLFWwindow* window)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    wwindowsize = w;
    hwindowsize = h;
    glViewport(0, 0, wwindowsize, hwindowsize);
    
}

bool OK(double xpos1, double ypos1, double xpos2, double ypos2) {
    if ((sze / 2 + sze / 2) <= sqrt(pow(xpos1 - xpos2, 2) + pow(ypos1 - ypos2, 2))) return true;
    return false;
}
void key(GLFWwindow* window, int key, int scanmode, int state, int mods)
{
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);

    if (key == GLFW_KEY_W && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        ypos1 -= 5;
        if (!OK(xpos1, ypos1, xpos2, ypos2)) {
            ypos1 += 5;
            ypos1 -= sqrt(pow(xpos1 - xpos2, 2) + pow(ypos1 - ypos2, 2)) - (sze / 2 + sze / 2);
        }
    }
    if (key == GLFW_KEY_S && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        ypos1 += 5;
        if (!OK(xpos1, ypos1, xpos2, ypos2)) {
            ypos1 -= 5;
            ypos1 += sqrt(pow(xpos1 - xpos2, 2) + pow(ypos1 - ypos2, 2)) - (sze / 2 + sze / 2);
        }
    }
    if (key == GLFW_KEY_A && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        xpos1 -= 5;
        if (!OK(xpos1, ypos1, xpos2, ypos2)) {
            xpos1 += 5;
            xpos1 -= sqrt(pow(xpos1 - xpos2, 2) + pow(ypos1 - ypos2, 2)) - (sze / 2 + sze / 2);
        }
    }
    if (key == GLFW_KEY_D && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        xpos1 += 5;
        if (!OK(xpos1, ypos1, xpos2, ypos2)) {
            xpos1 -= 5;
            xpos1 += sqrt(pow(xpos1 - xpos2, 2) + pow(ypos1 - ypos2, 2)) - (sze / 2 + sze / 2);
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (key == GLFW_KEY_UP && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        ypos2 -= 5;
        if (!OK(xpos1, ypos1, xpos2, ypos2)) {
            ypos2 += 5;
            ypos2 -= sqrt(pow(xpos1 - xpos2, 2) + pow(ypos1 - ypos2, 2)) - (sze / 2 + sze / 2);
        }
    }
    if (key == GLFW_KEY_DOWN && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        ypos2 += 5;
        if (!OK(xpos1, ypos1, xpos2, ypos2)) {
            ypos2 -= 5;
            ypos2 += sqrt(pow(xpos1 - xpos2, 2) + pow(ypos1 - ypos2, 2)) - (sze / 2 + sze / 2);
        }
    }
    if (key == GLFW_KEY_LEFT && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        xpos2 -= 5;
        if (!OK(xpos1, ypos1, xpos2, ypos2)) {
            xpos2 += 5;
            xpos2 -= sqrt(pow(xpos1 - xpos2, 2) + pow(ypos1 - ypos2, 2)) - (sze / 2 + sze / 2);
        }
    }
    if (key == GLFW_KEY_RIGHT && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        xpos2 += 5;
        if (!OK(xpos1, ypos1, xpos2, ypos2)) {
            xpos2 -= 5;
            xpos2 += sqrt(pow(xpos1 - xpos2, 2) + pow(ypos1 - ypos2, 2)) - (sze / 2 + sze / 2);
        }
    }
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



    float pos[360 * 6];
    int cnt = -1;
    for (int i = 0; i < 360; ++i) {
        double delta = i * PI / 180;
        pos[++cnt] = cos(delta) * sze / 2; // x
        pos[++cnt] = sin(delta) * sze / 2; // y
        pos[++cnt] = 0;

        pos[++cnt] = 0.2;
        pos[++cnt] = 0.6;
        pos[++cnt] = 0.4;
    }

    Object circle[2];
    circle[0].Init(pos, sizeof(pos));
    circle[0].VertexAttribpointer(0, 0, 3, 6, GL_FLOAT);
    circle[0].VertexAttribpointer(1, 3, 3, 6, GL_FLOAT);
    circle[0].Shader("Shaders/Basic.shader");
    circle[0].Ortho(0, wwindowsize, hwindowsize, 0, -100, 100);
    circle[0].Translate(glm::vec3(xpos1, ypos1, 0), false);


    circle[1].Init(pos, sizeof(pos));
    circle[1].VertexAttribpointer(0, 0, 3, 6, GL_FLOAT);
    circle[1].VertexAttribpointer(1, 3, 3, 6, GL_FLOAT);
    circle[1].Shader("Shaders/Basic.shader");
    circle[1].Ortho(0, wwindowsize, hwindowsize, 0, -100, 100);
    circle[1].Translate(glm::vec3(xpos2, ypos2, 0), false);



    while (!glfwWindowShouldClose(window))
    {
        //glClearColor(0.07f, 0.13f, 0.17f, 1);
        glClearColor(0, 0, 0, 0.5);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);

        circle[0].Translate(glm::vec3(xpos1, ypos1, 0), false);
        circle[1].Translate(glm::vec3(xpos2, ypos2, 0), false);
        circle[0].Bind(GL_POLYGON, 0, (cnt + 1) / 6);
        circle[1].Bind(GL_POLYGON, 0, (cnt + 1) / 6);



        glfwSwapBuffers(window);
        glfwPollEvents();
        //this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }
    exit(0);
    glfwTerminate();
    return 0;
}