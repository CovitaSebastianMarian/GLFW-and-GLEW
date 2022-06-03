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
#define PI 3.14159265359


using namespace std;


double wwindowsize = 1200 , hwindowsize = 700;
bool mouse_cursor_is_visible = false;
double angle = 0;
double xmouse, ymouse, xcnt = 0, ycnt = 0;
double xgunpos = wwindowsize/2, ygunpos = hwindowsize / 2;
bool W_KEY = false, S_KEY = false, A_KEY = false, D_KEY = false;
const int gunmovementspeed = 5;

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

void Recalculated(int x, int y) {
    xcnt = x - xgunpos;
    ycnt = y - ygunpos;
    angle = atan2(ycnt, xcnt);
}

void key(GLFWwindow* window, int key, int scanmode, int state, int mods)
{
    //if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS) exit(0);
    if (key == GLFW_KEY_ESCAPE && state == GLFW_RELEASE) {
        if (!mouse_cursor_is_visible) {
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouse_cursor_is_visible = true;
        }
        else exit(0);
    }

    if (key == GLFW_KEY_W && state == GLFW_PRESS) {
        W_KEY = true;
    }
    else if (key == GLFW_KEY_W && state == GLFW_RELEASE) {
        W_KEY = false;
    }
    if (key == GLFW_KEY_S && state == GLFW_PRESS) {
        S_KEY = true;
    }
    else if (key == GLFW_KEY_S && state == GLFW_RELEASE) {
        S_KEY = false;
    }
    if (key == GLFW_KEY_D && state == GLFW_PRESS) {
        D_KEY = true;
    }
    else if (key == GLFW_KEY_D && state == GLFW_RELEASE) {
        D_KEY = false;
    }
    if (key == GLFW_KEY_A && state == GLFW_PRESS) {
        A_KEY = true;
    }
    else if (key == GLFW_KEY_A && state == GLFW_RELEASE) {
        A_KEY = false;
    }

    if (W_KEY) {
        ygunpos -= gunmovementspeed;
        Recalculated(xmouse, ymouse);
    }
    if (S_KEY) {
        ygunpos += gunmovementspeed;
        Recalculated(xmouse, ymouse);
    }
    if (D_KEY) {
        xgunpos += gunmovementspeed;
        Recalculated(xmouse, ymouse);
    }
    if (A_KEY) {
        xgunpos -= gunmovementspeed;
        Recalculated(xmouse, ymouse);
    }



    /*
    if (key == GLFW_KEY_W && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        ygunpos--;
        Recalculated(xmouse, ymouse);
    }
    if (key == GLFW_KEY_S && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        ygunpos++;
        Recalculated(xmouse, ymouse);
    }
    if (key == GLFW_KEY_D && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        xgunpos++;
        Recalculated(xmouse, ymouse);
    }
    if (key == GLFW_KEY_A && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
        xgunpos--;
        Recalculated(xmouse, ymouse);
    }
    */
}

void mouse(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouse_cursor_is_visible = false;
    }

}
void cursor(GLFWwindow* window, double x, double y) {
    if (!mouse_cursor_is_visible) {
       
        Recalculated(x, y);
        xmouse = x;
        ymouse = y;

        //glfwSetCursorPos(window, wwindowsize / 2, hwindowsize / 2);
    }
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

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    //glfwSetCursorPos(window, wwindowsize / 2, hwindowsize / 2);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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


    glm::mat4 camera(1.0);
    camera = glm::lookAt(glm::vec3(0, 1, 0), glm::vec3(0, 0 + ycnt, 5), glm::vec3(0, 1, 0));



    float xsze = 100;
    float ysze = 10;
    float pos[] = {
        -xsze / 10,    -ysze / 2,0,      0, 0,
        9 * xsze /10,  -ysze / 2,0,      1, 0,
        9 * xsze /10,  ysze/2,0,         1, 1,
        -xsze/10,      ysze/2, 0,        0, 1,
    };
    
    Object *obj = new Object;
    obj->Init(pos, sizeof(pos), GL_STATIC_DRAW);
    obj->VertexAttribpointer(0, 0, 3, 5, GL_FLOAT);
    obj->VertexAttribpointer(1, 3, 2, 5, GL_FLOAT);
    obj->Shader("Shaders/Basic.shader");
    obj->Texture("Textures/image.jpg", GL_RGB, true, GL_REPEAT, GL_REPEAT);
    obj->Ortho(0, wwindowsize, hwindowsize, 0, 100, -100);
    //obj->Perspective(glm::radians(30.0), hwindowsize/wwindowsize, 0.1, 100);
    //obj->LookAt(glm::vec3(0, 1, 0), glm::vec3(0, 0, 5), glm::vec3(0, 1, 0));
    //obj->camera = camera;
    obj->Translate(glm::vec3(xgunpos, ygunpos, 0), false);

    //glPointSize(45);

    GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    glfwSetCursor(window, cursor);

    Object* circle = new Object;
    float circle_pos[3600];
    int circlecnt = -1;
    double circle_size = 200;

    circle->VectorPositions.push_back(-circle_size);
    circle->VectorPositions.push_back(circle_size);
    circle->VectorPositions.push_back(0);

    circle->VectorPositions.push_back(circle_size);
    circle->VectorPositions.push_back(circle_size);
    circle->VectorPositions.push_back(0);

    circle->VectorPositions.push_back(0);
    circle->VectorPositions.push_back(-circle_size / 2);
    circle->VectorPositions.push_back(0);


    circle->DynamicInit(GL_STATIC_DRAW);
    circle->VertexAttribpointer(0, 0, 3, 3, GL_FLOAT);
    circle->Shader("Shaders/Basic.shader");
    //circle->Texture("Textures/image.jpg", GL_RGB, false);
    circle->Ortho(0, wwindowsize, hwindowsize, 0, -100, 100);
    circle->Translate(glm::vec3(wwindowsize / 2, hwindowsize / 2, 0), false);


    double gangle = angle * 180 / PI;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //obj->Translate(glm::vec3(xgunpos, ygunpos, 0), false);
        //obj->Rotate(angle, glm::vec3(0, 0, 1), false);
        //obj->Bind(GL_QUADS, 0, 4);


        circle->Bind(GL_TRIANGLES, 0, 3);












        


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    exit(0);
    glfwTerminate();
    return 0;
}