#pragma once
#include <iostream>
#include <algorithm>
#include "Object.h"

#define PI 3.14159265359

using namespace std;



class Form {
public:
	
	class FormDetails {
	public:
		float positions[3600];
		unsigned int indices[3600];
		float size, height;
		int positions_size, indices_size, edges;
	};

	FormDetails CreateFace1(float size, float height, int edges) {
		FormDetails FD;
		FD.height = height/2;
		FD.size = size;

		while (360 % edges != 0) edges--;
		FD.edges = edges;
		int jump = 360 / edges;
		int ant = 0;

		FD.positions_size = -1;
		FD.indices_size = -1;

		FD.positions[++FD.positions_size] = 0;
		FD.positions[++FD.positions_size] = 0;
		FD.positions[++FD.positions_size] = FD.height / 2;

		FD.positions[++FD.positions_size] = 0.5;
		FD.positions[++FD.positions_size] = 0.5;

		for (int i = 0; i <= 360; i += jump) {
			double delta = i * PI / 180;
			FD.positions[++FD.positions_size] = cos(delta) * FD.size / 2;
			FD.positions[++FD.positions_size] = sin(delta) * FD.size / 2;
			FD.positions[++FD.positions_size] = FD.height/2;

			FD.positions[++FD.positions_size] = cos(delta) / 2 + 0.5;
			FD.positions[++FD.positions_size] = sin(delta) / 2 + 0.5;

			if (ant != 0) {
				FD.indices[++FD.indices_size] = 0;
				FD.indices[++FD.indices_size] = ant + 1;
				FD.indices[++FD.indices_size] = ant;
			}
			ant++;
		}
		FD.indices_size++;
		FD.positions_size++;
		return FD;
	}

	FormDetails CreateFace2(float size, float height, int edges) {
		FormDetails FD;
		FD.height = height/2;
		FD.size = size;
		while (360 % edges != 0) edges--;
		FD.edges = edges;
		int jump = 360 / edges;
		int ant = 0;
		FD.positions_size = -1;
		FD.indices_size = -1;

		FD.positions[++FD.positions_size] = 0;
		FD.positions[++FD.positions_size] = 0;
		FD.positions[++FD.positions_size] = -FD.height/2;

		FD.positions[++FD.positions_size] = 0.5;
		FD.positions[++FD.positions_size] = 0.5;

		for (int i = 0; i <= 360; i += jump) {
			double delta = i * PI / 180;
			FD.positions[++FD.positions_size] = cos(delta) * FD.size / 2;
			FD.positions[++FD.positions_size] = sin(delta) * FD.size / 2;
			FD.positions[++FD.positions_size] = -FD.height/2;

			FD.positions[++FD.positions_size] = cos(delta) / 2 + 0.5;
			FD.positions[++FD.positions_size] = sin(delta) / 2 + 0.5;

			if (ant != 0) {
				FD.indices[++FD.indices_size] = 0;
				FD.indices[++FD.indices_size] = ant + 1;
				FD.indices[++FD.indices_size] = ant;
			}
			ant++;
		}
		FD.indices_size++;
		FD.positions_size++;
		return FD;
	}

	FormDetails CreateLateralFace(float size, float height, int edges) {
		FormDetails FD;
		FD.height = height / 2;
		FD.size = size;
		while (360 % edges != 0) edges--;
		FD.edges = edges;
		int jump = 360 / edges;

		FD.positions_size = -1;
		FD.indices_size = -1;
		
		FD.positions[++FD.positions_size] = cos(0) * FD.size / 2;
		FD.positions[++FD.positions_size] = sin(0) * FD.size / 2;
		FD.positions[++FD.positions_size] = FD.height / 2; 

		FD.positions[++FD.positions_size] = 0;
		FD.positions[++FD.positions_size] = 1;


		FD.positions[++FD.positions_size] = cos(0) * FD.size / 2;
		FD.positions[++FD.positions_size] = sin(0) * FD.size / 2;
		FD.positions[++FD.positions_size] = -FD.height / 2;

		FD.positions[++FD.positions_size] = 0;
		FD.positions[++FD.positions_size] = 0;



		for (int i = jump; i <= 360; i += jump) {
			double delta = i * PI / 180;

			FD.positions[++FD.positions_size] = cos(delta) * FD.size / 2;
			FD.positions[++FD.positions_size] = sin(delta) * FD.size / 2;
			FD.positions[++FD.positions_size] = FD.height / 2;

			FD.positions[++FD.positions_size] = (float)((float)i / ((float)jump) * (1 / (float)(edges)));
			FD.positions[++FD.positions_size] = 1;

			FD.positions[++FD.positions_size] = cos(delta) * FD.size / 2;
			FD.positions[++FD.positions_size] = sin(delta) * FD.size / 2;
			FD.positions[++FD.positions_size] = -FD.height / 2;

			FD.positions[++FD.positions_size] = (float)((float)i / ((float)jump) * (1 / (float)(edges)));
			FD.positions[++FD.positions_size] = 0;
		}

		for (int i = 0; i <= edges * 2 - 1; i++) {
			FD.indices[++FD.indices_size] = 0 + i;
			FD.indices[++FD.indices_size] = 1 + i;
			FD.indices[++FD.indices_size] = 2 + i;
		}

		FD.indices_size++;
		FD.positions_size++;
		return FD;
	}

};
