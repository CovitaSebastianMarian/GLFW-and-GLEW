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


double wwindowsize = 1200, hwindowsize = 700;
float xlight = 2, ylight = 0, zlight = 2;

void reshape(GLFWwindow* window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    wwindowsize = w;
    hwindowsize = h;
    glViewport(0, 0, wwindowsize, hwindowsize);
}
void cursor(GLFWwindow* window, double x, double y) {

}


void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);


}


int main() {

    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window;
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
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
    glfwSetCursorPosCallback(window, cursor);

    ObjFile model;
    model.Location = "Textures/";
    model.LoadModel("Models/cube.obj", "Models/cube.mtl");
    Object* obj = new Object[model.KitVector.size()];
    for (int i = 0; i < model.KitVector.size(); ++i) {
        obj[i].Vertices = model.KitVector[i].Vertices;
        obj[i].Indices = model.KitVector[i].Indices;
        obj[i].Config(GL_STATIC_DRAW);
        obj[i].VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
        obj[i].VertexAttribpointer(1, 3, 2, 8, GL_FLOAT);
        obj[i].VertexAttribpointer(2, 5, 3, 8, GL_FLOAT);
        obj[i].Shader("Shaders/Light.shader");
        obj[i].Texture(model.KitVector[i].texture.c_str(), GL_RGB, true);
        obj[i].Ortho(0, wwindowsize, 0, hwindowsize, 100, -100);
        obj[i].Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);
        obj[i].Perspective(glm::radians(45.0), 1, 0.1, 100);
        obj[i].LookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
        obj[i].Scale(glm::vec3(100, 100, 100), false);
    }
    




    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");



    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Imgui window");
        ImGui::SliderFloat("X", &xlight, -10, 10);
        ImGui::SliderFloat("Y", &ylight, -10, 10);
        ImGui::SliderFloat("Z", &zlight, -10, 10);
        ImGui::End();

        for (int i = 0; i < model.KitVector.size(); ++i) {
            int lp = glGetUniformLocation(obj[i].shader, "lightPos");
            int lc = glGetUniformLocation(obj[i].shader, "lightColor");
            int vp = glGetUniformLocation(obj[i].shader, "viewPos");
            glUniform3f(lp, xlight, ylight ,zlight);
            glUniform3f(lc, 1, 1, 1);
            glUniform3f(vp, 0, 0, 3);
            //obj[i].Rotate(glm::radians(-1.0), glm::vec3(0, 1, 0), true);
            obj[i].Bind(GL_TRIANGLES);


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
