#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <Windows.h>
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
void reshape(GLFWwindow* window)
{
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

    if (glewInit() != GLEW_OK) cout << "\033[3;40;31m[Error: glew =try> move glewinit() after window creatons]" << endl;

    glfwSetKeyCallback(window, key);
    //reshape(window);



    ShaderProgramSource source = ParseShader("Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);


    float positions[] = {
        -0.5, -0.5, 0,       1,0,0,
        0.5, -0.5, 0,        0,1,0,
        0.5, 0.5, 0,         0,0,1,
        -0.5, 0.5, 0,        0.5, 0.5, 0,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    /*
    * glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    * 
    * index = id
    * size = cate valori sa ia de la pointer mergand spre dreapta
    * type = GL_FLOAT sau GL_INT depinde de ce tipuri de date contine matricea de pozitii
    * normalized = mai mereu e GL_FALSE
    * stride = e de unde sa inceapa sa ia valori data viitoare / peste cate valori sa sara
    * pointer = ne zice de unde incepe la inceput sa ia valori
    * 
    */

    

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        reshape(window);



        GLCall(glDrawArrays(GL_POLYGON, 0, 4));




        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }

    glDeleteShader(shader);
    glfwTerminate();
    return 0;
}
