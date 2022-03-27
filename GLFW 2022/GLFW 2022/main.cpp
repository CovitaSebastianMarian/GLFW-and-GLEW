#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include <thread>
using namespace std;
#define PI 3.14159265359



int wwindowsize = 1200 , hwindowsize = 700;
void reshape(GLFWwindow* window) {
    glfwGetWindowSize(window, &wwindowsize, &hwindowsize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, wwindowsize, hwindowsize, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, wwindowsize, hwindowsize);
}


bool HoverCircle(double x, double y, double xpos, double ypos, double xsize, double ysize) {
    for (int i = 0; i < 360; ++i) {
        double delta = i * PI / 180;
        if (abs(x - xpos) <= abs(cos(delta) * xsize) && abs(y - ypos) <= abs(sin(delta) * ysize)) {
            return true;
        }
    }
    return false;
}


void mouse(GLFWwindow* windwo, int button, int actions, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_PRESS) {
       
    }
}
float r = 0.8;
void motion(GLFWwindow* window, double x, double y) {
    if (HoverCircle(x, y, wwindowsize / 2, hwindowsize / 2, 400, 100)) {
        r = 0.2;
    }
    else r = 0.8;
}
void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) {
        exit(0);
    }
}


int main(void)
{
 
    if (!glfwInit()) return -1;
    

    GLFWwindow* window = glfwCreateWindow(wwindowsize, hwindowsize, "GLFW and GLEW", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) cout << "ERROR";
    


    GLFWimage image;
    image.pixels = stbi_load("icon.png", &image.width, &image.height, 0, 4);
    glfwSetWindowIcon(window, 1, &image);
    stbi_image_free(image.pixels);


    reshape(window);
    float positions[360*2+1];
    int cnt = -1;
    for (int i = 0; i < 360; ++i) {
        double delta = i * PI / 180;
        positions[++cnt] = cos(delta) * 400;
        positions[++cnt] = sin(delta) * 100;
    }

    
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, (cnt - 1) * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    

    glfwSetMouseButtonCallback(window, mouse);
    glfwSetCursorPosCallback(window, motion);
    glfwSetKeyCallback(window, key);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        reshape(window);



        glPushMatrix();
        glTranslated(wwindowsize / 2, hwindowsize / 2, 0);
        glColor3d(r, 0.3f, 0.02f);
        glDrawArrays(GL_POLYGON, 0, cnt);
        glPopMatrix();






        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }

    glfwTerminate();
    return 0;
}
