#include "Engine/Window.h"
#include "Engine/Model.h"
#include "Engine/Object.h"
#include "Engine/Materials.h"

#define PI 3.14159265359
#define uint unsigned int
using namespace std;

void key(GLFWwindow* window, int key, int scanmode, int state, int mods);



float cnt = 90;
float xcenter = cos(cnt * PI / 180);
float zcenter = sin(cnt * PI / 180);
float ycenter = 5;
float xeye = 0, yeye = 5, zeye = 0;
float speed = 0.5;

int main() {
    Window window;
    window.Create(1200, 700, "Engine", nullptr, nullptr);
    window.setIcon("Textures/image.jpg");
    window.ConfigImGui();
    glfwSetKeyCallback(window.window, key);
    float width, height;
    width = window.width;
    height = window.height;
    
   

    Shader shader("Shaders/VLightCasters.shader", "Shaders/FLightCasters.shader");

    Model mapa("Models/Map/player.obj");



    float xlightpos = 0, ylightpos = 0, zlightpos = 0;
    float xdir = 0, ydir = 0, zdir = -1;
    float cutOff = 12, outerCutOff = 17;
    float xamb = 0.1, yamb = 0.1, zamb = 0.1;
    float xdiff = 0.8, ydiff = 0.8, zdiff = 0.8;
    float xspec = 1.0, yspec = 1.0, zspec = 1.0;
    float constant = 1.0, linearr = 0, quadratic = 0;

    while (window.Open()) {
        window.Begin();
        width = window.width;
        height = window.height;

        if (window.getKey(GLFW_KEY_A)) {
            cnt-=1.5;
            xcenter = cos(cnt * PI / 180);
            zcenter = sin(cnt * PI / 180);
        }
        if (window.getKey(GLFW_KEY_D)) {
            cnt+=1.5;
            xcenter = cos(cnt * PI / 180);
            zcenter = sin(cnt * PI / 180);
        }
        if (window.getKey(GLFW_KEY_W)) {
            xeye += xcenter * speed;
            zeye += zcenter * speed;
        }
        if (window.getKey(GLFW_KEY_S)) {
            xeye -= xcenter * speed;
            zeye -= zcenter * speed;
        }






        for (unsigned int i = 0; i < mapa.meshes.size(); ++i) {
            glUseProgram(shader.ID);


            glm::mat4 projection = glm::perspective<float>(glm::radians(85.0), width / height, 0.1, 1000);
            shader.setMat4("projection", projection);


            glm::mat4 camera = glm::lookAt<float>(glm::vec3(xeye, yeye, zeye), glm::vec3(xcenter + xeye, ycenter, zcenter + zeye), glm::vec3(0, 1, 0));
            shader.setMat4("view", camera);

            glm::mat4 model(1.0);
            //model = glm::ortho<float>(0, width, 0, height, width, 0);
            //model = glm::translate<float>(model, glm::vec3(width / 2, height / 2, 0));
            //model = glm::rotate<float>(model, glm::radians(glfwGetTime() * 50), glm::vec3(1, 0, 0));
            //model = glm::scale<float>(model, glm::vec3(100, 100, 100));
            shader.setMat4("model", model);

            shader.setInt("material.diffuse", 0);
            shader.setInt("material.specular", 1);

            shader.setFloat("material.shininess", 32);

            shader.setVec3("light.position", xeye, yeye, zeye);
            shader.setVec3("light.direction", xcenter, yeye - ycenter - 0.1, zcenter);
            shader.setFloat("light.cutOff", glm::cos(glm::radians(cutOff)));
            shader.setFloat("light.outerCutOff", glm::cos(glm::radians(outerCutOff)));

            shader.setVec3("light.ambient", xamb, yamb, zamb);
            shader.setVec3("light.diffuse", xdiff, ydiff, zdiff);
            shader.setVec3("light.specular", xspec, yspec, zspec);

            shader.setFloat("light.constant", constant);
            shader.setFloat("light.linearr", linearr);
            shader.setFloat("light.quadratic", quadratic);

            shader.setVec3("viewPos", xeye, yeye, zeye);

            
            mapa.meshes[i].Draw(shader);
            glUseProgram(0);
        }        
    







        ImGui::Begin("Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);


        if (ImGui::BeginChild("Camera pos", { 0, 210 }, true)) {
            ImGui::Text("Eye");
            ImGui::InputFloat("Xeye", &xeye, 1.0f, 1.0f);
            ImGui::InputFloat("Yeye", &yeye, 1.0f, 1.0f);
            ImGui::InputFloat("Zeye", &zeye, 1.0f, 1.0f);
            ImGui::NewLine();
            ImGui::Text("Center");
            ImGui::InputFloat("Xcenter", &xcenter, 1.0f, 1.0f);
            ImGui::InputFloat("Ycenter", &ycenter, 1.0f, 1.0f);
            ImGui::InputFloat("Zcenter", &zcenter, 1.0f, 1.0f);
        }
        ImGui::EndChild();

        /*
        ImGui::Text("Light pos");
        ImGui::InputFloat("Xpos", &xlightpos, 1.0f, 1.0f);
        ImGui::InputFloat("Ypos", &ylightpos, 1.0f, 1.0f);
        ImGui::InputFloat("Zpos", &zlightpos, 1.0f, 1.0f);

        ImGui::Text("Light direction");
        ImGui::InputFloat("Xdir", &xdir, 1.0f, 1.0f);
        ImGui::InputFloat("Ydir", &ydir, 1.0f, 1.0f);
        ImGui::InputFloat("Zdir", &zdir, 1.0f, 1.0f);
        */
        ImGui::Text("Light cut off");
        ImGui::InputFloat("Cut off", &cutOff, 0.1, 0.1);

        ImGui::Text("Light outer cut off");
        ImGui::InputFloat("outer cut off", &outerCutOff, 0.1, 0.1);

        ImGui::Text("Light ambient");
        ImGui::InputFloat("Xamb", &xamb, 0.1f, 0.1f);
        ImGui::InputFloat("Yamb", &yamb, 0.1f, 0.1f);
        ImGui::InputFloat("Zamb", &zamb, 0.1f, 0.1f);

        ImGui::Text("Light diffuse");
        ImGui::InputFloat("Xdiff", &xdiff, 0.1f, 0.1f);
        ImGui::InputFloat("Ydiff", &ydiff, 0.1f, 0.1f);
        ImGui::InputFloat("Zdiff", &zdiff, 0.1f, 0.1f);

        ImGui::Text("Light specular");
        ImGui::InputFloat("Xspec", &xspec, 0.1f, 0.1f);
        ImGui::InputFloat("Yspec", &yspec, 0.1f, 0.1f);
        ImGui::InputFloat("Zspec", &zspec, 0.1f, 0.1f);

        ImGui::Text("Light constant");
        ImGui::InputFloat("constant", &constant, 0.01, 0.01);

        ImGui::Text("Light linear");
        ImGui::InputFloat("linear", &linearr, 0.01, 0.01);

        ImGui::Text("Light quadratic");
        ImGui::InputFloat("quadratic", &quadratic, 0.01, 0.01);

        ImGui::End();



        window.End();
    }
    window.ShutDown();
    return 0;
}


void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) {
        exit(0);
    }
}