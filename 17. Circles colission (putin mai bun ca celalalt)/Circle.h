#pragma once
#include "Object.h"
#define PI 3.1415926535
#include <vector>
#include <map>


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

void ChangeSpeed(int b, int a) {
    Data[a].speed += Data[b].speed / Data[b].sze;
    Data[b].speed -= Data[b].speed / (Data[b].sze * 2);
    if (Data[b].speed < 0) Data[b].speed = 0;
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
                ChangeSpeed(ID[i], ID[obs]);
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
                ChangeSpeed(ID[i], ID[obs]);
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
                ChangeSpeed(ID[i], ID[obs]);
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
                ChangeSpeed(ID[i], ID[obs]);
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

void Draw() {
    for (int i = 0; i < ID.size(); ++i) {
        Data[ID[i]].Translate(glm::vec3(Data[i].xpos, Data[i].ypos, 0), false);
        Data[ID[i]].Draw();
    }
}
