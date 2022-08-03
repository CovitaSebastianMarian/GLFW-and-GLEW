#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <Windows.h>
#include <algorithm>
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


double wwindowsize = 1200, hwindowsize = 700;
float scaleval = 10, playerxpos, playerypos = 0, playerzpos;
bool KEY_W = false, KEY_S = false, KEY_D = false, KEY_A = false;
Object* player;
Object* mapa;
ObjFile mapafile;
ObjFile playerfile;
int x = 1, z = 1;
float speed = 10;
void Config(char Map[21][21]) {
    int xx = x, zz = z;
    if (KEY_W) {
        playerzpos -= speed;
        zz = (playerzpos - (5 * scaleval) + 100 * scaleval) / (10 * scaleval);
        if (Map[zz][xx] == '#') playerzpos += speed, zz = z;
    }
    if (KEY_S) {
        playerzpos += speed;
        zz = (playerzpos + (5 * scaleval) + 100 * scaleval) / (10 * scaleval);
        if (Map[zz][xx] == '#') playerzpos -= speed, zz = z;
    }
    if (KEY_D) {
        playerxpos += speed;
        xx = (playerxpos + (5 * scaleval) + 100 * scaleval) / (10 * scaleval);
        if (Map[zz][xx] == '#') playerxpos -= speed, xx = x;
    }
    if (KEY_A) {
        playerxpos -= speed;
        xx = (playerxpos - (5 * scaleval) + 100 * scaleval) / (10 * scaleval);
        if (Map[zz][xx] == '#') playerxpos += speed, xx = x;
    }


    x = xx;
    z = zz;

}




void reshape(GLFWwindow* window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    wwindowsize = w;
    hwindowsize = h;
    glViewport(0, 0, wwindowsize, hwindowsize);
}

void mouse(GLFWwindow* window, int button, int action, int mods) {

}
void cursor(GLFWwindow* window, double x, double y) {

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
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);


    //wwindowsize = mode->width;
    //hwindowsize = mode->height;
    //window = glfwCreateWindow(mode->width, mode->height, "GLFW and GLEW", monitor, NULL);
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
    glfwSetMouseButtonCallback(window, mouse);

    //Imgui Window
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    if (true) {
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
    }


    char Map[21][21] = {
    "####################",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "####################"
    };



    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            if (i == z && j == x) {
                playerxpos = (j + 0.5) * (10 * scaleval) - (100 * scaleval);
                playerzpos = (i + 0.5) * (10 * scaleval) - (100 * scaleval);
            }
        }
    }







    glm::vec3 eye(0, 3, 3);
    glm::vec3 center(0, 0, -0.01);
    glm::vec3 up(0, 1, 0);



    //mapa
    mapafile.MTLlocation = "Models/";
    mapafile.TextureLocation = "Textures/";
    mapafile.LoadModel("Models/mapa.obj", 4);
    mapa = new Object[mapafile.KitVector.size()];
    for (int i = 0; i < mapafile.KitVector.size(); ++i) {
        mapa[i].Vertices = mapafile.KitVector[i].Vertices;
        mapa[i].Indices = mapafile.KitVector[i].Indices;
        mapa[i].Config(GL_DYNAMIC_DRAW);
        mapa[i].VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
        mapa[i].VertexAttribpointer(1, 3, 2, 8, GL_FLOAT);
        mapa[i].VertexAttribpointer(2, 5, 3, 8, GL_FLOAT);
        mapa[i].Shader("Shaders/Texture.shader");
        mapa[i].Texture(mapafile.KitVector[i].texture.c_str(), GL_RGB, true);
        mapa[i].Ortho(-wwindowsize / 2, wwindowsize / 2, -hwindowsize / 2, hwindowsize / 2, wwindowsize / 2, -wwindowsize / 2);
        mapa[i].Perspective(glm::radians(85.0), 1, 0.1, 100);
        mapa[i].LookAt(eye, center, up);
        mapa[i].Scale(glm::vec3(scaleval, scaleval, scaleval), false);

        mapa[i].setVec("light.ambient", 0.1f, 0.1f, 0.1f);
        mapa[i].setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
        mapa[i].setVec("light.specular", 1.0f, 1.0f, 1.0f);

        mapa[i].setVec("light.position", mapa[i].Camera.eye.x, mapa[i].Camera.eye.y, mapa[i].Camera.eye.z);
        mapa[i].setVec("viewPos", mapa[i].Camera.eye.x, mapa[i].Camera.eye.y, mapa[i].Camera.eye.z);


        mapa[i].setVec("material.shininess", 32);
        mapa[i].setVec("material.specular", 1.0f, 1.0f, 1.0f);
    }

    //player
    playerfile.MTLlocation = "Models/";
    playerfile.TextureLocation = "Textures/";
    playerfile.LoadModel("Models/player.obj", 4);
    player = new Object[playerfile.KitVector.size()];
    for (int i = 0; i < playerfile.KitVector.size(); ++i) {
        player[i].Vertices = playerfile.KitVector[i].Vertices;
        player[i].Indices = playerfile.KitVector[i].Indices;
        player[i].Config(GL_DYNAMIC_DRAW);
        player[i].VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
        player[i].VertexAttribpointer(1, 3, 2, 8, GL_FLOAT);
        player[i].VertexAttribpointer(2, 5, 3, 8, GL_FLOAT);
        player[i].Shader("Shaders/Texture.shader");
        player[i].Texture(playerfile.KitVector[i].texture.c_str(), GL_RGB, true);
        player[i].Ortho(-wwindowsize / 2, wwindowsize / 2, -hwindowsize / 2, hwindowsize / 2, wwindowsize / 2, -wwindowsize / 2);
        player[i].Perspective(glm::radians(85.0), 1, 0.1, 100);
        player[i].LookAt(eye, center, up);
        player[i].Translate(glm::vec3(playerxpos, playerypos, playerzpos), false);
        player[i].Scale(glm::vec3(scaleval, scaleval, scaleval), false);

        player[i].setVec("light.ambient", 0.1f, 0.1f, 0.1f);
        player[i].setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
        player[i].setVec("light.specular", 1.0f, 1.0f, 1.0f);

        player[i].setVec("light.position", player[i].Camera.eye.x, player[i].Camera.eye.y, player[i].Camera.eye.z);
        player[i].setVec("viewPos", player[i].Camera.eye.x, player[i].Camera.eye.y, player[i].Camera.eye.z);


        player[i].setVec("material.shininess", 32);
        player[i].setVec("material.specular", 1.0f, 1.0f, 1.0f);
    }











    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);
        Config(Map);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (int i = 0; i < mapafile.KitVector.size(); ++i) {
            glUseProgram(mapa[i].shader);
            mapa[i].setVec("light.ambient", 0.1f, 0.1f, 0.1f);
            mapa[i].setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
            mapa[i].setVec("light.specular", 1.0f, 1.0f, 1.0f);

            mapa[i].setVec("light.position", mapa[i].Camera.eye.x, mapa[i].Camera.eye.y, mapa[i].Camera.eye.z);
            mapa[i].setVec("viewPos", mapa[i].Camera.eye.x, mapa[i].Camera.eye.y, mapa[i].Camera.eye.z);


            mapa[i].setVec("material.shininess", 32);
            mapa[i].setVec("material.specular", 1.0f, 1.0f, 1.0f);


            mapa[i].Bind(GL_QUADS);
        }

        for (int i = 0; i < playerfile.KitVector.size(); ++i) {
            player[i].Translate(glm::vec3(playerxpos, playerypos, playerzpos), false);
            glUseProgram(player[i].shader);
            player[i].setVec("light.ambient", 0.1f, 0.1f, 0.1f);
            player[i].setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
            player[i].setVec("light.specular", 1.0f, 1.0f, 1.0f);

            player[i].setVec("light.position", player[i].Camera.eye.x, player[i].Camera.eye.y, player[i].Camera.eye.z);
            player[i].setVec("viewPos", player[i].Camera.eye.x, player[i].Camera.eye.y, player[i].Camera.eye.z);


            player[i].setVec("material.shininess", 32);
            player[i].setVec("material.specular", 1.0f, 1.0f, 1.0f);


            player[i].Bind(GL_QUADS);
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