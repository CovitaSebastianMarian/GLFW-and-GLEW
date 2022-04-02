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

void key(GLFWwindow* window, int key, int scanmode, int state, int mods)
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

    

    ShaderProgramSource source = ParseShader("Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));


    float positions[] = {
        -0.5, -0.5, 0,       1,0,0,
        0.5, -0.5, 0,        0,1,0,
        0.5, 0.5, 0,         0,0,1,
        -0.5, 0.5, 0,        0.5, 0.5, 0,


        -100, -100, 0,      1,0,0,
        100, -100, 0,       0,1,0,
        100, 100, 0,         0,0,1,
        -100, 100, 0,       0.5, 0.5, 0,
    };


    
    unsigned int VAO[2], VBO[2];
    /*
    *le generam o singura data 2
    *2 e de la numarul de obiecte pe care le generam
    *#################################
    * la fel e si jos, si atribuim VAO si VBO o singura data
    *   ||
    *   \/
    */
    GLCall(glGenVertexArrays(2, VAO));
    GLCall(glGenBuffers(2, VBO));




    GLCall(glBindVertexArray(VAO[0]));//legam primul obiect 

    GLCall(GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO[0]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW)));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));


    float pos[] = {
        -1, -1, 0,       0.4,0,0,
        -0.5, -1, 0,        0,0,0,
        -0.5, -0.5, 0,      0.4,0,0,
        -1, -0.5, 0,        0,0,0,
    };


    GLCall(glBindVertexArray(VAO[1]));//legam al doilea obiect
    GLCall(GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO[1]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW)));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));

    /*
    * #############################################################
    * #                                                           #
    * #                                                           #
    * #                                                           #
    * #                                                           #
    * #                                                           #
    * #                                                           #
    * #                                                           #
    * #                                                           #
    * #############################################################
    */


    


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        reshape(window);


        glBindVertexArray(VAO[0]);
        GLCall(glDrawArrays(GL_POLYGON, 0, 4));
        glBindVertexArray(VAO[1]);
        GLCall(glDrawArrays(GL_POLYGON, 0, 4));

        

        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }

    glDeleteShader(shader);
    glfwTerminate();
    return 0;
}