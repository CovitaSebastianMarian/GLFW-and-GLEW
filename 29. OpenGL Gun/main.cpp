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
double angle = 0;
double gunxpos = wwindowsize / 2, gunypos = hwindowsize / 2;
double xmouse = 0, ymouse = 0;
bool play = false;
double gunxsize = 100, gunysize = 20;

class Bullet {
private:
    double xsize = 10;
    double ysize = 5;
    double xmove = 10, ymove = 10;

    double xlat, ylat;
    double xdif, ydif;

    Object* glont;
public:
    double xpos, ypos;
    void Create() {
        glont = new Object;
        xdif = xmouse - gunxpos;
        ydif = ymouse - gunypos;
        xlat = xdif / sqrt(pow(xdif, 2) + pow(ydif, 2));
        ylat = ydif / sqrt(pow(ydif, 2) + pow(xdif, 2));
        xpos = gunxpos;
        ypos = gunypos;


        glont->VectorPositions.push_back(-xsize / 2);
        glont->VectorPositions.push_back(-ysize / 2);
        glont->VectorPositions.push_back(0);

        glont->VectorPositions.push_back(0);
        glont->VectorPositions.push_back(0);

        glont->VectorPositions.push_back(xsize / 2);
        glont->VectorPositions.push_back(-ysize / 2);
        glont->VectorPositions.push_back(0);

        glont->VectorPositions.push_back(1);
        glont->VectorPositions.push_back(0);

        glont->VectorPositions.push_back(xsize / 2);
        glont->VectorPositions.push_back(ysize / 2);
        glont->VectorPositions.push_back(0);

        glont->VectorPositions.push_back(1);
        glont->VectorPositions.push_back(1);

        glont->VectorPositions.push_back(-xsize / 2);
        glont->VectorPositions.push_back(ysize / 2);
        glont->VectorPositions.push_back(0);

        glont->VectorPositions.push_back(0);
        glont->VectorPositions.push_back(1);

        glont->DynamicInit(GL_DYNAMIC_DRAW);
        glont->VertexAttribpointer(0, 0, 3, 5, GL_FLOAT);
        glont->VertexAttribpointer(1, 3, 2, 5, GL_FLOAT);
        glont->Shader("Shaders/Basic.shader");
        glont->Texture("Textures/image.jpg", GL_RGB, false);
        glont->Ortho(0, wwindowsize, hwindowsize, 0, 1, -1);
        glont->Rotate(angle, glm::vec3(0, 0, 1), false);
        glont->Translate(glm::vec3(xpos + xlat * (gunxsize - xsize * 2), ypos + ylat * (gunxsize - xsize * 2), 0), false);
    }

    void Draw() {
        if (play) {
            glont->Translate(glm::vec3(xmove * xlat, ymove * ylat, 0), true);
            xpos += xmove * xlat;
            ypos += ymove * ylat;
        }
        glont->Bind(GL_QUADS, 0, 4);
    }

} bl;
vector<Bullet> bullet;



void reshape(GLFWwindow* window)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    if (w != wwindowsize || h != hwindowsize) {
        wwindowsize = w;
        hwindowsize = h;
        glViewport(0, 0, wwindowsize, hwindowsize);
    }
}

void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
    if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);
    if (key == GLFW_KEY_SPACE && state == GLFW_PRESS) {
        if (play) play = false;
        else play = true;
    }
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
    angle = atan2(ymouse - gunypos, xmouse - gunxpos);
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


    

    
    Object* gun = new Object;
    gun->VectorPositions.push_back(-gunxsize / 5);
    gun->VectorPositions.push_back(-gunysize / 2);
    gun->VectorPositions.push_back(0);

    gun->VectorPositions.push_back(0);
    gun->VectorPositions.push_back(0);

    gun->VectorPositions.push_back(4 * gunxsize / 5);
    gun->VectorPositions.push_back(-gunysize / 2);
    gun->VectorPositions.push_back(0);

    gun->VectorPositions.push_back(1);
    gun->VectorPositions.push_back(0);

    gun->VectorPositions.push_back(4 * gunxsize / 5);
    gun->VectorPositions.push_back(gunysize / 2);
    gun->VectorPositions.push_back(0);

    gun->VectorPositions.push_back(1);
    gun->VectorPositions.push_back(1);

    gun->VectorPositions.push_back(-gunxsize / 5);
    gun->VectorPositions.push_back(gunysize / 2);
    gun->VectorPositions.push_back(0);

    gun->VectorPositions.push_back(0);
    gun->VectorPositions.push_back(1);
    
    gun->DynamicInit(GL_DYNAMIC_DRAW);
    gun->VertexAttribpointer(0, 0, 3, 5, GL_FLOAT);
    gun->VertexAttribpointer(1, 3, 2, 5, GL_FLOAT);
    gun->Shader("Shaders/Basic.shader");
    gun->Texture("Textures/grass.jpg", GL_RGB, false);
    gun->Ortho(0, wwindowsize, hwindowsize, 0, 1, -1);
    gun->Translate(glm::vec3(gunxpos, gunypos, 0), false);





    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        gun->Translate(glm::vec3(gunxpos, gunypos, 0), false);
        gun->Rotate(angle, glm::vec3(0, 0, 1), false);
        gun->Bind(GL_QUADS, 0, 4);

        for (int i = 0; i < bullet.size(); ++i) {
            bullet[i].Draw();
            if (bullet[i].xpos < 0 || bullet[i].xpos > wwindowsize || bullet[i].ypos < 0 || bullet[i].ypos > hwindowsize) {
                bullet.erase(bullet.begin() + i);
            }
        }
        cout << bullet.size() << "\n";


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    exit(0);
    glfwTerminate();
    return 0;
}