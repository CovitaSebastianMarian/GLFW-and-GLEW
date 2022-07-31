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
#include "Animations.h"
#include "Physics.h"

#define PI 3.14159265359

using namespace std;


float wwindowsize = 1200, hwindowsize = 700;
bool KEY_W = false, KEY_S = false, KEY_D = false, KEY_A = false, KEY_LEFT_SHIFT = false, KEY_LEFT_CONTROL = false;
bool KEY_UP = false, KEY_DOWN = false, KEY_LEFT = false, KEY_RIGHT = false, KEY_RIGHT_SHIFT = false, KEY_RIGHT_CONTROL = false;
float ycamera = 0, xcamera = 0, zcamera = 0;
float objspeed = 40;
float cameraangle = -90;
float xmouse, ymouse;

Physics _physics;
Physics::Form mapapara, objpara;
Object mapa, obj;


void Config() {
    if (KEY_W) {
        zcamera += sin(glm::radians(cameraangle)) / 10;
        xcamera += cos(glm::radians(cameraangle)) / 10;
    }
    if (KEY_S) {
        zcamera -= sin(glm::radians(cameraangle)) / 10;
        xcamera -= cos(glm::radians(cameraangle)) / 10;
    }
    if (KEY_D) {
        cameraangle++;
    }
    if (KEY_A) {
        cameraangle--;
    }
    if (KEY_LEFT_SHIFT) {
        ycamera += 0.1;
    }
    if (KEY_LEFT_CONTROL) {
        ycamera -= 0.1;
    }


    if (KEY_UP) {
        if (_physics.VerifyZ(objpara, -objspeed)) objpara.zpos -= objspeed;
    }
    if (KEY_DOWN) {
        if (_physics.VerifyZ(objpara, objspeed)) objpara.zpos += objspeed;
    }
    if (KEY_LEFT) {
        if(_physics.VerifyX(objpara, -objspeed)) objpara.xpos -= objspeed;
    }
    if (KEY_RIGHT) {
        if (_physics.VerifyX(objpara, objspeed)) objpara.xpos += objspeed;
    }
    if (KEY_RIGHT_SHIFT) {
        if (_physics.VerifyY(objpara, objspeed)) objpara.ypos += objspeed;
    }
    if (KEY_RIGHT_CONTROL) {
        if (_physics.VerifyY(objpara, -objspeed)) objpara.ypos -= objspeed;
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
    xmouse = x;
    ymouse = hwindowsize - y;
}
void mouse(GLFWwindow* window, int button, int action, int mods) {
    

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

    if (key == GLFW_KEY_LEFT_SHIFT) {
        if (state == GLFW_PRESS) {
            KEY_LEFT_SHIFT = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_LEFT_SHIFT = false;
        }
    }

    if (key == GLFW_KEY_LEFT_CONTROL) {
        if (state == GLFW_PRESS) {
            KEY_LEFT_CONTROL = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_LEFT_CONTROL = false;
        }
    }

    if (key == GLFW_KEY_UP) {
        if (state == GLFW_PRESS) {
            KEY_UP = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_UP = false;
        }
    }

    if (key == GLFW_KEY_DOWN) {
        if (state == GLFW_PRESS) {
            KEY_DOWN = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_DOWN = false;
        }
    }

    if (key == GLFW_KEY_LEFT) {
        if (state == GLFW_PRESS) {
            KEY_LEFT = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_LEFT = false;
        }
    }

    if (key == GLFW_KEY_RIGHT) {
        if (state == GLFW_PRESS) {
            KEY_RIGHT = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_RIGHT = false;
        }
    }

    if (key == GLFW_KEY_RIGHT_SHIFT) {
        if (state == GLFW_PRESS) {
            KEY_RIGHT_SHIFT = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_RIGHT_SHIFT = false;
        }
    }

    if (key == GLFW_KEY_RIGHT_CONTROL) {
        if (state == GLFW_PRESS) {
            KEY_RIGHT_CONTROL = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_RIGHT_CONTROL = false;
        }
    }


}


int main() {
    srand(time(0));
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

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);

    glfwSetKeyCallback(window, key);
    glfwSetCursorPosCallback(window, cursor);
    glfwSetMouseButtonCallback(window, mouse);

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



    float scaleval = 100;
    

    glm::vec3 eye(0, 0.1, 2);
    glm::vec3 center(0, 0.1, -1);
    glm::vec3 up(0, 1, 0);

    
    //mapa
    if (true) {
        ObjFile model;
        model.TextureLocation = "Textures/";
        model.MTLlocation = "Models/";
        model.LoadModel("Models/map.obj");

        mapapara.form_type = Physics::FORM_TYPE::PARALLELEPIPED;
        mapapara.xpos = 0 * scaleval;
        mapapara.ypos = -2 * scaleval;
        mapapara.zpos = 0 * scaleval;
        mapapara.xsize = 100 * scaleval;
        mapapara.ysize = 1 * scaleval;
        mapapara.zsize = 100 * scaleval;
        _physics.InitPhysicsForm(mapapara);

        mapa.Vertices = model.KitVector[0].Vertices;
        mapa.Indices = model.KitVector[0].Indices;
        mapa.Config(GL_DYNAMIC_DRAW);
        mapa.VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
        mapa.VertexAttribpointer(1, 3, 2, 8, GL_FLOAT);
        mapa.VertexAttribpointer(2, 5, 3, 8, GL_FLOAT);
        mapa.Shader("Shaders/Texture.shader");
        mapa.Texture(model.KitVector[0].texture.c_str(), GL_RGB, true);
        mapa.Ortho(-wwindowsize / 2, wwindowsize / 2, -hwindowsize / 2, hwindowsize / 2, wwindowsize / 2, -wwindowsize / 2);
        mapa.Translate(glm::vec3(mapapara.xpos, mapapara.ypos, mapapara.zpos), false);
        mapa.Perspective(glm::radians(85.0), 1, 0.1, 100);
        mapa.LookAt(eye, center, up);
        mapa.Scale(glm::vec3(scaleval, scaleval, scaleval), false);


        mapa.setVec("light.ambient", 0.1f, 0.1f, 0.1f);
        mapa.setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
        mapa.setVec("light.specular", 1.0f, 1.0f, 1.0f);

        mapa.setVec("light.position", mapa.Camera.center.x, mapa.Camera.center.y, mapa.Camera.center.z);
        mapa.setVec("viewPos", mapa.Camera.center.x, mapa.Camera.center.y, mapa.Camera.center.z);


        mapa.setVec("material.shininess", 32);
        mapa.setVec("material.specular", 1.0f, 1.0f, 1.0f);
    }

    //cube
    if (true) {
        ObjFile model;
        model.TextureLocation = "Textures/";
        model.MTLlocation = "Models/";
        model.LoadModel("Models/cube.obj");

        objpara.form_type = Physics::FORM_TYPE::PARALLELEPIPED;
        objpara.xpos = 0;
        objpara.ypos = 0;
        objpara.zpos = 0;
        objpara.xsize = 1 * scaleval;
        objpara.ysize = 1 * scaleval;
        objpara.zsize = 1 * scaleval;
        _physics.InitPhysicsForm(objpara);

        obj.Vertices = model.KitVector[0].Vertices;
        obj.Indices = model.KitVector[0].Indices;
        obj.Config(GL_DYNAMIC_DRAW);
        obj.VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
        obj.VertexAttribpointer(1, 3, 2, 8, GL_FLOAT);
        obj.VertexAttribpointer(2, 5, 3, 8, GL_FLOAT);
        obj.Shader("Shaders/Texture.shader");
        obj.Texture(model.KitVector[0].texture.c_str(), GL_RGB, true);
        obj.Ortho(-wwindowsize / 2, wwindowsize / 2, -hwindowsize / 2, hwindowsize / 2, wwindowsize / 2, -wwindowsize / 2);
        obj.Translate(glm::vec3(objpara.xpos, objpara.ypos, objpara.zpos), false);
        obj.Perspective(glm::radians(85.0), 1, 0.1, 100);
        obj.LookAt(eye, center, up);
        obj.Scale(glm::vec3(scaleval, scaleval, scaleval), false);

        obj.setVec("light.ambient", 0.1f, 0.1f, 0.1f);
        obj.setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
        obj.setVec("light.specular", 1.0f, 1.0f, 1.0f);

        obj.setVec("light.position", obj.Camera.center.x, obj.Camera.center.y, obj.Camera.center.z);
        obj.setVec("viewPos", obj.Camera.center.x, obj.Camera.center.y, obj.Camera.center.z);


        obj.setVec("material.shininess", 32);
        obj.setVec("material.specular", 1.0f, 1.0f, 1.0f);
    }




    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Config();

        glm::vec3 eye(xcamera, ycamera, zcamera);
        glm::vec3 center(cos(glm::radians(cameraangle)) + xcamera, ycamera, sin(glm::radians(cameraangle)) + zcamera);

        glUseProgram(mapa.shader);
        mapa.LookAt(eye, center, up);
        mapa.setVec("light.ambient", 0.1f, 0.1f, 0.1f);
        mapa.setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
        mapa.setVec("light.specular", 1.0f, 1.0f, 1.0f);

        //mapa.setVec("light.position", mapa.Camera.center.x, mapa.Camera.center.y, mapa.Camera.center.z);
        mapa.setVec("light.position", mapa.Camera.center.x, mapa.Camera.center.y, mapa.Camera.center.z);
        mapa.setVec("viewPos", mapa.Camera.center.x, mapa.Camera.center.y, mapa.Camera.center.z);


        mapa.setVec("material.shininess", 32);
        mapa.setVec("material.diffuse", 0);
        mapa.setVec("material.specular", 1.0f, 1.0f, 1.0f);

        mapa.Bind(GL_TRIANGLES);


        glUseProgram(obj.shader);
        obj.Translate(glm::vec3(objpara.xpos, objpara.ypos, objpara.zpos), false);
        obj.LookAt(eye, center, up);
        obj.setVec("light.ambient", 0.1f, 0.1f, 0.1f);
        obj.setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
        obj.setVec("light.specular", 1.0f, 1.0f, 1.0f);

        obj.setVec("light.position", obj.Camera.center.x, obj.Camera.center.y, obj.Camera.center.z);
        obj.setVec("viewPos", obj.Camera.center.x, obj.Camera.center.y, obj.Camera.center.z);


        obj.setVec("material.shininess", 32);
        obj.setVec("material.diffuse", 0);
        obj.setVec("material.specular", 1.0f, 1.0f, 1.0f);
        obj.Bind(GL_TRIANGLES);
       








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