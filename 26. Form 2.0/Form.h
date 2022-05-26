#pragma once
#include <iostream>
#include <algorithm>

#define PI 3.14159265359

using namespace std;



class Form {
public:
	
	class FormDetails {
	public:
		float positions[3600];
		unsigned int indices[3600];
		float xsize, ysize, height;
		float xsize1, xsize2, ysize1, ysize2;

		int positions_size, indices_size, edges;
	};

	FormDetails CreateFace1(float xsize, float ysize, float height, int edges, int reper = 0) {
		FormDetails FD;
		FD.height = height/2;
		FD.xsize = xsize;
		FD.ysize = ysize;

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
			double delta2 = (i + reper) * PI / 180;
			FD.positions[++FD.positions_size] = cos(delta) * FD.xsize / 2;
			FD.positions[++FD.positions_size] = sin(delta) * FD.ysize / 2;
			FD.positions[++FD.positions_size] = FD.height/2;

			FD.positions[++FD.positions_size] = cos(delta2) / 2 + 0.5;
			FD.positions[++FD.positions_size] = sin(delta2) / 2 + 0.5;

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

	FormDetails CreateFace2(float xsize,float ysize, float height, int edges, int reper = 0) {
		FormDetails FD;
		FD.height = height/2;
		FD.xsize = xsize;
		FD.ysize = ysize;
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
			double delta2 = (i + reper) * PI / 180;
			FD.positions[++FD.positions_size] = cos(delta) * FD.xsize / 2;
			FD.positions[++FD.positions_size] = sin(delta) * FD.ysize / 2;
			FD.positions[++FD.positions_size] = -FD.height/2;

			FD.positions[++FD.positions_size] = cos(delta2) / 2 + 0.5;
			FD.positions[++FD.positions_size] = sin(delta2) / 2 + 0.5;

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

	FormDetails CreateLateralFace(float xsize1, float ysize1, float xsize2, float ysize2, float height, int edges, int reper = 0) {
		FormDetails FD;
		FD.height = height / 2;
		FD.xsize1 = xsize1;
		FD.ysize1 = ysize1;
		FD.xsize2 = xsize2;
		FD.ysize2 = ysize2;
		while (360 % edges != 0) edges--;
		FD.edges = edges;
		int jump = 360 / edges;

		FD.positions_size = -1;
		FD.indices_size = -1;
		

		for (int i = 0; i <= 360; i += jump) {
			double delta = i * PI / 180;
			
			FD.positions[++FD.positions_size] = cos(delta) * FD.xsize1 / 2;
			FD.positions[++FD.positions_size] = sin(delta) * FD.ysize1 / 2;
			FD.positions[++FD.positions_size] = FD.height / 2;

			FD.positions[++FD.positions_size] = (float)((float)(i) / ((float)jump) * (1 / (float)(edges)));
			FD.positions[++FD.positions_size] = 1;

			FD.positions[++FD.positions_size] = cos(delta) * FD.xsize2 / 2;
			FD.positions[++FD.positions_size] = sin(delta) * FD.ysize2 / 2;
			FD.positions[++FD.positions_size] = -FD.height / 2;

			FD.positions[++FD.positions_size] = (float)((float)(i) / ((float)jump) * (1 / (float)(edges)));
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