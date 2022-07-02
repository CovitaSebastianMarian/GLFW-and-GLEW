#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <math.h>
#include <thread>
#include "Shader.h"
#include "error.h"
#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <vector>
#include "Game.h"

#define PI 3.14159265359


using namespace std;

//dimensiunile ferestrei de joc
double wwindowsize = 1200, hwindowsize = 700;

//de ele ne folosim pentu a stii coordonatele mouse-lui
double xmouse = 0, ymouse = 0;

//contorizeaza cand se apasa una dintre tastele a,w,s,d
bool KEY_W = false, KEY_S = false, KEY_A = false, KEY_D = false;

//viteza caracterului de joc
int speed = 2;

//viteza glontului
float glontspeed = 60;

/*contorizam cand este apasat click dreapta si astfel in timul ala
*trebuie sa se creieze gloante noi*/
bool newglont = false;

//din cat in cat sa se creieze cate un glont
int delayszie = 10;

//de fiecare data cand delay = delaysize se creaza un glont si dupa primeste 0 si creste o data cu apelarea functiei
int delay = delayszie;

//creiem obiectele
Map* harta = new Map;
Character* person = new Character;
Character* bratstang = new Character;
Character* bratdrept = new Character;
Glont glont;
//in acest vector stocam toate gloantele creiate pentru a le desena
vector<Glont> VectorGloante;

//creiem setarile la obiecte
Settings* hartasett = new Settings;
Settings* personsett = new Settings;
Settings* bratstangsett = new Settings;
Settings* bratdreptsett = new Settings;
Settings glontsett;



//configuram setarile obiectelor si le aribuil lor
void GameConfiguration() {
    //Configuram glontul
    glontsett.fliptexture = true;
    glontsett.wwindowsize = wwindowsize;
    glontsett.hwindowsize = hwindowsize;
    //glontsett.texture = "Textures/bullet.png";
    glontsett.xsize = 10;// max(wwindowsize, hwindowsize) / 50;
    glontsett.ysize = 2;// min(wwindowsize, hwindowsize) / 50;
    //glont.PutsTexture("Textures/bullet.png", true);


    hartasett->wwindowsize = wwindowsize;
    hartasett->hwindowsize = hwindowsize;
    hartasett->texture = "Textures/map.png";
    hartasett->xpos = 1000;
    hartasett->ypos = 1000;
    hartasett->xsize = 2000;
    hartasett->ysize = 2000;
    hartasett->xorigin = 0;
    hartasett->yorigin = 0;
    harta->CreateMap(hartasett);

    personsett->wwindowsize = wwindowsize;
    personsett->hwindowsize = hwindowsize;
    personsett->texture = "Textures/img4.png";
    personsett->xpos = wwindowsize / 2;
    personsett->ypos = hwindowsize / 2;
    personsett->xsize = min(wwindowsize, hwindowsize) / 10;
    personsett->ysize = min(wwindowsize, hwindowsize) / 10;
    personsett->xorigin = 0;
    personsett->yorigin = 0;

    bratstangsett->wwindowsize = wwindowsize;
    bratstangsett->hwindowsize = hwindowsize;
    bratstangsett->texture = "Textures/gun.png";
    bratstangsett->xpos = wwindowsize / 2 - max(wwindowsize, hwindowsize) / 40;
    bratstangsett->ypos = hwindowsize / 2;
    bratstangsett->xsize = max(wwindowsize, hwindowsize) / 15;
    bratstangsett->ysize = min(wwindowsize, hwindowsize) / 30;
    bratstangsett->xorigin = -max(wwindowsize, hwindowsize) / 30;
    bratstangsett->yorigin = 0;

    bratdreptsett->wwindowsize = wwindowsize;
    bratdreptsett->hwindowsize = hwindowsize;
    bratdreptsett->texture = "Textures/gun.png";
    bratdreptsett->xpos = wwindowsize / 2 + max(wwindowsize, hwindowsize) / 40;
    bratdreptsett->ypos = hwindowsize / 2;
    bratdreptsett->xsize = max(wwindowsize, hwindowsize) / 15;
    bratdreptsett->ysize = min(wwindowsize, hwindowsize) / 30;
    bratdreptsett->xorigin = -max(wwindowsize, hwindowsize) / 30;
    bratdreptsett->yorigin = 0;


    person->CreateCharacter(personsett);
    bratstang->CreateCharacter(bratstangsett);
    bratdrept->CreateCharacter(bratdreptsett);
}

/*
* Calculam unghiul la care sa se roteasca armele si le rotim in x (pe orizontala) 180 pentru a nu parea cu susul in jos
*/
double CalculateAngle(Character * chr, Settings* sett) {
    double newangle = atan2(ymouse - sett->ypos, xmouse - sett->xpos);

    if (newangle * 180 / PI > -90 && newangle * 180 / PI < 90) {
        chr->RotateTexture(0);
        return newangle;
    }
    else if((newangle * 180 / PI < -90 || newangle * 180 / PI > 90)) {
        chr->RotateTexture(180);
        return newangle;
    }
   
}

bool InWindowY(float y) {
    return y >= 0 && y <= hwindowsize;
}
bool InWindowX(float x) {
    return x >= 0 && x <= wwindowsize;
}


//initializam la fiecare randare a ferestrei
void Init() {
    bratstangsett->angle = CalculateAngle(bratstang, bratstangsett);
    bratdreptsett->angle = CalculateAngle(bratdrept, bratdreptsett);
    if (KEY_W) {
        if (hartasett->yview - speed + hartasett->ypos - hwindowsize/2 >= -(hartasett->ysize - hwindowsize) / 2 && personsett->ypos >= hwindowsize / 2) {
            hartasett->yview -= speed;
        }
        else if (InWindowY(personsett->ypos + speed + (personsett->ysize / 2 - personsett->yorigin))) {
            personsett->ypos += speed;
            bratstangsett->ypos += speed;
            bratdreptsett->ypos += speed;
        }
    }
    if (KEY_S) {
        if (hartasett->yview + speed + hartasett->ypos - hwindowsize/2 <= (hartasett->ysize - hwindowsize) / 2 && personsett->ypos <= hwindowsize / 2) {
            hartasett->yview += speed;
        }
        else if (InWindowY(personsett->ypos - speed - (personsett->ysize / 2 + personsett->yorigin))) {
            personsett->ypos -= speed;
            bratstangsett->ypos -= speed;
            bratdreptsett->ypos -= speed;
        }
    }
    if (KEY_A) {
        if (hartasett->xview + speed + hartasett->xpos - wwindowsize/2 <= (hartasett->xsize - wwindowsize) / 2 && personsett->xpos <= wwindowsize / 2) {
            hartasett->xview += speed;
        }
        else if (InWindowX(bratstangsett->xpos - speed) && InWindowX(bratdreptsett->xpos - speed)) {
            personsett->xpos -= speed;
            bratstangsett->xpos -= speed;
            bratdreptsett->xpos -= speed;
        }
    }
    if (KEY_D) {
        if (hartasett->xview - speed + hartasett->xpos - wwindowsize/2 >= -(hartasett->xsize - wwindowsize) / 2 && personsett->xpos >= wwindowsize / 2) {
            hartasett->xview -= speed;
        }
        else if (InWindowX(bratstangsett->xpos + speed) && InWindowX(bratdreptsett->xpos + speed)) {
            personsett->xpos += speed;
            bratstangsett->xpos += speed;
            bratdreptsett->xpos += speed;
        }
    }

    if (newglont) {
        if (delay == delayszie) {
            //Glont stang
            glontsett.angle = bratstangsett->angle;
            glontsett.xpos = bratstangsett->xpos;
            glontsett.ypos = bratstangsett->ypos;
            glont.xpos = bratstangsett->xpos;
            glont.ypos = bratstangsett->ypos;

            float xdif = xmouse - bratstangsett->xpos;
            float ydif = ymouse - bratstangsett->ypos;

            float xlat = xdif / sqrt(pow(xdif, 2) + pow(ydif, 2));
            float ylat = ydif / sqrt(pow(ydif, 2) + pow(xdif, 2));

            glont.xspeed = glontspeed * xlat;
            glont.yspeed = glontspeed * ylat;

            glont.CreateGlont(glontsett);
            VectorGloante.push_back(glont);


            //Glont drept
            glontsett.angle = bratdreptsett->angle;
            glontsett.xpos = bratdreptsett->xpos;
            glontsett.ypos = bratdreptsett->ypos;
            glont.xpos = bratdreptsett->xpos;
            glont.ypos = bratdreptsett->ypos;

            xdif = xmouse - bratdreptsett->xpos;
            ydif = ymouse - bratdreptsett->ypos;

            xlat = xdif / sqrt(pow(xdif, 2) + pow(ydif, 2));
            ylat = ydif / sqrt(pow(ydif, 2) + pow(xdif, 2));

            glont.xspeed = glontspeed * xlat;
            glont.yspeed = glontspeed * ylat;

            glont.CreateGlont(glontsett);
            VectorGloante.push_back(glont);
            delay = 0;
        }
        else delay++;
    }
}

//restabilim toate variabilele care depind de dimensiunile ferestrei
void reshape(GLFWwindow* window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    if (w != wwindowsize || h != hwindowsize) {
        float antw = w - wwindowsize;
        float anth = h - hwindowsize;
        wwindowsize = w;
        hwindowsize = h;
        glViewport(0, 0, wwindowsize, hwindowsize);

        personsett->wwindowsize = wwindowsize;
        personsett->hwindowsize = hwindowsize;
        person->ReConfig(personsett);

        bratstangsett->wwindowsize = wwindowsize;
        bratstangsett->hwindowsize = hwindowsize;
        bratstang->ReConfig(bratstangsett);
        
        bratdreptsett->wwindowsize = wwindowsize;
        bratdreptsett->hwindowsize = hwindowsize;
        bratdrept->ReConfig(bratdreptsett);

        glontsett.wwindowsize = wwindowsize;
        glontsett.hwindowsize = hwindowsize;

        hartasett->wwindowsize = wwindowsize;
        hartasett->hwindowsize = hwindowsize;
        //hartasett->xpos = wwindowsize / 2;
        //hartasett->ypos = hwindowsize / 2;
        hartasett->xview = antw / 2;
        hartasett->yview = anth / 2;

    }
}

//key press
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

    if (key == GLFW_KEY_A) {
        if (state == GLFW_PRESS) {
            KEY_A = true;
        }
        else if (state == GLFW_RELEASE) {
            KEY_A = false;
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

}
//mouse press
void mouse(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            newglont = true;
            delay = delayszie;
        }
        else if (action == GLFW_RELEASE) newglont = false;
    }
}
//cursor move
void cursor(GLFWwindow* window, double x, double y) {
    xmouse = x;
    ymouse = hwindowsize - y;
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
    //GLCall(glEnable(GL_DEPTH_TEST));
    

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    


    GameConfiguration();

   
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Imgui window");
        ImGui::SetWindowSize({ (float)wwindowsize / 4, (float)hwindowsize });
        ImGui::SetWindowPos({ 0, 0 });
        ImGui::End();

        Init();

        //desenam obiectele pe ecran
        harta->DrawMap(hartasett);
        person->DrawCharacter(personsett);
        for (size_t i = 0; i < VectorGloante.size(); ++i) {
            //daca un glont este inafara hartii il stergem, daca nu il desenam
            if (VectorGloante[i].xpos < 0 || VectorGloante[i].xpos > wwindowsize || VectorGloante[i].ypos < 0 || VectorGloante[i].ypos > hwindowsize) {
                VectorGloante.erase(VectorGloante.begin() + i);
            }
            else VectorGloante[i].DrawGlont();
        }
        bratstang->DrawCharacter(bratstangsett);
        bratdrept->DrawCharacter(bratdreptsett);



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    exit(0);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}