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


double wwindowsize = 1200 , hwindowsize = 700;
double xmouse, ymouse, xpos, ypos;
double zoom = 0.0;
float angle = 0.0;



void reshape(GLFWwindow* window)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    wwindowsize = w;
    hwindowsize = h;
    glViewport(0, 0, wwindowsize, hwindowsize);
}

void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);
    else if (key == GLFW_KEY_D && (state == GLFW_REPEAT || state == GLFW_PRESS)) angle++;
    else if (key == GLFW_KEY_A && (state == GLFW_REPEAT || state == GLFW_PRESS)) angle--;
}
void cursor(GLFWwindow* window, double x, double y) {
    xmouse = x;
    ymouse = y;
}
bool ok2 = false;
void mouse(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
       if (!ok2) {
            xpos = xmouse;
            ypos = ymouse;
            ok2 = true;
       }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        ok2 = false;
    }
}
bool ok = false;
void scrollfunc(GLFWwindow* window, double x, double y)
{
    if (y == 1)
    {
        ok = true;
        zoom = 0.4;
    }
    else if (y == -1)
    {
        ok = true;
        zoom = -0.4;
    }
    reshape(window);
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

    if (glewInit() != GLEW_OK)
    {    
        cout << "\033[3;40;31m[Error: glew =try> move glewinit() after window init]" << endl;
        return -1;
    }

    

    GLCall(glEnable(GL_POINT_SMOOTH));
    GLCall(glHint(GL_POINT_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_LINE_SMOOTH));
    GLCall(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_POLYGON_SMOOTH));
    GLCall(glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));


    glfwSetKeyCallback(window, key);
    glfwSetCursorPosCallback(window, cursor);
    glfwSetScrollCallback(window, scrollfunc);
    glfwSetMouseButtonCallback(window, mouse);


    float positions[] = {
        //positions       color    texture
        -100.0, -100.0, 100.0,    1,0,0,   0.0,0.0,
        100.0, -100.0, 100.0,     0,1,0,   1.0,0.0,
        100.0, 100.0, 100.0,      0,0,1,   1.0,1.0,
        -100.0, 100.0, 100.0,     1,1,0,   0.0,1.0,

        100.0, -100.0, -100.0,    1,0,0,   0.0,0.0,
        100.0, -100.0, 100.0,     0,1,0,   1.0,0.0,
        100.0, 100.0, 100.0,      0,0,1,   1.0,1.0,
        100.0, 100.0, -100.0,     1,1,0,   0.0,1.0,

        -100.0, -100.0, -100.0,    1,0,0,   0.0,0.0,
        100.0, -100.0, -100.0,     0,1,0,   1.0,0.0,
        100.0, 100.0, -100.0,      0,0,1,   1.0,1.0,
        -100.0, 100.0, -100.0,     1,1,0,   0.0,1.0,

        -100.0, -100.0, -100.0,    1,0,0,   0.0,0.0,
        -100.0, -100.0, 100.0,     0,1,0,   1.0,0.0,
        -100.0, 100.0, 100.0,      0,0,1,   1.0,1.0,
        -100.0, 100.0, -100.0,     1,1,0,   0.0,1.0,
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("grass.jpg", &width, &height, &nrChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);






    ShaderProgramSource source = ParseShader("Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    glm::mat4 Perspective = glm::perspective(glm::radians(25.0f), 1.0f, 0.1f, 100.0f);

    glm::mat4 Camera = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    
    glm::mat4 transform(1.0);
    transform = glm::ortho(0.0, wwindowsize, hwindowsize, 0.0, 100.0, -100.0);
    transform = glm::translate(transform, glm::vec3(wwindowsize / 2, hwindowsize / 2, 0));
    transform = glm::scale(transform, glm::vec3(1, 1, 1));
    transform = glm::rotate(transform, glm::radians(0.0f), glm::vec3(0, 1, 0));

    glm::mat4 PT = Perspective * Camera * transform;
    ;
    int location = glGetUniformLocation(shader, "Matrix");

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(PT));





    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);

        glm::mat4 transform(1.0);
        transform = glm::ortho(0.0, wwindowsize, hwindowsize, 0.0, 100.0, -100.0);
        transform = glm::translate(transform, glm::vec3(wwindowsize / 2, hwindowsize / 2, 0));
        transform = glm::scale(transform, glm::vec3(1, 1, 1));
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0, 1, 0));
        glm::mat4 PT = Perspective * Camera * transform;
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(PT));

        glBindVertexArray(VAO);
        glDrawArrays(GL_QUADS, 0, 16);
        
        

        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }


    glfwTerminate();
    return 0;
}