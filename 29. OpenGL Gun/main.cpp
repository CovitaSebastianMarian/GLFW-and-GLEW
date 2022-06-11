#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <thread>
#include "Shader.h"
#include "error.h"
#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <vector>
#include "Form.h"
#define PI 3.14159265359


using namespace std;


double wwindowsize = 1200 , hwindowsize = 700;
double xmouse, ymouse;
double angle = 0;
double gunxpos = wwindowsize / 2, gunypos = hwindowsize / 2;
double gunxsize = 200, gunysize = 40;
int pointtobuildgun = 3;
double xdif, ydif;
bool texturechange = false;
double normalangle = angle * 180 / PI;

//key
bool W_KEY = false, S_KEY = false, A_KEY = false, D_KEY = false;
double gunmovespeed = 10;
Object* gun;



class Bullet {
private:
    Object* glont;
public:
    double xpos, ypos;
    double speed = 100;
    double glontxsize = 20, glontysize = 10;
    double xlat, ylat;

    void Create() {

        xlat = xdif / sqrt(pow(xdif, 2) + pow(ydif, 2));
        ylat = ydif / sqrt(pow(ydif, 2) + pow(xdif, 2));
        xpos = gunxpos + xlat * (gunxsize - glontxsize);
        ypos = gunypos + ylat * (gunxsize - glontysize);

        glont = new Object;

        glont->VectorPositions.push_back(-glontxsize / 2);
        glont->VectorPositions.push_back(-glontysize / 2);

        glont->VectorPositions.push_back(0);
        glont->VectorPositions.push_back(0);

        glont->VectorPositions.push_back(glontxsize / 2);
        glont->VectorPositions.push_back(-glontysize / 2);

        glont->VectorPositions.push_back(1);
        glont->VectorPositions.push_back(0);

        glont->VectorPositions.push_back(glontxsize / 2);
        glont->VectorPositions.push_back(glontysize / 2);

        glont->VectorPositions.push_back(1);
        glont->VectorPositions.push_back(1);

        glont->VectorPositions.push_back(-glontxsize / 2);
        glont->VectorPositions.push_back(glontysize / 2);

        glont->VectorPositions.push_back(0);
        glont->VectorPositions.push_back(1);

        glont->DynamicInit(GL_DYNAMIC_DRAW);
        glont->VertexAttribpointer(0, 0, 2, 4, GL_FLOAT);
        glont->VertexAttribpointer(1, 2, 2, 4, GL_FLOAT);
        glont->Shader("Shaders/Basic.shader");
        glont->Texture("Textures/bullet.png", GL_RGBA, texturechange);
        glont->Ortho(0, wwindowsize, hwindowsize, 0, 1, -1);
        glont->Translate(glm::vec3(xpos, ypos - 8, 0), false);
        glont->Rotate(angle, glm::vec3(0, 0, 1), false);
    }


    void Draw() {
        glont->Translate(glm::vec3(speed * xlat, speed * ylat, 0), true);
        xpos += speed * xlat;
        ypos += speed * ylat;
        glont->Bind(GL_QUADS, 0, 4);
    }


} bl;
vector<Bullet> bullet;


void Config() {
    if (W_KEY) {
        gunypos -= gunmovespeed;
    }
    if (S_KEY) {
        gunypos += gunmovespeed;
    }
    if (A_KEY) {
        gunxpos -= gunmovespeed;
    }
    if (D_KEY) {
        gunxpos += gunmovespeed;
    }
    angle = atan2(ymouse - gunypos, xmouse - gunxpos);
    xdif = xmouse - gunxpos;
    ydif = ymouse - gunypos;
}




void reshape(GLFWwindow* window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    if (w != wwindowsize || h != hwindowsize) {
        wwindowsize = w;
        hwindowsize = h;
        glViewport(0, 0, wwindowsize, hwindowsize);
        gun->Ortho(0, wwindowsize, hwindowsize, 0, 1, -1);
    }
}

void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);
    
    if (key == GLFW_KEY_W)
        if (state == GLFW_PRESS) W_KEY = true;
        else if (state == GLFW_RELEASE) W_KEY = false;

    if (key == GLFW_KEY_S)
        if (state == GLFW_PRESS) S_KEY = true;
        else if (state == GLFW_RELEASE) S_KEY = false;

    if (key == GLFW_KEY_A)
        if (state == GLFW_PRESS) A_KEY = true;
        else if (state == GLFW_RELEASE) A_KEY = false;

    if (key == GLFW_KEY_D)
        if (state == GLFW_PRESS) D_KEY = true;
        else if (state == GLFW_RELEASE) D_KEY = false;

}

void mouse(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        bl.Create();
        bullet.push_back(bl);
    }
}
void cursor(GLFWwindow* window, double x, double y) {
    xmouse = x;
    ymouse = y;
    angle = atan2(y - gunypos, x - gunxpos);
    xdif = x - gunxpos;
    ydif = y - gunypos;
}



int main()
{
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window;
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(wwindowsize, hwindowsize, "GLFW and GLEW", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    if (glewInit() != GLEW_OK) {
        cout << "\033[3;40;31m[Error: glew =try> move glewinit() after window init]" << endl;
        return -1;
    }
    glfwSetKeyCallback(window, key);
    glfwSetCursorPosCallback(window, cursor);
    glfwSetMouseButtonCallback(window, mouse);
    glfwSetWindowRefreshCallback(window, reshape);

    GLCall(glEnable(GL_POINT_SMOOTH));
    GLCall(glHint(GL_POINT_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_LINE_SMOOTH));
    GLCall(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_POLYGON_SMOOTH));
    GLCall(glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));


    


    gun = new Object;
    gun->VectorPositions.push_back(-gunxsize / pointtobuildgun);
    gun->VectorPositions.push_back(-gunysize / 2);

    gun->VectorPositions.push_back(0);
    gun->VectorPositions.push_back(0);

    gun->VectorPositions.push_back((pointtobuildgun - 1) * gunxsize / pointtobuildgun);
    gun->VectorPositions.push_back(-gunysize / 2);

    gun->VectorPositions.push_back(1);
    gun->VectorPositions.push_back(0);

    gun->VectorPositions.push_back((pointtobuildgun - 1) * gunxsize / pointtobuildgun);
    gun->VectorPositions.push_back(gunysize / 2);

    gun->VectorPositions.push_back(1);
    gun->VectorPositions.push_back(1);

    gun->VectorPositions.push_back(-gunxsize / pointtobuildgun);
    gun->VectorPositions.push_back(gunysize / 2);

    gun->VectorPositions.push_back(0);
    gun->VectorPositions.push_back(1);
    gun->DynamicInit(GL_DYNAMIC_DRAW);
    gun->VertexAttribpointer(0, 0, 2, 4, GL_FLOAT);
    gun->VertexAttribpointer(1, 2, 2, 4, GL_FLOAT);
    gun->Shader("Shaders/Basic.shader");
    gun->Texture("Textures/gun.png", GL_RGBA, false);
    gun->Ortho(0, wwindowsize, hwindowsize, 0, 1, -1);
    gun->Translate(glm::vec3(gunxpos, gunypos, 0), false);



    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        Config();


        normalangle = angle * 180 / PI;
        if (normalangle  > -90 && normalangle < 90 && texturechange) {
            gun->Texture("Textures/gun.png", GL_RGBA, false);
            texturechange = false;
        }
        else if((normalangle < -90 || normalangle > 90) && !texturechange) {
            gun->Texture("Textures/gun.png", GL_RGBA, true);
            texturechange = true;
        }


        gun->Translate(glm::vec3(gunxpos, gunypos, 0), false);
        gun->Rotate(angle, glm::vec3(0, 0, 1), false);
        gun->Bind(GL_QUADS, 0, 4);

        for (size_t i = 0; i < bullet.size(); ++i) {
            if (bullet[i].xpos < 0 || bullet[i].xpos > wwindowsize || bullet[i].ypos < 0 || bullet[i].ypos > hwindowsize) {
                bullet.erase(bullet.begin() + i);
            }
            else bullet[i].Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    exit(0);
    glfwTerminate();
    return 0;
}
