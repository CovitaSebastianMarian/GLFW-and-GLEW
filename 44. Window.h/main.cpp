#include "Window.h"

#define PI 3.14159265359

using namespace std;


void key(GLFWwindow* window, int key, int scanmode, int state, int mods);




int main() {
    Window window;
    window.Create(1200, 700, "dsadsa", nullptr, nullptr);
    window.ConfigImGui();
    glfwSetKeyCallback(window.window, key);

    int width, height;
    width = window.width;
    height = window.height;



    vector<float> pos;

    while (window.Open()) {
        window.Begin();
        width = window.width;
        height = window.height;
        
        if (window.getMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
            double x, y;
            glfwGetCursorPos(window.window, &x, &y);
            y = height - y;
            if (x >= 0 && x <= width && y >= 0 && y <= height) {
                pos.push_back(x);
                pos.push_back(y);
                window.InitPixels(pos);
            }
        }
        
       
        window.DrawPixels(pos, 1, 0, 0, 1);
        

        
        
        

       




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