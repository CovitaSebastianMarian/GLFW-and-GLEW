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

    


    glfwSetKeyCallback(window, key);
    glfwSetCursorPosCallback(window, cursor);
    glfwSetScrollCallback(window, scrollfunc);
    glfwSetMouseButtonCallback(window, mouse);



    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    ShaderProgramSource source = ParseShader("Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);



    unsigned int VAO, VBO;

    float positions[] = {
        // positions          // colors           // texture coords
         0.5,  0.5, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5, -0.5, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5, -0.5, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5,  0.5, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left


        100,  100, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         100, -100, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -100, -100, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -100,  100, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left
    };

    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glGenBuffers(1, &VBO));

    GLCall(glBindVertexArray(VAO));
    GLCall(GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW)));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));
    
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));

    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(2));


    int width, height, nrChannels; 
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("grass.jpg", &width, &height, &nrChannels, 0);
   
    GLuint texture; 
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
 
    stbi_image_free(data);

    
    
    glm::mat4 transform(1.0); 
    transform = glm::ortho(0.0, wwindowsize, hwindowsize, 0.0, -1.0, 1.0);
    transform = glm::translate(transform, glm::vec3(wwindowsize/2, hwindowsize/2, 0.0));
    transform = glm::rotate(transform, (float)glm::radians(0.0), glm::vec3(0.0, 0.0, 1.0));
    transform = glm::scale(transform, glm::vec3(1.0, 1.0, 1.0));

    GLCall(glUseProgram(shader));

    int location = glGetUniformLocation(shader, "Matrix");
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform)));

    



    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        reshape(window);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (ok)
        {
            transform = glm::scale(transform, glm::vec3(1.0 + zoom, 1.0 + zoom, 1.0 + zoom));
            GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform)));
            ok = false;
        }
        if (ok2) {
            transform = glm::translate(transform, glm::vec3(xmouse - xpos, ymouse - ypos, 0.0));
            xpos = xmouse;
            ypos = ymouse;
            GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform)));
        }
        glBindVertexArray(VAO);
        GLCall(glDrawArrays(GL_POLYGON, 4, 4));



        

        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }

    glDeleteShader(shader);
    glfwTerminate();
    return 0;
}
