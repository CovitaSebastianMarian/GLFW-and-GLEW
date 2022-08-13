#include "Engine/Window.h"
#include "Engine/Object.h"

#define PI 3.14159265359
#define uint unsigned int
using namespace std;

void key(GLFWwindow* window, int key, int scanmode, int state, int mods);

vector<float> pos;
vector <Object> obj;

int main() {
    Window window;
    window.Create(1200, 700, "Engine", nullptr, nullptr);
    window.setIcon("Textures/grass.jpg");
    window.ConfigImGui();
    glfwSetKeyCallback(window.window, key);
    

    float SCR_WIDTH, SCR_HEIGHT;
    SCR_WIDTH = window.width;
    SCR_HEIGHT = window.height;

    
   

    Object obj;
    obj.vertices.push_back(100);
    obj.vertices.push_back(100);
    obj.vertices.push_back(400);
    obj.vertices.push_back(400);
    obj.vertices.push_back(800);
    obj.vertices.push_back(500);
    obj.ConfigDrawTool();
    

    unsigned int shader, VAO, VBO;

    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec2 tc;\n"
        "out vec2 tcoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 0, 1);\n"
        "   tcoord = tc;\n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "in vec2 tcoord;\n"
        "uniform sampler2D Texture;\n"
        "void main()\n"
        "{\n"
        "   gl_FragColor = vec4(texture(Texture, tcoord).rgb, 1);\n"
        "}\0";
  
    float vertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

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

    glUseProgram(shader);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //glBindVertexArray(0);



    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);








    while (window.Open()) {
        //window.Begin();
        SCR_WIDTH = window.width;
        SCR_HEIGHT = window.height;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
        glEnable(GL_DEPTH_TEST);
        
        obj.Draw(SCR_WIDTH, SCR_HEIGHT, GL_TRIANGLES, 1);



        // second pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        */
      
        
        ImGui::Begin("Window");
        //ImGui::GetWindowDrawList()->AddImage((ImTextureID)framebuffer, ImGui::GetWindowPos(), ImGui::GetWindowSize());
        ImGui::GetWindowDrawList()->AddImage((ImTextureID)textureColorbuffer, ImVec2(ImGui::GetCursorScreenPos()), ImVec2(ImGui::GetCursorScreenPos().x + SCR_WIDTH / 1.5, ImGui::GetCursorScreenPos().y + SCR_HEIGHT / 1.5), ImVec2(0, 0), ImVec2(1, 1));
        //ImGui::GetWindowDrawList()->AddCallback(framebuffer, nullptr);
        ImGui::End();
        
        


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.window);
        glfwPollEvents();
       
    }
    window.ShutDown();
    return 0;
}


void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) {
        exit(0);
    }
}