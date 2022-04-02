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
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#define PI 3.14159265359


using namespace std;


int wwindowsize = 1200 , hwindowsize = 700;
double xpos = 100, ypos = 100;

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

    if (glewInit() != GLEW_OK) cout << "\033[3;40;31m[Error: glew =try> move glewinit() after window init]" << endl;

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



    glm::mat4 tr(1.0);
    tr = glm::translate(tr, glm::vec3(200.0, 400.0, 0));


    ShaderProgramSource source = ParseShader("Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));
    int location = glGetUniformLocation(shader, "MV");
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &proj[0][0]));


    int ltranslation = glGetUniformLocation(shader, "translate");
    glUniformMatrix4fv(ltranslation, 1, GL_FALSE, glm::value_ptr(tr));
    /*
    * Pentru translatie avem nevoie de facem asta obligatoriu
    * glm::mat4 tr(1.0);
    * tr = glm::translate(tr, glm::vec3(200.0, 400.0, 0));
    * ########################################################
    * SEPARAT** NU FACEM ASTA "glm::mat4 tr = glm::translate(tr, glm::vec3(200.0, 400.0, 0));" - NU
    * ########################################################
    * in vec3 sunt coordonatele de translate
    * 
    * 
    * e obligatoriu sa initializam la glUniformMatrix4fv ultimul termen cu glm::value_ptr(tr);
    * 
    * value_ptr e in biblioteca #include <glm/gtc/type_ptr.hpp>
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