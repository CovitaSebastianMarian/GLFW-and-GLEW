#pragma once
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;
template<class T>
class Loader {
private:

	int x, y, z;
public:
	int x_size, y_size, z_size;
	void Load(const char* loadfile, T***& m) {
		ifstream fin(loadfile);
		fin >> x >> y >> z;
		x_size = x;
		y_size = y;
		z_size = z;
		m = new T * *[z];
		for (int zz = 0; zz < z; ++zz) {
			m[zz] = new T * [y];
			for (int yy = 0; yy < y; ++yy) {
				m[zz][yy] = new T[x];
			}
		}


		T val;

		for (int zz = 0; zz < z; ++zz) {
			for (int yy = 0; yy < y; ++yy) {
				for (int xx = 0; xx < x; ++xx) {
					fin >> val;
					m[zz][yy][xx] = val;
				}
			}
		}


	}
	void Load(const char* loadfile, T**& m) {
		ifstream fin(loadfile);
		fin >> x >> y >> z;
		x_size = x;
		y_size = y;
		z_size = 1;
		m = new T * [y];
		for (int yy = 0; yy < y; ++yy) {
			m[yy] = new T[x];
		}

		T val;
		for (int yy = 0; yy < y; ++yy) {
			for (int xx = 0; xx < x; ++xx) {
				fin >> val;
				m[yy][xx] = val;
			}
		}
	}


};

