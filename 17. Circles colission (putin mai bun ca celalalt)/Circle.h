#pragma once
#include "Object.h"
#define PI 3.1415926535
#include <vector>
#include <map>

/*
class Circle
{
private:
	Object circle;
    int w, h;
    double wwindowsize, hwindowsize;
public:
	Circle(GLFWwindow * window,  double sze, glm::vec3 coord) {
        float pos[360 * 6];
        int cnt = -1;
        for (int i = 0; i < 360; ++i) {
            double delta = i * PI / 180;
            pos[++cnt] = cos(delta) * sze / 2; // x
            pos[++cnt] = sin(delta) * sze / 2; // y
            pos[++cnt] = 0;

            pos[++cnt] = 0.2;
            pos[++cnt] = 0.4;
            pos[++cnt] = 0.7;
        }
        circle.Init(pos, sizeof(pos));
        circle.VertexAttribpointer(0, 0, 3, 6, GL_FLOAT);
        circle.VertexAttribpointer(1, 3, 3, 6, GL_FLOAT);
        circle.Shader("Shaders/Basic.shader");
        glfwGetWindowSize(window, &w, &h);
        wwindowsize = w;
        hwindowsize = h;
        circle.Ortho(0, wwindowsize, hwindowsize, 0, -100, 100);
        circle.Translate(coord, false);

	}
};
*/
/*
unsigned int __IDD__ = 0;

class Circle {
public:
    double wwindowsize, hwindowsize;
    double sze;
    double speed;
    float positions[360 * 6];
    float red, green, blue;
    unsigned int id;
    double xpos, ypos;
    string type;
    Object circle;
};


map<unsigned int, Circle> Data;


void Init(Circle circle) {
    int cnt = -1;
    for (int i = 0; i < 360; ++i) {
        double delta = i * PI / 180;
        circle.positions[++cnt] = cos(delta) * circle.sze / 2; // x
        circle.positions[++cnt] = sin(delta) * circle.sze / 2; // y
        circle.positions[++cnt] = 0;
        //colors
        circle.positions[++cnt] = circle.red;
        circle.positions[++cnt] = circle.green;
        circle.positions[++cnt] = circle.blue;
    }

    circle.id = __IDD__;
    circle.circle.Init(circle.positions, sizeof(circle.positions));
    circle.circle.VertexAttribpointer(0, 0, 3, 6, GL_FLOAT);
    circle.circle.VertexAttribpointer(1, 3, 3, 6, GL_FLOAT);
    circle.circle.Shader("Shaders/Basic.shader");
    circle.circle.Ortho(0, circle.wwindowsize, circle.hwindowsize, 0, -100, 100);
    circle.circle.Translate(glm::vec3(circle.xpos, circle.ypos, 0), false);
    Data[__IDD__] = circle;
    __IDD__++;
}

void Draw() {
    for (int i = 0; i < Data.size(); ++i) {
        if (Data[i].type == "DRAW") {
            Data[i].circle.Bind(GL_POLYGON, 0, 360);
        }
    }
}

bool Colision(unsigned int id) {
    for (int i = 0; i < Data.size(); ++i) {
        if (id != Data[i].id) {
            if (sqrt(pow(Data[id].xpos - Data[i].xpos, 2) + pow(Data[id].ypos - Data[i].ypos, 2)) <= (Data[id].sze / 2 + Data[i].sze / 2)) {
                return true;
            }
        }
    }
    return false;
}
*/
/*
struct Details {
    double wwindowsize, hwindowsize;
    float sze, red, green, blue;
};


map<unsigned int, Object> Data;
unsigned int __ID__ = 0;

class Circle {
public:
    Circle(Details &d) {
        Object circle;
        static float pos[360 * 6];
        int cnt = -1;
        for (int i = 0; i < 360; ++i) {
            double delta = i * PI / 180;
            pos[++cnt] = cos(delta) * d.sze / 2; // x
            pos[++cnt] = sin(delta) * d.sze / 2; // y
            pos[++cnt] = 0;

            pos[++cnt] = d.red;
            pos[++cnt] = d.green;
            pos[++cnt] = d.blue;
        }
        circle.Init(pos, sizeof(pos));
        circle.VertexAttribpointer(0, 0, 3, 6, GL_FLOAT);
        circle.VertexAttribpointer(1, 3, 3, 6, GL_FLOAT);
        circle.Shader("Shaders/Basic.shader");
        circle.Ortho(0, d.wwindowsize, d.hwindowsize, 0, -100, 100);
        circle.Translate(glm::vec3(d.wwindowsize / 2, d.hwindowsize / 2, 0), false);
        Data[__ID__] = circle;
        __ID__++;
    }
};
*/


class CircleDetails {
public:
    double wwindowsize, hwindowsize;
    double red, green, blue, sze, xpos, ypos, speed, antxpos = 0, antypos = 0;
    unsigned int id;
    int direction;
} d;

class Circle : public Object {
private:
    float pos[360 * 6];
public:
    unsigned int id;
    double sze, xpos, ypos, speed, wwindowsize, hwindowsize, antxpos, antypos;
    int direction;
    Circle(CircleDetails d) {
        int cnt = -1;
        id = d.id;
        sze = d.sze;
        xpos = d.xpos;
        ypos = d.ypos;
        speed = d.speed;
        wwindowsize = d.wwindowsize;
        hwindowsize = d.hwindowsize;
        direction = d.direction;
        antxpos = d.antxpos;
        antypos = d.antypos;
        for (int i = 0; i < 360; ++i) {
            double delta = i * PI / 180;
            pos[++cnt] = cos(delta) * d.sze / 2; // x
            pos[++cnt] = sin(delta) * d.sze / 2; // y
            pos[++cnt] = 0;

            pos[++cnt] = d.red;
            pos[++cnt] = d.green;
            pos[++cnt] = d.blue;
        }
        Init(pos, sizeof(pos));
        VertexAttribpointer(0, 0, 3, 6, GL_FLOAT);
        VertexAttribpointer(1, 3, 3, 6, GL_FLOAT);
        Shader("Shaders/Basic.shader");
        Ortho(0, d.wwindowsize, d.hwindowsize, 0, -100, 100);
        Translate(glm::vec3(d.xpos, d.ypos, 0), false);
    }
    void Draw() {
        Bind(GL_POLYGON, 0, 360);
    }
};

vector<Circle> Data;
vector<int> ID;
int __ID__ = 0;

void Init(CircleDetails d) {
    ID.push_back(__ID__);
    __ID__++;
    Circle c(d);
    Data.push_back(c);
}

double Rest(int a, int b) {
    return sqrt(pow(Data[a].xpos - Data[b].xpos, 2) + pow(Data[a].ypos - Data[b].ypos, 2)) - (Data[a].sze / 2 + Data[b].sze / 2);
}


bool Colision(unsigned int id, int& obs) {
    for (int i = 0; i < ID.size(); ++i) {
        if (id != ID[i]) {
            if (sqrt(pow(Data[id].xpos - Data[ID[i]].xpos, 2) + pow(Data[id].ypos - Data[ID[i]].ypos, 2)) <= (Data[id].sze / 2 + Data[ID[i]].sze / 2)) {
                
                obs = i;
                return true;
            }
        }
    }
    return false;
}


void MoveDirection(unsigned int id) {
    int obstacol;
    if (!Colision(ID[id], obstacol))
    switch (id)
    {
    case 1:
        Data[id].xpos += Data[id].speed;
        Data[id].ypos -= Data[id].speed;
        break;
    case 2:
        Data[id].xpos -= Data[id].speed;
        Data[id].ypos -= Data[id].speed;
        break;
    case 3:
        Data[id].xpos += Data[id].speed;
        Data[id].ypos += Data[id].speed;
        break;
    case 4:
        Data[id].xpos -= Data[id].speed;
        Data[id].ypos += Data[id].speed;
        break;
    }
}


void Move() {
    int obs = 0;
    vector<int> v;

    for (int i = 0; i < ID.size(); ++i) {
        //if (find(v.begin(), v.end(), ID[i]) != v.end()) continue;
        switch (Data[ID[i]].direction)
        {
        case 1://sus dreapta
            Data[ID[i]].antxpos = Data[ID[i]].xpos;
            Data[ID[i]].antypos = Data[ID[i]].ypos;
            Data[ID[i]].xpos += Data[ID[i]].speed;
            Data[ID[i]].ypos -= Data[ID[i]].speed;
            if (Colision(i, obs)) {
                Data[ID[i]].xpos -= Data[ID[i]].speed;
                Data[ID[i]].ypos += Data[ID[i]].speed;
                Data[ID[i]].xpos += Rest(i, ID[obs]);
                Data[ID[i]].ypos -= Rest(i, ID[obs]);
                swap(Data[ID[i]].direction, Data[ID[obs]].direction);
                //MoveDirection(i);
                //MoveDirection(ID[obs]);
                v.push_back(ID[obs]);
            }
            if (Data[ID[i]].xpos > Data[ID[i]].wwindowsize - Data[ID[i]].sze / 2) {
                Data[ID[i]].xpos = Data[ID[i]].wwindowsize - Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 2;
            }
            if (Data[ID[i]].ypos < Data[ID[i]].sze / 2) {
                Data[ID[i]].ypos = Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 3;
            }
            break;
        case 2://sus stanga
            Data[ID[i]].antxpos = Data[ID[i]].xpos;
            Data[ID[i]].antypos = Data[ID[i]].ypos;
            Data[ID[i]].xpos -= Data[ID[i]].speed;
            Data[ID[i]].ypos -= Data[ID[i]].speed;
           if (Colision(i, obs)) {
                Data[ID[i]].xpos += Data[ID[i]].speed;
                Data[ID[i]].ypos += Data[ID[i]].speed;
                Data[ID[i]].xpos -= Rest(i, ID[obs]);
                Data[ID[i]].ypos -= Rest(i, ID[obs]);
                swap(Data[ID[i]].direction, Data[ID[obs]].direction);
                //MoveDirection(i);
                //MoveDirection(ID[obs]);
                v.push_back(ID[obs]);
            }
            if (Data[ID[i]].xpos < Data[ID[i]].sze / 2) {
                Data[ID[i]].xpos = Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 1;
            }
            if (Data[ID[i]].ypos < Data[ID[i]].sze / 2) {
                Data[ID[i]].ypos = Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 4;
            }
            break;
        case 3://jos dreapa
            Data[ID[i]].antxpos = Data[ID[i]].xpos;
            Data[ID[i]].antypos = Data[ID[i]].ypos;
            Data[ID[i]].xpos += Data[ID[i]].speed;
            Data[ID[i]].ypos += Data[ID[i]].speed;
            if (Colision(i, obs)) {
                Data[ID[i]].xpos -= Data[ID[i]].speed;
                Data[ID[i]].ypos -= Data[ID[i]].speed;
                Data[ID[i]].xpos += Rest(i, ID[obs]);
                Data[ID[i]].ypos += Rest(i, ID[obs]);
                swap(Data[ID[i]].direction, Data[ID[obs]].direction);
                //MoveDirection(i);
                //MoveDirection(ID[obs]);
                v.push_back(ID[obs]);
            }
            if (Data[ID[i]].xpos > Data[ID[i]].wwindowsize - Data[ID[i]].sze / 2) {
                Data[ID[i]].xpos = Data[ID[i]].wwindowsize - Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 4;
            }
            if (Data[ID[i]].ypos > Data[ID[i]].hwindowsize - Data[ID[i]].sze / 2) {
                Data[ID[i]].ypos = Data[ID[i]].hwindowsize - Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 1;
            }
            break;
        case 4://jos stanga
            Data[ID[i]].antxpos = Data[ID[i]].xpos;
            Data[ID[i]].antypos = Data[ID[i]].ypos;
            Data[ID[i]].xpos -= Data[ID[i]].speed;
            Data[ID[i]].ypos += Data[ID[i]].speed;
            if (Colision(i, obs)) {
                Data[ID[i]].xpos += Data[ID[i]].speed;
                Data[ID[i]].ypos -= Data[ID[i]].speed;
                Data[ID[i]].xpos -= Rest(i, ID[obs]);
                Data[ID[i]].ypos += Rest(i, ID[obs]);
                swap(Data[ID[i]].direction, Data[ID[obs]].direction);
                //MoveDirection(i);
                //MoveDirection(ID[obs]);
                v.push_back(ID[obs]);
            }
            if (Data[ID[i]].xpos < Data[ID[i]].sze / 2) {
                Data[ID[i]].xpos = Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 3;
            }
            if (Data[ID[i]].ypos > Data[ID[i]].hwindowsize - Data[ID[i]].sze / 2) {
                Data[ID[i]].ypos = Data[ID[i]].hwindowsize - Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 2;
            }
            break;
        }

        if (Data[ID[i]].antxpos == Data[ID[i]].xpos && Data[ID[i]].antypos == Data[ID[i]].ypos) {
            Data[ID[i]].direction = 5 - Data[ID[i]].direction;
        }

        if(Data[ID[i]].speed > 0) Data[ID[i]].speed -= 0.001;
    }
}

/*
void Move() {
    int obstacol;

    vector<int> v;

    for (int i = 0; i < ID.size(); ++i) {
        //if (find(v.begin(), v.end(), ID[i]) != v.end()) continue;
        switch (Data[ID[i]].direction)
        {
        case 1://sus dreapta
            if (!Colision(ID[i], obstacol)) {
                Data[ID[i]].xpos += Data[ID[i]].speed;
                Data[ID[i]].ypos -= Data[ID[i]].speed;
            }
            else {
                swap(Data[ID[i]].direction, Data[ID[obstacol]].direction);
                //MoveDirection(i);
                //MoveDirection(ID[obstacol]);
                v.push_back(ID[obstacol]);
            }
            if (Data[ID[i]].xpos > Data[ID[i]].wwindowsize - Data[ID[i]].sze / 2) {
                Data[ID[i]].xpos = Data[ID[i]].wwindowsize - Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 2;
            }
            if (Data[ID[i]].ypos < Data[ID[i]].sze / 2) {
                Data[ID[i]].ypos = Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 3;
            }
            break;
        case 2://sus stanga
            if (!Colision(ID[i], obstacol)) {
                Data[ID[i]].xpos -= Data[ID[i]].speed;
                Data[ID[i]].ypos -= Data[ID[i]].speed;
            }
            else {
                swap(Data[ID[i]].direction, Data[ID[obstacol]].direction);
                //MoveDirection(i);
                //MoveDirection(ID[obstacol]);
                v.push_back(ID[obstacol]);
            }
            if (Data[ID[i]].xpos < Data[ID[i]].sze / 2) {
                Data[ID[i]].xpos = Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 1;
            }
            if (Data[ID[i]].ypos < Data[ID[i]].sze / 2) {
                Data[ID[i]].ypos = Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 4;
            }
            break;
        case 3://jos derapta
            if (!Colision(ID[i], obstacol)) {
                Data[ID[i]].xpos += Data[ID[i]].speed;
                Data[ID[i]].ypos += Data[ID[i]].speed;
            }
            else {
                swap(Data[ID[i]].direction, Data[ID[obstacol]].direction);
                //MoveDirection(i);
                //MoveDirection(ID[obstacol]);
                v.push_back(ID[obstacol]);
            }
            if (Data[ID[i]].xpos > Data[ID[i]].wwindowsize - Data[ID[i]].sze / 2) {
                Data[ID[i]].xpos = Data[ID[i]].wwindowsize - Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 4;
            }
            if (Data[ID[i]].ypos > Data[ID[i]].hwindowsize - Data[ID[i]].sze / 2) {
                Data[ID[i]].ypos = Data[ID[i]].hwindowsize - Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 1;
            }
            break;
        case 4://jos stanga
            if (!Colision(ID[i], obstacol)) {
                Data[ID[i]].xpos -= Data[ID[i]].speed;
                Data[ID[i]].ypos += Data[ID[i]].speed;
            }
            else {
                swap(Data[ID[i]].direction, Data[ID[obstacol]].direction);
                //MoveDirection(i);
                //MoveDirection(ID[obstacol]);
                v.push_back(ID[obstacol]);
            }
            if (Data[ID[i]].xpos < Data[ID[i]].sze / 2) {
                Data[ID[i]].xpos = Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 3;
            }
            if (Data[ID[i]].ypos > Data[ID[i]].hwindowsize - Data[ID[i]].sze / 2) {
                Data[ID[i]].ypos = Data[ID[i]].hwindowsize - Data[ID[i]].sze / 2;
                Data[ID[i]].direction = 2;
            }
            break;    
        }
    }
}
*/



void Draw() {
    for (int i = 0; i < ID.size(); ++i) {
        Data[ID[i]].Translate(glm::vec3(Data[i].xpos, Data[i].ypos, 0), false);
        Data[ID[i]].Draw();
    }
}
