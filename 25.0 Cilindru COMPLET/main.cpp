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


void Build(float pos[], unsigned int ind[], int &cnt, int &icnt, float sze, float hsze, int stride) {
    cnt = -1, icnt = -1;
    pos[++cnt] = 0;
    pos[++cnt] = 0;
    pos[++cnt] = hsze/2;

    pos[++cnt] = 0.5;
    pos[++cnt] = 0.5;

    int ant = 0;

    int jump;
    while (360 % stride != 0) {
        stride--;
    }
    jump = 360 / stride;
    for (int i = 0; i <= 360; i += jump) {
        double delta = i * PI / 180;
        pos[++cnt] = cos(delta) * sze / 2;
        pos[++cnt] = sin(delta) * sze / 2;
        pos[++cnt] = hsze / 2;

        pos[++cnt] = cos(delta) / 2 + 0.5;
        pos[++cnt] = sin(delta) / 2 + 0.5;


        if (ant != 0) {
            ind[++icnt] = 0;
            ind[++icnt] = ant + 1;
            ind[++icnt] = ant;
        }
        ant++;
    }
    ant++;
    int ant2 = ant;
    pos[++cnt] = 0;
    pos[++cnt] = 0;
    pos[++cnt] = -hsze / 2;

    pos[++cnt] = 0.5;
    pos[++cnt] = 0.5;

    for (int i = 0; i <= 360; i += jump) {
        double delta = i * PI / 180;
        pos[++cnt] = cos(delta) * sze / 2;
        pos[++cnt] = sin(delta) * sze / 2;
        pos[++cnt] = -hsze / 2;

        pos[++cnt] = cos(delta) / 2 + 0.5;
        pos[++cnt] = sin(delta) / 2 + 0.5;

        if (ant != ant2) {
            ind[++icnt] = ant2;
            ind[++icnt] = ant + 1;
            ind[++icnt] = ant;
        }
        ant++;
    }


    for (int i = 0; i < stride * 3; i += 3) {
        int j = ind[i + 1] + ant2;
        ind[++icnt] = ind[i + 1];
        ind[++icnt] = ind[i + 2];
        ind[++icnt] = j;

        ind[++icnt] = j;
        ind[++icnt] = j - 1;
        ind[++icnt] = ind[i + 2];
    }


    icnt++;
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
    

    glfwSetKeyCallback(window, key);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    float sze = 200, height = 1;
    int edges = 6;

    Object Face[3];
    Form cilindru;
    Form::FormDetails fd[3];
    fd[0] = cilindru.CreateFace1(sze, height, edges);
    fd[1] = cilindru.CreateFace2(sze, height, edges);
    fd[2] = cilindru.CreateLateralFace(sze, height, edges);

    for (int i = 0; i < 3; ++i) {
        Face[i].Init(fd[i].positions, sizeof(fd[i].positions), fd[i].indices, sizeof(fd[i].indices));
        Face[i].VertexAttribpointer(0, 0, 3, 5, GL_FLOAT);
        Face[i].VertexAttribpointer(1, 3, 2, 5, GL_FLOAT);
        Face[i].Shader("Shaders/Basic.shader");
        Face[i].Texture("Textures/image.jpg", GL_RGB, false);
        Face[i].Ortho(0, wwindowsize, hwindowsize, 0, -100, 100);
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
        ImGui::Begin("Imgui window");
        ImGui::SliderInt("Stride", &edges, 3, 100);
        ImGui::SliderFloat("Size", &sze, 10, min(wwindowsize, hwindowsize));
        ImGui::SliderFloat("Height", &height, 0, min(wwindowsize, hwindowsize));
        ImGui::SliderFloat("X", &xangle, -360, 360);
        ImGui::SliderFloat("Y", &yangle, -360, 360);
        ImGui::SliderFloat("Z", &zangle, -360, 360);
        ImGui::End();


        fd[0] = cilindru.CreateFace1(sze, height, edges);
        fd[1] = cilindru.CreateFace2(sze, height, edges);
        fd[2] = cilindru.CreateLateralFace(sze, height, edges);
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