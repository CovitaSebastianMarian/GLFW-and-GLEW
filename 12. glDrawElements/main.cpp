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
double xpos = 0, ypos = 0;
float sze = 1;
int cnt = 4;

void reshape(GLFWwindow* window)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    wwindowsize = w;
    hwindowsize = h;
    glViewport(0, 0, wwindowsize, hwindowsize);
    
}

void key(GLFWwindow* window, int key, int scanmode, int state, int mods)
{
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);
    if (key == GLFW_KEY_W && (state == GLFW_PRESS || state == GLFW_REPEAT) && ypos < 3 * sze / 4) ypos += 0.01;
    if (key == GLFW_KEY_S && (state == GLFW_PRESS || state == GLFW_REPEAT) && ypos > 0) ypos -= 0.01;
    if (key == GLFW_KEY_D && (state == GLFW_PRESS || state == GLFW_REPEAT) && xpos < 3 * sze / 4) xpos+=0.01;
    if (key == GLFW_KEY_A && (state == GLFW_PRESS || state == GLFW_REPEAT) && xpos > -3 * sze / 4) xpos-=0.01;
}
void cursor(GLFWwindow* window, double x, double y) {
    
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



    float positions[] = {
        //positionws        color      texture coord   
        sze, -sze, 0,                          1,0,0,1,  0,0,
        sze, sze, 0,                           0,1,0,1,  0,1,
        -sze, sze, 0,                          0,0,1,1,  1,1,
        -sze, -sze, 0,                         1,1,0,1,  1,0,
        -sze/ cnt + xpos, -sze + ypos, 0,      1,1,0,1,  0,0,
        -sze/ cnt + xpos, sze/ cnt + ypos, 0,  0,0,1,1,  0,0,
        sze/ cnt + xpos, sze/ cnt + ypos, 0,   0,1,0,1,  0,0,
        sze/ cnt + xpos, -sze + ypos, 0,       1,0,0,1,  0,0,
    };
    unsigned int indices[] = {
        0,1,6,7,
        1,6,5,2,
        2,5,4,3,
        3,4,7,0,
    };
    
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    ShaderProgramSource source = ParseShader("Shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);



    while (!glfwWindowShouldClose(window))
    {
        //glClearColor(0.07f, 0.13f, 0.17f, 0.1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);


        glUseProgram(shader);
        glBindVertexArray(VAO);
        
        float positions[] = {
            //positionws        color      texture coord   
            sze, -sze, 0,                          1,0,0,1,  0,0,
            sze, sze, 0,                           0,1,0,1,  0,1,
            -sze, sze, 0,                          0,0,1,1,  1,1,
            -sze, -sze, 0,                         1,1,0,1,  1,0,
            -sze / cnt + xpos, -sze + ypos, 0,      1,1,0,1,  0,0,
            -sze / cnt + xpos, sze / cnt + ypos, 0,  0,0,1,1,  0,0,
            sze / cnt + xpos, sze / cnt + ypos, 0,   0,1,0,1,  0,0,
            sze / cnt + xpos, -sze + ypos, 0,       1,0,0,1,  0,0,
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glDrawElements(GL_QUADS, 16, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);



        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }
    exit(0);
    glfwTerminate();
    return 0;
}