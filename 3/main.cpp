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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

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

    

    

  
    float positions[] = {
        -0.5, -0.5, 0,       1,0,0,
        0.5, -0.5, 0,        0,1,0,
        0.5, 0.5, 0,         0,0,1,
        -0.5, 0.5, 0,        0.5, 0.5, 0,


        50, 50, 0,      1,0,0,
        50, 150, 0,       0,1,0,
        150, 150, 0,         0,0,1,
        150, 50, 0,       0.5, 0.5, 0,
    };
    


    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW)));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));


    

    glm::mat4 proj = glm::ortho(0.0, (double)wwindowsize, (double)hwindowsize, 0.0, -1.0, 1.0);
    glm::vec4 pos(wwindowsize / 2, hwindowsize / 2, 0, 1);
    glm::vec4 res = pos * proj;
    /*
    * glm::ortho e obligatoriu sa abie numere de double ca parametrii
    * 
    * 
    */




    ShaderProgramSource source = ParseShader("Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    int location = glGetUniformLocation(shader, "MV");
    GLCall(glUseProgram(shader));
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &proj[0][0]));


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


        GLCall(glDrawArrays(GL_POLYGON, 4, 4));



        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }

    glDeleteShader(shader);
    glfwTerminate();
    return 0;
}