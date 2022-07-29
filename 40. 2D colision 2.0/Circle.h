#pragma once
#include <iostream>
#include "Object.h"
#define PI 3.14159265359
using namespace std;


enum class DIRECTION : int {
	NONE = 0,UP_RIGHT = 1, UP_LEFT = 2, DOWN_LEFT = 3, DOWN_RIGHT = 4
};


class Circle
{
protected:

	Object obj;
public:
	float xpos, ypos, radius, angle, speed, force, mass, velocity;
	float wwindowsize, hwindowsize;
	DIRECTION dir = DIRECTION::NONE;
	bool hide;
	void CreateCircle() {
		/*
		vector<float> vertices;

		for (int i = 0; i < 360; ++i) {
			double delta = i * PI / 180;
			vertices.push_back(cos(delta) * radius); // x
			vertices.push_back(sin(delta) * radius); // y

			vertices.push_back(cos(delta) / 2 + 0.5);
			vertices.push_back(sin(delta) / 2 + 0.5);
		}

		obj.Vertices = vertices;
		obj.DynamicInit(GL_DYNAMIC_DRAW);
		obj.VertexAttribpointer(0, 0, 2, 4, GL_FLOAT);
		obj.VertexAttribpointer(1, 2, 2, 4, GL_FLOAT);
		obj.Shader("Shaders/2D.shader");
		obj.Texture("Textures/circle.jpg", GL_RGB, true);
		obj.Ortho(0,wwindowsize, 0, hwindowsize, wwindowsize, 0);
		obj.Translate(glm::vec3(xpos, ypos, 0), false);
		//obj.Perspective(glm::radians(45.0), 1, 0.1, 100);
		//obj.LookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
		*/

		ObjFile model;
		model.MTLlocation = "Models/";
		model.TextureLocation = "Textures/";
		model.LoadModel("Models/cube.obj");
		obj.Vertices = model.KitVector[0].Vertices;
		obj.Indices = model.KitVector[0].Indices;
		obj.Config(GL_DYNAMIC_DRAW);
		obj.VertexAttribpointer(0, 0, 3, 8, GL_FLOAT);
		obj.VertexAttribpointer(1, 3, 2, 8, GL_FLOAT);
		obj.VertexAttribpointer(2, 5, 3, 8, GL_FLOAT);
		obj.Shader("Shaders/Texture.shader");
		obj.Texture(model.KitVector[0].texture.c_str(), GL_RGB, true);
		obj.Ortho(0, wwindowsize, 0, hwindowsize, wwindowsize, 0);
		obj.Translate(glm::vec3(xpos, ypos, 0), false);
		obj.Perspective(glm::radians(85.0), 1, 0.1, 100);
		obj.LookAt(glm::vec3(0, 0, 2.1), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
		obj.Scale(glm::vec3(radius, radius, radius), false);
		


		obj.setVec("light.ambient", 0.1f, 0.1f, 0.1f);
		obj.setVec("light.diffuse", 0.8f, 0.8f, 0.8f);
		obj.setVec("light.specular", 1.0f, 1.0f, 1.0f);

		obj.setVec("light.position", obj.Camera.eye.x, obj.Camera.eye.y, obj.Camera.eye.z);
		obj.setVec("viewPos", obj.Camera.eye.x, obj.Camera.eye.y, obj.Camera.eye.z);

		obj.setVec("material.shininess", 32);
		obj.setVec("material.specular", 1.0f, 1.0f, 1.0f);

	}
	
	void ChangeDirection() {
		if (angle <= 90) {
			dir = DIRECTION::UP_RIGHT;
		}
		else if (angle <= 180) {
			dir = DIRECTION::UP_LEFT;
		}
		else if (angle <= 270) {
			dir = DIRECTION::DOWN_LEFT;
		}
		else {
			dir = DIRECTION::DOWN_RIGHT;
		}
	}




	void Draw() {
		obj.Scale(glm::vec3(radius, radius, radius), false);
		obj.Translate(glm::vec3(xpos, ypos, 0), false);		
		obj.Bind(GL_POLYGON, 0, obj.Vertices.size());
	}









};

