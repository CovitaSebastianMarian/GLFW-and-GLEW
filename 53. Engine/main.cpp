#include "engine/seb.hpp"
#include "engine/utility.hpp"
#include "engine/2D.hpp"
#include "engine/3D.hpp"
#include "engine/model.hpp"
#include "engine/window.hpp"
#include "game/game.hpp"



seb::FrameBuffer framebuffer;
void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    framebuffer.resize(width, height);
}

int main() {
    seb::Window window;
    window.create(1200, 700, "GLEW Example");

    

    Casa casa;
    casa.create();


    framebuffer.open(1200, 700);
    seb::Shader framebuffer_shader;
    framebuffer_shader.open("game/shaders/fb.vert", "game/shaders/fb.frag");
    seb::Quad3D quad;
    quad.create();
    
    float val = 0;
    window.set_window_size_callback(window_size_callback);
    while (!window.should_close()) {
        framebuffer.bind();
        window.clear_color(0, 0.2, 0.5, 1);
        window.clear();

        casa.draw(window.get_width(), window.get_height());

        framebuffer.unbind();


        framebuffer.resolve(window.get_width(), window.get_height());


        if(glfwGetKey(window.window, GLFW_KEY_A) == GLFW_PRESS) val -= 0.01f;
        if(glfwGetKey(window.window, GLFW_KEY_D) == GLFW_PRESS) val += 0.01f;

        window.clear_color(0, 0, 0, 1);
        window.clear();

        framebuffer_shader.bind();
        framebuffer.bind_texture();
        framebuffer_shader.set_int("texture0", 0);
        framebuffer_shader.set_mat4("model", quad.get_transform_matrix());
        framebuffer_shader.set_mat4("projection", quad.get_perspective_matrix(90, window.get_width() / window.get_height(), 0.1f, 100.f));
        framebuffer_shader.set_mat4("view", quad.get_view_matrix(glm::vec3(sin(val) * 3, 0, cos(val) * 3), glm::vec3(0, 0, 0)));
        quad.draw();
        framebuffer_shader.unbind();


        window.use_imgui([&]() {
            ImGui::Begin("Console");
            ImGui::InputFloat("Scale", &casa.scale, 1);
            ImGui::InputFloat("X Angle", &casa.xangle, 1);
            ImGui::InputFloat("Y Angle", &casa.yangle, 1);
            ImGui::InputFloat("Z Angle", &casa.zangle, 1);
            ImGui::InputFloat("X Position", &casa.xpos, 1);
            ImGui::InputFloat("Y Position", &casa.ypos, 1);
            ImGui::InputFloat("Z Position", &casa.zpos, 1);
            ImGui::End();
            });

        window.swap_buffers();
        window.poll_events();
    }
    window.close();

    return 0;
}





/*
int WindowWidth = 800, WindowHeight = 800;
int xmousepos, ymousepos;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    WindowWidth = width;
    WindowHeight = height;
    glViewport(0, 0, width, height);
    resize(WindowWidth, WindowHeight);
}
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    xmousepos = xpos;
    ymousepos = ypos;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void seb_imgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //#############################################################################


    ImGui::Begin("Console");
    ImGui::InputFloat("Scale", &casa.scale, 1);
    ImGui::InputFloat("X Angle", &casa.xangle, 1);
    ImGui::InputFloat("Y Angle", &casa.yangle, 1);
    ImGui::InputFloat("Z Angle", &casa.zangle, 1);
    ImGui::InputFloat("X Position", &casa.xpos, 1);
    ImGui::InputFloat("Y Position", &casa.ypos, 1);
    ImGui::InputFloat("Z Position", &casa.zpos, 1);
    ImGui::End();



    //#############################################################################
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    //glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "GLEW Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetKeyCallback(window, key_callback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }


    glEnable(GL_DEPTH_TEST);





    init(WindowWidth, WindowHeight);


    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw(WindowWidth, WindowHeight, xpos, zpos);

        seb_imgui();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
*/