#pragma once
#include "Library.h"

using namespace std;


class Window {
private:
    
    void Error(const char* error) {
        cout << "\033[3;40;31m";
        cout << error;
        cout << "\033[0m";
    }

    ImGuiContext* context = ImGui::CreateContext();

    bool UseImGui = false;
    bool UseFramebuffer = false;
public:
    unsigned int framebuffer, textureColorbuffer, rbo;
    int width, height;
    GLFWwindow* window;


    inline    bool            Create(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
    inline    bool            Open();
    inline    bool            ReshapeWindow();
    inline    void            ConfigImGui();
    inline    void            Begin(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    inline    void            End();
    inline    void            ShutDown();
    inline    bool            getKey(int key);
    inline    const char*     getKeyName(int key, int scancode);
    inline    bool            getMouseButton(int key);
    inline    void            getMouseCursorPos(double* x, double* y);
    inline    void            setIcon(const char* icon);

    inline    void            ConfigFrameBuffer();
    inline    void            BeginFrameBuffer(float r = 0, float g = 0, float b = 0, float a = 1);
    inline    void            EndFrameBuffer();
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

        //ImGui::GetStyle().FramePadding = ImVec2(10, 10);
        ImGui::GetStyle().ScrollbarRounding = 0;


    }
    inline    void            Window::Begin(float r, float g, float b, float a) {
        glfwMakeContextCurrent(window);
        if (!UseFramebuffer) {
            glClearColor(r, g, b, a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
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
    inline    void            Window::setIcon(const char* icon) {
        GLFWimage image;
        image.pixels = stbi_load(icon, &image.width, &image.height, 0, 4);
        glfwSetWindowIcon(window, 1, &image);
        stbi_image_free(image.pixels);
    }

    inline    void            Window::ConfigFrameBuffer() {

        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    inline    void            Window::BeginFrameBuffer(float r, float g, float b, float a) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    inline    void            Window::EndFrameBuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
