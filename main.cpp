#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "stb_image.h"
#include <thread>
#include "shader.h"
#include "error.h"
#define PI 3.14159265359


using namespace std;


int wwindowsize = 1200 , hwindowsize = 700;
void reshape(GLFWwindow* window) {
    glfwGetWindowSize(window, &wwindowsize, &hwindowsize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, wwindowsize, hwindowsize, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, wwindowsize, hwindowsize);
}

static void key(GLFWwindow* window, int key, int scanmode, int state, int mods)
{
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);
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

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) cout << "\033[3;40;31m[Error: glew =try> glewinit() after window creatons]" << endl;

    glfwSetKeyCallback(window, key);


    

    //reshape(window);
    float positions[] = {
        -0.5, -0.5,
        0.5, -0.5,
        0.5, 0.5,
        -0.5, 0.5
    };
    
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
  
    ShaderProgramSource source = ParseShader("Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);


    int location =glGetUniformLocation(shader, "u_color");
    glUniform4f(location, 1, 0.2, 0.5, 1);

    float r = 1, cnt = 0.05;


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //reshape(window);

        glUniform4f(location, r, 0.2, 0.5, 1);
        glDrawArrays(GL_QUADS, 0, 4);
        if (r > 1) {
            cnt = -0.01;
        }
        else if (r < 0) {
            cnt = 0.01;
        }
        r += cnt;

        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }

    glfwTerminate();
    return 0;
}