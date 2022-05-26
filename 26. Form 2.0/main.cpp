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
#include "Ecosistem.h"
#define PI 3.14159265359


using namespace std;


double wwindowsize = 1200 , hwindowsize = 700;



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

void key(GLFWwindow* window, int key, int scanmode, int state, int mods)
{
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);

}

void mouse(GLFWwindow* window, int button, int action, int mods) {

}
void cursor(GLFWwindow* window, double x, double y) {

}

void makePixel(GLFWwindow*window, int x, int y, int r, int g, int b, GLbyte* pixels)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    if (0 <= x && x < w && 0 <= y && y < h) {
        int position = (x + y * w) * 3;
        pixels[position] = r;
        pixels[position + 1] = g;
        pixels[position + 2] = b;
    }
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


    float sze = 1;
    float pos[] = {
        -sze, -sze, 0,   0,0.4,1,      0, 0,
        sze, -sze, 0,    1,0.5,0,      1, 0,
        sze, sze, 0,     0.3, 1, 0,    1, 1,
        -sze, sze, 0,    0, 0.7, 0.8,  0, 1,
    };
    Object obj;
    obj.Init(pos, sizeof(pos));
    obj.VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
    obj.VertexAttribpointer(1, 3, 3, 8, GL_FLOAT);
    obj.VertexAttribpointer(2, 6, 2, 8, GL_FLOAT);
    obj.Shader("Shaders/Basic.shader");
    obj.Texture("Textures/image.jpg", GL_RGB, true);


    int w = 100, h = 100;
    GLbyte* pb = new GLbyte[w * h * 3];

    makePixel(window, 200, 200, 0, 0, 0, pb);


    while (!glfwWindowShouldClose(window))
    {
        //glClearColor(0.07f, 0.13f, 0.17f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        
        
        obj.Bind(GL_QUADS, 0, 4);

        glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, pb);




        


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    exit(0);
    glfwTerminate();
    return 0;
}