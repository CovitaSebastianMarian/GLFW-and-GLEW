#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "error.h"
#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

using namespace std;

class Window {
private:
    void Error(const char *error) {
        cout << "\033[3;40;31m";
        cout << error;
        cout << "\033[0m";
    }

    ImGuiContext * context = ImGui::CreateContext();

    bool UseImGui = false;
    
    unsigned int shader, VAO[2], VBO;

    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "uniform mat4 Matrix;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = Matrix * vec4(aPos,1, 1);\n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 inColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = inColor;\n"
        "}\0";
public:
    int width, height;
    GLFWwindow* window;


    inline    bool            Create(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
    inline    bool            Open();
    inline    bool            ReshapeWindow();
    inline    void            ConfigImGui();
    inline    void            Begin();
    inline    void            End();
    inline    void            ShutDown();
    inline    bool            getKey(int key);
    inline    const char*     getKeyName(int key, int scancode);
    inline    bool            getMouseButton(int key);
    inline    void            getMouseCursorPos(double* x, double* y);
                           
    //varianta cea mai eficienta
    inline    void            InitPixels(vector<float> pos);
    inline    void            DrawPixels(vector<float> pos, int size, float r = 0, float g = 0, float b = 0, float a = 1);

    //varianta neeficienta
    inline    void            InitPixel();
    inline    void            DrawPixel(double x, double y, int size, int r, int g, int b, int a = 1);




};


    inline    bool            Window::Create(int width_, int height_, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
    if (!glfwInit()) {
        Error("[GLFW: error]");
        return false;
    }
    width = width_;
    height = height_;
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, title, monitor, share);

    if (!window)
    {
        glfwTerminate();
        Error("[Window: error]");
        return false;
    }
    glfwMakeContextCurrent(window);


    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        Error("[Error: glew error]");
        return false;
    }

    glPointSize(50);
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

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

   
    glGenVertexArrays(2, VAO);
}
    inline    bool            Window::Open() {
    if (!glfwWindowShouldClose(window)) return true;
    return false;
}
    inline    bool            Window::ReshapeWindow() {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    if (w != width || h != height) {
        width = w;
        height = h;
        glViewport(0, 0, width, height);
        return true;
    }
    return false;
}
    inline    void            Window::ConfigImGui() {
    UseImGui = true;
    ImGui::SetCurrentContext(context);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    if (true) {
        ImGui::GetStyle().FrameRounding = 4.0f;
        ImGui::GetStyle().GrabRounding = 4.0f;
        ImGui::GetStyle().WindowRounding = 0;
        ImGui::GetStyle().ChildRounding = 0;

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
}
    inline    void            Window::Begin() {
    glfwMakeContextCurrent(window);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ReshapeWindow();
    if (UseImGui) {
        ImGui::SetCurrentContext(context);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
}
    inline    void            Window::End() {
    glfwMakeContextCurrent(window);
    if (UseImGui) {
        ImGui::SetCurrentContext(context);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
}
    inline    void            Window::ShutDown() {
    glfwMakeContextCurrent(window);
    if (UseImGui) {
        ImGui::SetCurrentContext(context);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    glfwTerminate();
}
    inline    bool            Window::getKey(int key) {
    return glfwGetKey(window, key);
}
    inline    const char*     Window::getKeyName(int key, int scancode) {
    return glfwGetKeyName(key, scancode);
}
    inline    bool            Window::getMouseButton(int key) {
    return glfwGetMouseButton(window, key);
}
    inline    void            Window::getMouseCursorPos(double* x, double* y) {
    glfwGetCursorPos(window, x, y);
}


    inline    void            Window::InitPixels(vector<float> pos) {
    glBindVertexArray(VAO[0]);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(float), pos.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
    inline    void            Window::DrawPixels(vector<float> pos, int size, float r, float g, float b, float a) {
    glUseProgram(shader);
    GLint transformLoc = glGetUniformLocation(shader, "inColor");
    glUniform4f(transformLoc, r, g, b, a);

    glm::mat4 matrix(1.0);
    matrix = glm::ortho(0.0, (double)width, 0.0, (double)height);
    int location = glGetUniformLocation(shader, "Matrix");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

    glPointSize(size);
    glLineWidth(size);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_LINE_LOOP, 0, pos.size() / 2);
    glBindVertexArray(0);
}


    inline    void            Window::InitPixel() {
    glBindVertexArray(VAO[1]);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float vertices[] = { 0.0, 0.0 };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
    inline    void            Window::DrawPixel(double x, double y, int size, int r, int g, int b, int a) {
    glUseProgram(shader);
    GLint transformLoc = glGetUniformLocation(shader, "inColor");
    glUniform4f(transformLoc, r, g, b, a);

    glm::mat4 matrix(1.0);
    matrix = glm::ortho(0.0, (double)width, 0.0, (double)height);
    matrix = glm::translate(matrix, glm::vec3(x, y, 0));
    int location = glGetUniformLocation(shader, "Matrix");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

    glPointSize(size);
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
}
