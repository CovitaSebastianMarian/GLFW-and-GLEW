#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include "stb_image.h"
#include <thread>
#include "Shader.h"
#include "error.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>


#define PI 3.14159265359


using namespace std;


float wwindowsize = 1200 , hwindowsize = 700;

void reshape(GLFWwindow* window)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    wwindowsize = w;
    hwindowsize = h;
    glOrtho(0, wwindowsize, hwindowsize, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, wwindowsize, hwindowsize);
}

static void key(GLFWwindow* window, int key, int scanmode, int state, int mods)
{
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);
}



int main()
{
    
    if (!glfwInit()) {
        return -1;
    }


    GLFWwindow* window = glfwCreateWindow(wwindowsize, hwindowsize, "GLFW and GLEW", NULL, NULL);
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
    //reshape(window);

    

    float positions[] = {
        -0.5, -0.5, 0,       1,0,0,
        0.5, -0.5, 0,        0,1,0,
        0.5, 0.5, 0,         0,0,1,
        -0.5, 0.5, 0,        0.5, 0.5, 0,


        -50, -50, 0,      1,0,0,
        50, -50, 0,       0,1,0,
        50, 50, 0,         0,0,1,
        -50, 50, 0,       0.5, 0.5, 0,
    };
    
    
    

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW)));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));


    
    /*####################################################################################################
    *glm::mat4 proj = glm::ortho(0.0, (double)wwindowsize, (double)hwindowsize, 0.0, -1.0, 1.0);
    *glm::mat4 tr(1.0);
    *tr = glm::translate(tr, glm::vec3(200.0, 400.0, 0));
    *glm::mat4 ro(1.0);
    *ro = glm::rotate(ro, (float)15.0, glm::vec3(0.0, 0.0, 0.1));
    *ro = glm::scale(ro, glm::vec3(1, 1, 1));
    *glm::mat4 MT = proj * tr * ro;
    * 
    * /\
    * ||
    *POTI SCRIE SI ASA
    *######################################################################################################
    *
    * #############################################
    *MAI OK SI USOR E ASA
    *   ||
    *   \/
    */

    glm::mat4 transform(1.0);
    float angle = 10.0;
    transform = glm::perspective(45.0f, 1.0f, 0.0f, 100.0f);
    transform = glm::ortho(0.0, (double)wwindowsize, (double)hwindowsize, 0.0, -1.0, 1.0);
    transform = glm::translate(transform, glm::vec3(wwindowsize/2, hwindowsize/2, 0.0));
    transform = glm::rotate(transform, (float)glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
    transform = glm::scale(transform, glm::vec3(1, 1, 1));

    
    ShaderProgramSource source = ParseShader("Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));
    

    int location = glGetUniformLocation(shader, "Matrix");
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform)));



    

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        reshape(window);

        angle+=0.001;
        transform = glm::rotate(transform, (float)glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform)));
        glLineWidth(3);
        GLCall(glDrawArrays(GL_POLYGON, 4, 4));
        

        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }

    glDeleteShader(shader);
    glfwTerminate();
    return 0;
}