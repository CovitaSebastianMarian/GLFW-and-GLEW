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
#include "Circle.h"

#define PI 3.14159265359

using namespace std;


float wwindowsize = 1200, hwindowsize = 700;
bool KEY_W = false, KEY_S = false, KEY_D = false, KEY_A = false;
float angle = 0, xmouse, ymouse;
vector<Circle> circle;
float UP_BORDER, DOWN_BORDER, LEFT_BORDER, RIGHT_BORDER;
bool start = false;

float Colision(Circle &cir, int cnt) {

    for (int i = 0; i < circle.size(); ++i) {
        if (i != cnt) {
           float distance = sqrt(pow(cir.xpos - circle[i].xpos, 2) + pow(cir.ypos - circle[i].ypos, 2));
           if (cir.radius + circle[i].radius > distance) {
               swap(cir.angle, circle[i].angle);
               cir.ChangeDirection();
               circle[i].ChangeDirection();
               return (cir.radius + circle[i].radius) - distance;
           }
        }
    }
    return 0;
}

void Config() {


    for (int i = 0; i < circle.size(); ++i) {
        if (!circle[i].hide && start) {
            double angle = glm::radians(circle[i].angle);
            float xpos2 = circle[i].xpos;
            float ypos2 = circle[i].ypos;
            xpos2 += circle[i].speed * cos(angle);
            ypos2 += circle[i].speed * sin(angle);



            bool UP = true, DOWN = true, RIGHT = true, LEFT = true;


            if (xpos2 < circle[i].radius + LEFT_BORDER) {
                LEFT = false;
            }
            if (xpos2 > RIGHT_BORDER - circle[i].radius) {
                RIGHT = false;
            }
            if (ypos2 < circle[i].radius + DOWN_BORDER) {
                DOWN = false;
            }
            if (ypos2 > UP_BORDER - circle[i].radius) {
                UP = false;
            }


            if (UP && DOWN && RIGHT && LEFT) {
                float xx = circle[i].xpos;
                float yy = circle[i].ypos;
                circle[i].xpos = xpos2;
                circle[i].ypos = ypos2;
                float cnt = Colision(circle[i], i);

                if (cnt != 0) {
                    circle[i].xpos -= cnt * cos(angle);
                    circle[i].ypos -= cnt * sin(angle);
                }
            }
            else {
                if (!UP) {
                    if (circle[i].dir == DIRECTION::UP_RIGHT) {
                        circle[i].angle = 360 - circle[i].angle;
                        circle[i].ChangeDirection();
                    }
                    else if (circle[i].dir == DIRECTION::UP_LEFT) {
                        circle[i].angle = 180 - circle[i].angle + 180;
                        circle[i].ChangeDirection();
                    }
                }
                if (!DOWN) {
                    if (circle[i].dir == DIRECTION::DOWN_RIGHT) {
                        circle[i].angle = 360 - circle[i].angle;
                        circle[i].ChangeDirection();
                    }
                    else if (circle[i].dir == DIRECTION::DOWN_LEFT) {
                        circle[i].angle = 180 - (circle[i].angle - 180);
                        circle[i].ChangeDirection();
                    }
                }
                if (!LEFT) {
                    if (circle[i].dir == DIRECTION::UP_LEFT) {
                        circle[i].angle = 180 - circle[i].angle;
                        circle[i].ChangeDirection();
                    }
                    else if (circle[i].dir == DIRECTION::DOWN_LEFT) {
                        circle[i].angle = 270 - circle[i].angle + 270;
                        circle[i].ChangeDirection();
                    }
                }
                if (!RIGHT) {
                    if (circle[i].dir == DIRECTION::UP_RIGHT) {
                        circle[i].angle = 90 - circle[i].angle + 90;
                        circle[i].ChangeDirection();
                    }
                    else if (circle[i].dir == DIRECTION::DOWN_RIGHT) {
                        circle[i].angle = 360 - circle[i].angle + 180;
                        circle[i].ChangeDirection();
                    }
                }
            }
        }

    }

}





bool reshape(GLFWwindow* window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    if (w != wwindowsize || h != hwindowsize) {
        wwindowsize = w;
        hwindowsize = h;
        glViewport(0, 0, wwindowsize, hwindowsize);
        UP_BORDER = hwindowsize;
        DOWN_BORDER = 0;
        LEFT_BORDER = wwindowsize / 5;
        RIGHT_BORDER = wwindowsize;
        return true;
    }
    return false;
}
void cursor(GLFWwindow* window, double x, double y) {
    xmouse = x;
    ymouse = hwindowsize - y;
}
void mouse(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        Circle cl;
        cl.wwindowsize = RIGHT_BORDER - LEFT_BORDER;
        cl.wwindowsize = wwindowsize;
        cl.hwindowsize = UP_BORDER - DOWN_BORDER;
        cl.hwindowsize = hwindowsize;
        cl.radius = rand() % 10 + 40;
        cl.xpos = xmouse;
        cl.ypos = ymouse;
        cl.angle = rand() % 360;
        cl.ChangeDirection();
        cl.speed = 10;
        cl.mass = 0;
        cl.force = 9.8 * cl.mass;
        cl.hide = false;
        cl.CreateCircle();
        circle.push_back(cl);
    }
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
    //GLCall(glEnable(GL_MULTISAMPLE));
    //GLCall(glEnable(GL_STENCIL_TEST));


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


    //glViewport(wwindowsize / 4, 0, wwindowsize * 3 / 4, hwindowsize);
    UP_BORDER = hwindowsize;
    DOWN_BORDER = 0;
    LEFT_BORDER = wwindowsize / 5;
    RIGHT_BORDER = wwindowsize;


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        reshape(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Edit window", nullptr, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoMove);
        ImGui::SetWindowSize({ LEFT_BORDER, hwindowsize });
        ImGui::SetWindowPos({ 0,0 });

        ImGui::PushStyleColor(ImGuiCol_Button, { 0, 1, 0, 1 });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0, 0.1, 0, 1 });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0, 0.5, 0, 1 });
        ImGui::PushStyleColor(ImGuiCol_Text, { 0, 0, 0, 1 });
        if (!start) {
            if (ImGui::Button("START", { 100, 0 })) {
                if (start) start = false;
                else start = true;
            }
        }
        else {
            if (ImGui::Button("PAUSE", { 100, 0 })) {
                if (start) start = false;
                else start = true;
            }
        }
        ImGui::PopStyleColor(4);


        for (int i = 0; i < circle.size(); ++i) {
            stringstream ss;
            ss << i + 1;
            string nr;
            ss >> nr;
            string name = "Circle" + nr;

            ImGui::Spacing();

            if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::BeginChild(i + 1, { 0, 270 }, true);
                

                ImGui::TextColored({ 0.0, 0.5, 0.5, 1 }, "EDIT");
                if (circle[i].angle >= 360) circle[i].angle -= 360;
                else if (circle[i].angle < 0) circle[i].angle = 360 + circle[i].angle;
                ImGui::InputFloat(" :Angle", &circle[i].angle, 1, 1);
                ImGui::InputFloat(" :X", &circle[i].xpos, 1, 1);
                ImGui::InputFloat(" :Y", &circle[i].ypos, 1, 1);
                ImGui::InputFloat(" :Radius", &circle[i].radius, 1, 1);
                ImGui::InputFloat(" :Speed", &circle[i].speed, 1, 1);

                ImGui::InputFloat(" :Velocity", &circle[i].velocity, 1, 1);
                ImGui::InputFloat(" :Force", &circle[i].force, 1, 1);
                ImGui::InputFloat(" :Mass", &circle[i].mass, 1, 1);

                switch (circle[i].dir)
                {
                case DIRECTION::NONE:
                    break;
                case DIRECTION::UP_RIGHT:
                    ImGui::LabelText(" :DIRECTION", "UP_RIGHT");
                    break;
                case DIRECTION::UP_LEFT:
                    ImGui::LabelText(" :DIRECTION", "UP_LEFT");
                    break;
                case DIRECTION::DOWN_LEFT:
                    ImGui::LabelText(" :DIRECTION", "DOWN_LEFT");
                    break;
                case DIRECTION::DOWN_RIGHT:
                    ImGui::LabelText(" :DIRECTION", "DOWN_RIGHT");
                    break;
                default:
                    break;
                }


                

                if (ImGui::BeginTable("!!!", 2)) {

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushStyleColor(ImGuiCol_Button, { 1, 0, 0, 1 });
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.1, 0, 0, 1 });
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.5, 0, 0, 1 });
                    if (ImGui::Button("DELETE", { 80, 0 })) {
                        circle.erase(circle.begin() + i);
                    }
                    ImGui::PopStyleColor(3);


                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 1, 1 });
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0, 0, 0.1, 1 });
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0, 0, 0.5, 1 });
                    if (ImGui::Button("HIDE", { 80, 0 })) {
                        if (circle[i].hide) circle[i].hide = false;
                        else circle[i].hide = true;
                    }
                    ImGui::PopStyleColor(3);
                    ImGui::EndTable();
                }

                



                ImGui::EndChild();
            }


        }
        ImGui::End();

        

        Config();

















        for (size_t i = 0; i < circle.size(); ++i) {
            if(!circle[i].hide)circle[i].Draw();
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