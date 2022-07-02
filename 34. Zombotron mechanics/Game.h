#pragma once
#include <iostream>
#include <vector>
#include "Object.h"

using namespace std;

//clasa setari care se atibuie fiecarui obiect
class Settings {
public:
	//dimensiunile ferestrei de intrare
	float wwindowsize, hwindowsize;
	//lungimea, inaltimea, originea si pozitiile obiectului
	float xsize, ysize, xorigin = 0, yorigin = 0, xpos, ypos;
	//unghiul la care se roteste obiectul
	float angle = 0;
	//o folosim pentu a muta obiectele pe ecran( e ca si cum am schimba xpos +=xview si la fel si pentru ypos +=yview
	float xview = 0, yview = 0;
	//modul cu incarcam textura (rotita pe orizontala)
	bool fliptexture = true;
	//locata texturii
	string texture;
};

//clasa pentru obiecte din caracterul main
class Character {
private:
	//functie care atriguie coordonatele de pozitie si textura in functie de originea (origine = unde se roteste obiectul)
	vector<float> CreateVertices(float xsize, float ysize, float xorigin, float yorigin) {
		vector<float> v;
		v.push_back(-xsize / 2 - xorigin);
		v.push_back(-ysize / 2 - yorigin);

		v.push_back(0);
		v.push_back(0);

		v.push_back(xsize / 2 - xorigin);
		v.push_back(-ysize / 2 - yorigin);

		v.push_back(1);
		v.push_back(0);

		v.push_back(xsize / 2 - xorigin);
		v.push_back(ysize / 2 - yorigin);

		v.push_back(1);
		v.push_back(1);

		v.push_back(-xsize / 2 - xorigin);
		v.push_back(ysize / 2 - yorigin);

		v.push_back(0);
		v.push_back(1);
		return v;
	}
	//obiectul folosit
	Object* obj = new Object;
	//unghiul pentru a roti obiectul in x (pe orizontala)
	float rotate_x_angle = 0;
public:
	//functie pentru atriguirea unghiului
	void RotateTexture(float ang) {
		rotate_x_angle = ang;
	}

	//functie pentru reshape la fereastra
	void ReConfig(Settings* sett) {
		obj->Vertices = CreateVertices(sett->xsize, sett->ysize, sett->xorigin, sett->yorigin);
		obj->ReConfig(GL_STATIC_DRAW);
		obj->Texture(sett->texture.c_str(), GL_RGBA, sett->fliptexture);
		obj->Ortho(0, sett->wwindowsize, 0, sett->hwindowsize, 10, -10);
		obj->Translate(glm::vec3(sett->xpos, sett->ypos, 0), false);
		obj->Rotate(sett->angle, glm::vec3(0, 0, 1), false);
	}
	//creiem caracterul bazandu-ne pe setari
	void CreateCharacter(Settings * sett) {
		obj->Vertices = CreateVertices(sett->xsize, sett->ysize, sett->xorigin, sett->yorigin);
		obj->Config(GL_STATIC_DRAW, false);
		obj->VertexAttribpointer(0, 0, 2, 4, GL_FLOAT);
		obj->VertexAttribpointer(1, 2, 2, 4, GL_FLOAT);
		obj->Shader("Shaders/Basic.shader");
		obj->Texture(sett->texture.c_str(), GL_RGBA, sett->fliptexture);
		obj->Ortho(0, sett->wwindowsize, 0, sett->hwindowsize, 10, -10);
		obj->Translate(glm::vec3(sett->xpos, sett->ypos, 0), false);
	}
	//desenam caracterul
	void DrawCharacter(Settings* sett) {
		obj->Translate(glm::vec3(sett->xpos, sett->ypos, 0), false);
		obj->Rotate(sett->angle, glm::vec3(0, 0, 1), false);
		obj->Rotate(glm::radians(rotate_x_angle), glm::vec3(1, 0, 0), true);
		obj->Bind(GL_QUADS);
	}

};

//clasa pentru harta de joc
class Map {
private:
	//functie care atriguie coordonatele de pozitie si textura in functie de originea (origine = unde se roteste obiectul)
	vector<float> CreateVertices(float xsize, float ysize, float xorigin, float yorigin) {
		vector<float> v;
		v.push_back(-xsize / 2 - xorigin);
		v.push_back(-ysize / 2 - yorigin);

		v.push_back(0);
		v.push_back(0);

		v.push_back(xsize / 2 - xorigin);
		v.push_back(-ysize / 2 - yorigin);

		v.push_back(1);
		v.push_back(0);

		v.push_back(xsize / 2 - xorigin);
		v.push_back(ysize / 2 - yorigin);

		v.push_back(1);
		v.push_back(1);

		v.push_back(-xsize / 2 - xorigin);
		v.push_back(ysize / 2 - yorigin);

		v.push_back(0);
		v.push_back(1);
		return v;
	}
	//obiectul nostru
	Object* obj = new Object;
	//harta sub forma de matrice
	char Level1Map[35][35] = {
		"##################################",
		"#                                #",
		"#                                #",
		"#                                #",
		"#   =####  ##########  ##  #*****#",
		"#   =                      #     #",
		"#=                         #     #",
		"#=                         #     #",
		"####  ###  ##  #####  ####=#     #",
		"#                         =#     #",
		"#                         =#     #",
		"#                         =#     #",
		"#                       ####     #",
		"#=##########################     #",
		"#=                         #     #",
		"#=                         #     #",
		"#=                         #     #",
		"#=                         #     #",
		"##########  #  #  ######  =#     #",
		"#        #  #  #          ##     #",
		"#                        ###     #",
		"#    MM              # #####     #",
		"#=########  ################     #",
		"#=       #  #              #     #",
		"#=       #  #              #     #",
		"##          #              #     #",
		"###         #              #     #",
		"### #    #  #              #     #",
		"########=####              #     #",
		"#       =#                 #     #",
		"#       =#                 #     #",
		"#    #  =#                 #     #",
		"#   ### =#                 #     #",
		"##################################"
	};

public:
	//functie pentru reshape la fereastra
	void ReConfig(Settings* sett) {
		obj->Vertices = CreateVertices(sett->xsize, sett->ysize, sett->xorigin, sett->yorigin);
		obj->ReConfig(GL_STATIC_DRAW);
		obj->Texture(sett->texture.c_str(), GL_RGBA, sett->fliptexture);
		obj->Ortho(0, sett->wwindowsize, 0, sett->hwindowsize, 10, -10);
		obj->Translate(glm::vec3(sett->xpos, sett->ypos, 0), false);
		obj->Rotate(sett->angle, glm::vec3(0, 0, 1), false);
	}

	//creiem harta folosind setarile ei
	void CreateMap(Settings* sett) {
		obj->Vertices = CreateVertices(sett->xsize, sett->ysize, sett->xorigin, sett->yorigin);
		obj->Config(GL_STATIC_DRAW, false);
		obj->VertexAttribpointer(0, 0, 2, 4, GL_FLOAT);
		obj->VertexAttribpointer(1, 2, 2, 4, GL_FLOAT);
		obj->Shader("Shaders/Basic.shader");
		obj->Texture(sett->texture.c_str(), GL_RGBA, sett->fliptexture);
		obj->Ortho(0, sett->wwindowsize, 0, sett->hwindowsize, 1, -1);
		obj->Translate(glm::vec3(sett->xpos, sett->ypos, 0), false);
	}
	//desenam harta
	void DrawMap(Settings * sett) {
		obj->Translate(glm::vec3(sett->xpos + sett->xview, sett->ypos + sett->yview, 0), false);
		obj->Rotate(sett->angle, glm::vec3(0, 0, 1), false);
		obj->Bind(GL_QUADS);
	}


};


//clasa pentru gloante
class Glont {
private:
	//functie care atriguie coordonatele de pozitie si textura in functie de originea (origine = unde se roteste obiectul)
	vector<float> CreateVertices(float xsize, float ysize, float xorigin, float yorigin) {
		vector<float> v;
		v.push_back(-xsize / 2 - xorigin);
		v.push_back(-ysize / 2 - yorigin);

		v.push_back(0);
		v.push_back(0);

		v.push_back(xsize / 2 - xorigin);
		v.push_back(-ysize / 2 - yorigin);

		v.push_back(1);
		v.push_back(0);

		v.push_back(xsize / 2 - xorigin);
		v.push_back(ysize / 2 - yorigin);

		v.push_back(1);
		v.push_back(1);

		v.push_back(-xsize / 2 - xorigin);
		v.push_back(ysize / 2 - yorigin);

		v.push_back(0);
		v.push_back(1);
		return v;
	}
	//obiectul folosit nealocat dinamic deoarece se vor stoca mai multe gloante
	Object obj;

public:
	//de valorile astea avem nevoie sa le gasim usor si pentru a nu salva pt fiecare glont setarile sale am salvat ce era important aici
	float xspeed, yspeed, xpos, ypos;

	//punem o singura data textura pe glont
	void PutsTexture(string texture, bool fliptexture) {
		obj.Texture(texture.c_str(), GL_RGBA, fliptexture);
	}

	//functie pentru reshape la fereastra
	void ReConfig(Settings sett) {
		obj.Vertices = CreateVertices(sett.xsize, sett.ysize, sett.xorigin, sett.yorigin);
		obj.ReConfig(GL_STATIC_DRAW);
		obj.Texture(sett.texture.c_str(), GL_RGBA, sett.fliptexture);
		obj.Ortho(0, sett.wwindowsize, 0, sett.hwindowsize, 10, -10);
		obj.Translate(glm::vec3(sett.xpos, sett.ypos, 0), false);
		obj.Rotate(sett.angle, glm::vec3(0, 0, 1), false);
	}


	//creiem glontul
	void CreateGlont(Settings sett) {
		obj.Vertices = CreateVertices(sett.xsize, sett.ysize, sett.xorigin, sett.yorigin);
		obj.Config(GL_STATIC_DRAW, false);
		obj.VertexAttribpointer(0, 0, 2, 4, GL_FLOAT);
		//obj.VertexAttribpointer(1, 2, 2, 4, GL_FLOAT);
		obj.Shader("Shaders/Glont.shader");
		//obj.Texture(sett.texture.c_str(), GL_RGBA, sett.fliptexture);
		obj.Ortho(0, sett.wwindowsize, 0, sett.hwindowsize, 1, -1);
		obj.Translate(glm::vec3(sett.xpos, sett.ypos, 0), false);
		obj.Rotate(sett.angle, glm::vec3(0, 0, 1), false);
	}
	//desenam glontul
	void DrawGlont() {
		xpos += xspeed;
		ypos += yspeed;
		obj.Translate(glm::vec3(xspeed, yspeed, 0), true);
		obj.Bind(GL_QUADS);
	}
};
