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


#define PI 3.14159265359


using namespace std;


double wwindowsize = 1200 , hwindowsize = 700;
double xangle = 0 , yangle = 90;

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
}


int main()
{
    
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window;
    glfwWindowHint(GLFW_SAMPLES, 4);
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

    float sze = 1;
    float pos[] = {
        //positions
        //-sze, sze, 0,
        //sze, sze, 0,
        //0, -sze, 0,
        -0.5, 0.5, 0,
        0.5, 0.5, 0,
        0, -0.5, 0,
    };

    Object obj;
    obj.Init(pos, sizeof(pos));
    obj.VertexAttribpointer(0, 0, 3, 3, GL_FLOAT);
    obj.Shader("Shaders/Basic.shader");
    obj.Ortho(0, wwindowsize, hwindowsize, 0, -1, 1);
    obj.Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);

    int location = glGetUniformLocation(obj.shader, "color");
    int sizelocation = glGetUniformLocation(obj.shader, "size");
    float color[4] = { 0,0,0,1 };
    glUseProgram(obj.shader);
    glUniform4f(location, color[0], color[1], color[2], color[3]);
    glUniform1f(sizelocation, sze);

    bool draw = false;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Imgui window");
        ImGui::Text("Buna ziua copii!!");
        ImGui::Checkbox("Visible", &draw);
        ImGui::SliderFloat("Size", &sze, 1, 600);
        ImGui::ColorEdit4("Color", color);
        ImGui::End();

        glUseProgram(obj.shader);
        glUniform4f(location, color[0], color[1], color[2], color[3]);
        glUniform1f(sizelocation, sze);

        if(draw) obj.Bind(GL_TRIANGLES, 0, 3);



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


       
        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::microseconds(1000 / 60));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}