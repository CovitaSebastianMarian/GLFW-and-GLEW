#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include "Shader.h"
#include "error.h"
#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "Form.h"


#define PI 3.14159265359

using namespace std;


double wwindowsize = 1200 , hwindowsize = 700;
float xangle = 0, yangle = 0, zangle = 0;


void reshape(GLFWwindow* window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    wwindowsize = w;
    hwindowsize = h;
    glViewport(0, 0, wwindowsize, hwindowsize);
}
bool ok = false;
void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);

    if (key == GLFW_KEY_UP && (state == GLFW_PRESS || state == GLFW_REPEAT)) xangle++;
    if (key == GLFW_KEY_DOWN && (state == GLFW_PRESS || state == GLFW_REPEAT)) xangle--;

    if (key == GLFW_KEY_LEFT && (state == GLFW_PRESS || state == GLFW_REPEAT)) yangle++;
    if (key == GLFW_KEY_RIGHT && (state == GLFW_PRESS || state == GLFW_REPEAT)) yangle--;
}


int main() {
    
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window;
    glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    window = glfwCreateWindow(wwindowsize, hwindowsize, "GLFW and GLEW", NULL, NULL);
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
    GLCall(glEnable(GL_MULTISAMPLE));
    GLCall(glEnable(GL_STENCIL_TEST));
    

    glfwSetKeyCallback(window, key);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    float xsze1 = 10, ysze1 = 10, xsze2 = 10, ysze2 = 10,height = 0;
    int edges = 3;

    Object Face[3];
    Form cilindru;
    Form::FormDetails fd[3];
    fd[0] = cilindru.CreateFace1(xsze1, ysze1, height, edges, 45);
    fd[1] = cilindru.CreateFace2(xsze2, ysze2, height, edges, 45);
    fd[2] = cilindru.CreateLateralFace(xsze1, ysze1, xsze2, ysze2, height, edges, 0);

    for (int i = 0; i < 3; ++i) {
        Face[i].Init(fd[i].positions, sizeof(fd[i].positions), fd[i].indices, sizeof(fd[i].indices));
        Face[i].VertexAttribpointer(0, 0, 3, 5, GL_FLOAT);
        Face[i].VertexAttribpointer(1, 3, 2, 5, GL_FLOAT);
        Face[i].Shader("Shaders/Basic.shader");
        if(i==0 || i==1) Face[i].Texture("Textures/circle.jpg", GL_RGB, false);
        else  Face[i].Texture("Textures/brickstone.jpg", GL_RGB, true);
        Face[i].Ortho(0, wwindowsize, hwindowsize, 0, 100, -100);
        Face[i].Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);
        Face[i].Perspective(glm::radians(45.0), 1, 0.1, 100);
        Face[i].LookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
    }


    

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        
        ImGui::NewFrame();
        ImGui::Begin("Edit window");
        ImGui::SliderInt("Edges", &edges, 3, 100);
        ImGui::SliderFloat("XSize1", &xsze1, 0, min(wwindowsize, hwindowsize));
        ImGui::SliderFloat("YSize1", &ysze1, 0, min(wwindowsize, hwindowsize));
        ImGui::SliderFloat("XSize2", &xsze2, 0, min(wwindowsize, hwindowsize));
        ImGui::SliderFloat("YSize2", &ysze2, 0, min(wwindowsize, hwindowsize));
        ImGui::SliderFloat("Height", &height, 0, min(wwindowsize, hwindowsize));
        ImGui::SliderFloat("X", &xangle, -360, 360);
        ImGui::SliderFloat("Y", &yangle, -360, 360);
        ImGui::SliderFloat("Z", &zangle, -360, 360);
        ImGui::End();



        fd[0] = cilindru.CreateFace1(xsze1, ysze1, height, edges, 45);
        fd[1] = cilindru.CreateFace2(xsze2, ysze2, height, edges, 45);
        fd[2] = cilindru.CreateLateralFace(xsze1, ysze1, xsze2, ysze2, height, edges, 0);
        for (int i = 0; i < 3; ++i) {
            Face[i].Reload(fd[i].positions, sizeof(fd[i].positions), fd[i].indices, sizeof(fd[i].indices));
            Face[i].Rotate(glm::radians(xangle), glm::vec3(1, 0, 0), false);
            Face[i].Rotate(glm::radians(yangle), glm::vec3(0, 1, 0), true);
            Face[i].Rotate(glm::radians(zangle), glm::vec3(0, 0, 1), true);
            Face[i].Bind(GL_TRIANGLES, fd[i].indices_size, GL_UNSIGNED_INT, 0);
        }
        






        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}