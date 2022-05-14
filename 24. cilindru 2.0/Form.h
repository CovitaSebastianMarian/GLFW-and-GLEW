#pragma once
#include <iostream>
#include <algorithm>

#define PI 3.14159265359

using namespace std;


class FormDetails {
public:
	float positions[3600];
	unsigned int indices[3600];
	//float* positions;
	//unsigned int *indices;
	float size, height;
	int edges;
	int positions_size, indices_size;
};


static inline int Positions_size(int edges) {
	int size = (edges + 1) * 2 + edges * 2;
	return size;
}
static inline int Indices_size(int edges) {
	int size = edges * 6 + 6 * edges;
	return size;
}


static FormDetails CreateForm(float size, float height, int edges) {
	FormDetails FD;
	FD.edges = edges;
	FD.height = height;
	FD.size = size;
	FD.positions_size = Positions_size(edges);
	FD.indices_size = Indices_size(edges);

	//FD.positions = new float[FD.positions_size];
	//FD.indices = new unsigned int[FD.indices_size];

	int positions_cnt = -1;
	int indices_cnt = -1;

	while (360 % edges != 0) edges--;

	int jump = 360 / edges;
	int ant = 0;

	FD.positions[++positions_cnt] = 0;
	FD.positions[++positions_cnt] = 0;
	FD.positions[++positions_cnt] = height / 2;

	FD.positions[++positions_cnt] = 0.5;
	FD.positions[++positions_cnt] = 0.5;

	for (int i = 0; i <= 360; i += jump) {
		double delta = i * PI / 180;

		FD.positions[++positions_cnt] = cos(delta) * FD.size / 2;
		FD.positions[++positions_cnt] = sin(delta) * FD.size / 2;
		FD.positions[++positions_cnt] = height / 2;

		FD.positions[++positions_cnt] = cos(delta) / 2 + 0.5;
		FD.positions[++positions_cnt] = sin(delta) / 2 + 0.5;

		if (ant != 0) {
			FD.indices[++indices_cnt] = 0;
			FD.indices[++indices_cnt] = ant + 1;
			FD.indices[++indices_cnt] = ant;
		}
		ant++;
	}

	ant++;
	int ant2 = ant;

	FD.positions[++positions_cnt] = 0;
	FD.positions[++positions_cnt] = 0;
	FD.positions[++positions_cnt] = -height / 2;

	FD.positions[++positions_cnt] = 0.5;
	FD.positions[++positions_cnt] = 0.5;

	for (int i = 0; i <= 360; i += jump) {
		double delta = i * PI / 180;

		FD.positions[++positions_cnt] = cos(delta) * FD.size / 2;
		FD.positions[++positions_cnt] = sin(delta) * FD.size / 2;
		FD.positions[++positions_cnt] = -height / 2;

		FD.positions[++positions_cnt] = cos(delta) / 2 + 0.5;
		FD.positions[++positions_cnt] = sin(delta) / 2 + 0.5;

		if (ant != ant2) {
			FD.indices[++indices_cnt] = ant2;
			FD.indices[++indices_cnt] = ant + 1;
			FD.indices[++indices_cnt] = ant;
		}
		ant++;
	}

	for (int i = 0; i < edges * 3; i += 3) {
		int j = FD.indices[i + 1] + ant2;
		FD.indices[++indices_cnt] = FD.indices[i + 1];
		FD.indices[++indices_cnt] = FD.indices[i + 2];
		FD.indices[++indices_cnt] = j;

		FD.indices[++indices_cnt] = j;
		FD.indices[++indices_cnt] = j - 1;
		FD.indices[++indices_cnt] = FD.indices[i + 2];
	}
	FD.indices_size = indices_cnt + 1;
	FD.positions_size = positions_cnt;
	return FD;
}

/*
class Form {
protected:
	FormDetails FD;
	static inline int Positions_size(int edges) {
		int size = (edges + 1) * 2 + edges * 2;
		return size;
	}
	static inline int Indices_size(int edges) {
		int size = edges * 6 + 6 * edges;
		return size;
	}

public:
	FormDetails CreateForm(float size, float height, int edges) {
		FD.edges = edges;
		FD.height = height;
		FD.size = size;
		FD.positions_size = Positions_size(edges);
		FD.indices_size = Indices_size(edges);

		FD.positions = new float[FD.positions_size];
		FD.indices = new unsigned int[FD.indices_size];

		int positions_cnt = -1;
		int indices_cnt = -1;

		while (360 % edges != 0) edges--;

		int jump = 360 / edges;
		int ant = 0;

		FD.positions[++positions_cnt] = 0;
		FD.positions[++positions_cnt] = 0;
		FD.positions[++positions_cnt] = height / 2;

		FD.positions[++positions_cnt] = 0.5;
		FD.positions[++positions_cnt] = 0.5;

		for (int i = 0; i <= 360; i += jump) {
			double delta = i * PI / 180;

			FD.positions[++positions_cnt] = cos(delta) * FD.size / 2;
			FD.positions[++positions_cnt] = sin(delta) * FD.size / 2;
			FD.positions[++positions_cnt] = height / 2;

			FD.positions[++positions_cnt] = cos(delta) / 2 + 0.5;
			FD.positions[++positions_cnt] = sin(delta) / 2 + 0.5;

			if (ant != 0) {
				FD.indices[++indices_cnt] = 0;
				FD.indices[++indices_cnt] = ant;
				FD.indices[++indices_cnt] = ant + 1;
			}
			ant++;
		}

		ant++;
		int ant2 = ant;

		FD.positions[++positions_cnt] = 0;
		FD.positions[++positions_cnt] = 0;
		FD.positions[++positions_cnt] = -height / 2;

		FD.positions[++positions_cnt] = 0.5;
		FD.positions[++positions_cnt] = 0.5;

		for (int i = 0; i <= 360; i += jump) {
			double delta = i * PI / 180;

			FD.positions[++positions_cnt] = cos(delta) * FD.size / 2;
			FD.positions[++positions_cnt] = sin(delta) * FD.size / 2;
			FD.positions[++positions_cnt] = -height / 2;

			FD.positions[++positions_cnt] = cos(delta) / 2 + 0.5;
			FD.positions[++positions_cnt] = sin(delta) / 2 + 0.5;

			if (ant != ant2) {
				FD.indices[++indices_cnt] = 0;
				FD.indices[++indices_cnt] = ant2;
				FD.indices[++indices_cnt] = ant + 1;
			}
			ant++;
		}

		for (int i = 0; i < edges * 3; ++i) {
			int j = FD.indices[i + 1] + ant2;
			FD.indices[++indices_cnt] = FD.indices[i + 1];
			FD.indices[++indices_cnt] = FD.indices[i + 2];
			FD.indices[++indices_cnt] = j;

			FD.indices[++indices_cnt] = j;
			FD.indices[++indices_cnt] = j - 1;
			FD.indices[++indices_cnt] = FD.indices[i + 2];
		}
	}
};
*/