#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
//#include "stb_image.h"
#include <thread>
#include "Shader.h"
#include "error.h"
#include "Object.h"
#include "Circle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#define PI 3.14159265359


using namespace std;


double wwindowsize = 1200 , hwindowsize = 700;
double xmouse = 0, ymouse = 0;
int cnt = 0;
double sze = 30;
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

void mouse(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        d.wwindowsize = wwindowsize;
        d.hwindowsize = hwindowsize;
        d.sze = sze;
        d.red = rand() % 2 + 0.5;
        d.green = rand() % 2 + 0.5;
        d.blue = rand() % 2 + 0.5;
        d.xpos = xmouse;
        d.ypos = ymouse;
        d.speed = 5;
        d.direction = rand() % 4 + 1;
        Init(d);
    }

}
void cursor(GLFWwindow* window, double x, double y) {
    xmouse = x;
    ymouse = y;
}

int main()
{
    srand(time(0));
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

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


    if (glewInit() != GLEW_OK) {
        cout << "\033[3;40;31m[Error: glew =try> move glewinit() after window init]" << endl;
        return -1;
    }
    glfwSetKeyCallback(window, key);
    glfwSetCursorPosCallback(window, cursor);
    glfwSetMouseButtonCallback(window, mouse);

    GLCall(glEnable(GL_POINT_SMOOTH));
    GLCall(glHint(GL_POINT_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_LINE_SMOOTH));
    GLCall(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_POLYGON_SMOOTH));
    GLCall(glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));





    while (!glfwWindowShouldClose(window))
    {
        //glClearColor(0.07f, 0.13f, 0.17f, 1);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);


       
        Draw();




        glfwSwapBuffers(window);
        glfwPollEvents();
        //this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }
    exit(0);
    glfwTerminate();
    return 0;
}