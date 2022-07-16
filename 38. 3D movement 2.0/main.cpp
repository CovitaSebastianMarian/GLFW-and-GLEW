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


float wwindowsize = 1200, hwindowsize = 700;
bool KEY_W = false, KEY_S = false, KEY_D = false, KEY_A = false;
float xpos = 0, zpos = 0, ypos = 0.3, ycenterpos = ypos / 2;
float cnt = 90;
float angle = cnt * PI / 180;


void Config() {

    if (KEY_W) {
        zpos += (sin(angle) / 100);
        xpos += (cos(angle) / 100);
        
    }
    if (KEY_S) {
        zpos -= (sin(angle) / 100);
        xpos -= (cos(angle) / 100);
    }
    if (KEY_D) {
        cnt++;
        angle = cnt * PI / 180;
    }
    if (KEY_A) {
        cnt--;
        angle = cnt * PI / 180;

    }


}


bool reshape(GLFWwindow* window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    if (w != wwindowsize || h != hwindowsize) {
        wwindowsize = w;
        hwindowsize = h;
        glViewport(0, 0, wwindowsize, hwindowsize);
        return true;
    }
    return false;
}
void cursor(GLFWwindow* window, double x, double y) {
    /*
    glfwSetCursorPos(window, wwindowsize / 2, hwindowsize / 2);

    float xres = x - wwindowsize / 2;
    float yres = y - hwindowsize / 2;
    if (xres > 0) {
        cnt += 2;
        angle = cnt * PI / 180;
    }
    else if (xres < 0) {
        cnt -= 2;
        angle = cnt * PI / 180;
    }
    */

}

void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);

    if (key == GLFW_KEY_W) {
        if (state == GLFW_PRESS) {
            KEY_W = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_W = false;
        }
    }

    if (key == GLFW_KEY_S) {
        if (state == GLFW_PRESS) {
            KEY_S = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_S = false;
        }
    }

    if (key == GLFW_KEY_D) {
        if (state == GLFW_PRESS) {
            KEY_D = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_D = false;
        }
    }

    if (key == GLFW_KEY_A) {
        if (state == GLFW_PRESS) {
            KEY_A = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_A = false;
        }
    }

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
    glfwSetCursorPosCallback(window, cursor);




    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGui::GetStyle().FrameRounding = 4.0f;
    ImGui::GetStyle().GrabRounding = 4.0f;
    ImGui::GetStyle().WindowRounding = 10;
    ImGui::GetStyle().ChildRounding = 10;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);















    ObjFile model;
    model.TextureLocation = "Textures/";
    model.MTLlocation = "Models/";
    model.LoadModel("Models/cube.obj");
    Object* obj = new Object[model.KitVector.size()];
    for (int i = 0; i < model.KitVector.size(); ++i) {
        obj[i].Vertices = model.KitVector[i].Vertices;
        obj[i].Indices = model.KitVector[i].Indices;
        obj[i].Config(GL_DYNAMIC_DRAW);
        obj[i].VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
        obj[i].VertexAttribpointer(1, 3, 2, 8, GL_FLOAT);
        obj[i].VertexAttribpointer(2, 5, 3, 8, GL_FLOAT);
        obj[i].Shader("Shaders/Texture.shader");
        obj[i].Texture(model.KitVector[i].texture.c_str(), GL_RGB, true);
        obj[i].Ortho(-wwindowsize/2, wwindowsize/2, -hwindowsize/2, hwindowsize, wwindowsize/2, -wwindowsize/2);
        //obj[i].Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);

        obj[i].Perspective(glm::radians(85.0), 1, 0.1, 100);
        obj[i].LookAt(glm::vec3(xpos, ypos, zpos), glm::vec3(cos(angle) + xpos, ycenterpos, sin(angle) + zpos), glm::vec3(0, 1, 0));
        obj[i].Scale(glm::vec3(100, 100, 100), true);


        obj[i].setVec("light.ambient", 0.1f, 0.1f, 0.1f);
        obj[i].setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
        obj[i].setVec("light.specular", 1.0f, 1.0f, 1.0f);

        obj[i].setVec("light.position", obj[i].Camera.eye.x, obj[i].Camera.eye.y, obj[i].Camera.eye.z);
        obj[i].setVec("viewPos", obj[i].Camera.eye.x, obj[i].Camera.eye.y, obj[i].Camera.eye.z);



        obj[i].setVec("material.shininess", 32);
    }



    const char Map[33][33] = {
        "################################",
        "#@                             #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "#                              #",
        "################################"
    };


    double xtarget;
    double ztarget;

    double square_size = (float)1 / 16;
    

    for (int i = 0; i < 32; ++i) {
        ztarget = i * square_size + square_size / 2;
        for (int j = 0; j < 32; ++j) {
            xtarget = j * square_size + square_size / 2;

            cout << xtarget  << " " << ztarget  << " " <<square_size<<"\n";
            if (Map[i][j] == '7') {
                xpos = xtarget;
                zpos = ztarget;
            }

        }
        xtarget = -1;
    }
    






    float v[4];


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (reshape(window)) {
            for (int i = 0; i < model.KitVector.size(); ++i) {
                obj[i].Ortho(-wwindowsize / 2, wwindowsize / 2, -hwindowsize / 2, hwindowsize, wwindowsize / 2, -wwindowsize / 2);
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Config();

        ImGui::Begin("Edit");
        if(ImGui::CollapsingHeader("Translate")) {
            v[0] = xpos;
            v[1] = zpos;
            v[2] = ypos;
            v[3] = ycenterpos;
            ImGui::InputFloat4("X:Z:Y:YC", v);
            xpos = v[0];
            zpos = v[1];
            ypos = v[2];
            ycenterpos = v[3];
        }
        ImGui::End();
        




        for (size_t i = 0; i < model.KitVector.size(); ++i) {
            glUseProgram(obj[i].shader);
            obj[i].LookAt(glm::vec3(xpos, ypos, zpos), glm::vec3(cos(angle) + xpos, ycenterpos, sin(angle) + zpos), glm::vec3(0, 1, 0));
            obj[i].setVec("light.ambient", 0.1f, 0.1f, 0.1f);
            obj[i].setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
            obj[i].setVec("light.specular", 1.0f, 1.0f, 1.0f);

            obj[i].setVec("light.position", obj[i].Camera.eye.x, obj[i].Camera.eye.y, obj[i].Camera.eye.z);
            obj[i].setVec("viewPos", obj[i].Camera.eye.x, obj[i].Camera.eye.y, obj[i].Camera.eye.z);



            obj[i].setVec("material.shininess", 32);

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