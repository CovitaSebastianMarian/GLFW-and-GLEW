#pragma once
#include <GL/glew.h> // #include <glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include "stb_image.h"
#include <exception>
#include <vector>
#include <queue>
#include "Shader.h"
#include "error.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#pragma warning(disable : 4996)
using namespace std;

#ifdef STB_IMAGE_IMPLEMENTATION
#if defined(INCLUDE_ERROR_LIBRARY) && defined(INCLUDE_SHADER_LIBRARY)

/*#############################################################
#                                                             #
#   $$$$  $$$$$  $$$$    $$$    $$$$  $$$$$  $   $$$   $   $  #
#  $      $      $   $  $   $  $        $    $  $   $  $$  $  #
#   $$$   $$$$$  $$$$$  $$$$$   $$$     $    $  $$$$$  $ $ $  #
#      $  $      $   $  $   $      $    $    $  $   $  $  $$  #
#  $$$$   $$$$$  $$$$   $   $  $$$$     $    $  $   $  $   $  #
#                                                             #
#############################################################*/


/*##############################################################################
# CONTENT:                                                                     #
#	-> Object();                                                               #
#	-> Destruct();                                                             #
#	-> Init();                                                                 #
#   -> DynamicInit();                                                          #
#   -> DynamicInitInd();                                                       #
#	-> Reload();                                                               #
#	-> LoadMatrix();                                                           #
#	-> VertexAttribPointer();                                                  #
#	-> Shader();                                                               #
#	-> Texture();                                                              #
#	-> Perspective();                                                          #
#	-> LookAt();                                                               #
#	-> Ortho();                                                                #
#	-> Translate();                                                            #
#	-> Scale();                                                                #
#	-> Rotate();                                                               #
#	-> Bind();                                                                 #
#	-> UnBind();                                                               #
#                                                                              #
#                                                                              #
##############################################################################*/


class Object{
protected:
	unsigned int VAO, VBO, EBO;
	int location;
	string locationname = "Matrix";
	bool config_with_indices = false;
public:
	vector<float> Vertices;
	vector<unsigned int> Indices;
	unsigned int texture, shader;
	glm::mat4 Matrix = glm::mat4(1.0);
	glm::mat4 perspective = glm::mat4(1.0);
	glm::mat4 camera = glm::mat4(1.0);
	glm::mat4 ortho = glm::mat4(1.0);
	glm::mat4 translate = glm::mat4(1.0);
	glm::mat4 rotate = glm::mat4(1.0);
	glm::mat4 scale = glm::mat4(1.0);

	void ChangeShaderLocationName(string newname) {
		locationname = newname;
	}
	void Destruct() {
		GLCallReturn(glDeleteVertexArrays(1, &VAO));
		GLCallReturn(glDeleteBuffers(1, &VBO));
		GLCallReturn(glDeleteTextures(1, &texture));
		GLCallReturn(glDeleteProgram(shader));
	}
	void Init(float positions[], const int size, int usage) {
		GLCallReturn(glDeleteVertexArrays(1, &VAO));
		GLCallReturn(glDeleteBuffers(1, &VBO));

		GLCallReturn(glGenVertexArrays(1, &VAO));
		GLCallReturn(glGenBuffers(1, &VBO));

		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, size, positions, usage));
	};
	void Init(float positions[], const int size, unsigned int indices[], const int sizeind, int usage) {
		GLCallReturn(glDeleteVertexArrays(1, &VAO));
		GLCallReturn(glDeleteBuffers(1, &VBO));
		GLCallReturn(glDeleteBuffers(1, &EBO));

		GLCallReturn(glGenVertexArrays(1, &VAO));
		GLCallReturn(glGenBuffers(1, &VBO));
		GLCallReturn(glGenBuffers(1, &EBO));

		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, size, positions, usage));

		GLCallReturn(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCallReturn(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, indices, usage));
	};
	void Config(int usage, bool use_indices = true) {
		config_with_indices = use_indices;
		GLCallReturn(glDeleteVertexArrays(1, &VAO));
		GLCallReturn(glDeleteBuffers(1, &VBO));
		if (use_indices) GLCallReturn(glDeleteBuffers(1, &EBO));

		GLCallReturn(glGenVertexArrays(1, &VAO));
		GLCallReturn(glGenBuffers(1, &VBO));
		if(use_indices) GLCallReturn(glGenBuffers(1, &EBO));

		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), usage));

		if (use_indices) {
			GLCallReturn(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
			GLCallReturn(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), usage));
		}
	}
	void ReConfig(int usage) {
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), usage));
		if (config_with_indices) {
			GLCallReturn(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
			GLCallReturn(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), usage));
		}
	}
	void Reload(float positions[], const int size) {
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW));
	}
	void Reload(float positions[], const int size, unsigned int indices[], const int sizeind) {
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW));

		GLCallReturn(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCallReturn(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, indices, GL_STATIC_DRAW));
	}
	void LoadMatrix() {
		Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void VertexAttribpointer(int index, int start, int count, int jump, int type) {
		GLCallReturn(glVertexAttribPointer(index, count, type, GL_FALSE, jump * sizeof(float), (void*)(start * sizeof(float))));
		GLCallReturn(glEnableVertexAttribArray(index));
	}
	void Shader(const char* filename) {
		ShaderProgramSource source = ParseShader(filename);
		shader = CreateShader(source.VertexSource, source.FragmentSource);
		//GLCallReturn(glUseProgram(shader));

		location = glGetUniformLocation(shader, locationname.c_str());
		
		

		Matrix = perspective * camera * (ortho * translate * rotate * scale); // LoadMatrix();
	}
	void Texture(const char* filename, int format, bool flip, float WRAP_S_param = GL_CLAMP_TO_EDGE, float WRAP_T_param = GL_CLAMP_TO_EDGE, float MIN_FILTER_param = GL_LINEAR_MIPMAP_LINEAR, float  MAG_FILTER_param = GL_LINEAR_MIPMAP_LINEAR) {
		GLCallReturn(glDeleteTextures(1, &texture));

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(flip);
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

		if (!data) {
			cout << "\033[3;40;31m[Error::Texture: " << filename << " we can't open it]\033[0m" << endl;
		}

		GLCallReturn(glGenTextures(1, &texture));
		GLCallReturn(glBindTexture(GL_TEXTURE_2D, texture));

		//GLCallReturn(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		//GLCallReturn(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		//GLCallReturn(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		//GLCallReturn(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		//GLCallReturn(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		//GLCallReturn(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		//GLCallReturn(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		//GLCallReturn(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCallReturn(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAP_S_param));
		GLCallReturn(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAP_T_param));
		GLCallReturn(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIN_FILTER_param));
		GLCallReturn(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MAG_FILTER_param));

		GLCallReturn(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));

		GLCallReturn(glGenerateMipmap(GL_TEXTURE_2D));

		stbi_image_free(data);
	}
	void Perspective(double fovy, double aspect, double zNear, double zFar) {
		perspective = glm::mat4(1.0);
		perspective = glm::perspective(fovy, aspect, zNear, zFar);
		Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
		camera = glm::mat4(1.0);
		camera = glm::lookAt(eye, center, up);
		Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Ortho(double left, double right, double bottmom, double top, double zNear, double zFar) {
		ortho = glm::mat4(1.0);
		ortho = glm::ortho(left, right, bottmom, top, zNear, zFar);
		Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Translate(glm::vec3 pos, bool repeat) {
		if (!repeat) translate = glm::mat4(1.0);
		translate = glm::translate(translate, pos);
		Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Scale(glm::vec3 dim, bool repeat) {
		if (!repeat) scale = glm::mat4(1.0);
		scale = glm::scale(scale, dim);
		Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Rotate(float angle, glm::vec3 dir, bool keep) {
		if (!keep) {
			rotate = glm::mat4(1.0);
		}
		rotate = glm::rotate(rotate, angle, dir);
		Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Bind(int mode, int start, int count) {
		GLCallReturn(glUseProgram(shader));
		GLCallReturn(glBindTexture(GL_TEXTURE_2D, texture));
		GLCallReturn(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix)));
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glDrawArrays(mode, start, count));
		GLCallReturn(glBindVertexArray(0));
	}
	void Bind(int mode, int count, int type, const void* indices) {
		GLCallReturn(glUseProgram(shader));
		GLCallReturn(glBindTexture(GL_TEXTURE_2D, texture));
		GLCallReturn(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix)));
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glDrawElements(mode, count, type, indices));
		GLCallReturn(glBindVertexArray(0));
	}
	void Bind(int mode) {
		GLCallReturn(glUseProgram(shader));
		GLCallReturn(glBindTexture(GL_TEXTURE_2D, texture));
		GLCallReturn(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix)));
		GLCallReturn(glBindVertexArray(VAO));
		if (config_with_indices) { GLCallReturn(glDrawElements(mode, Indices.size(), GL_UNSIGNED_INT, 0)); }
		else { GLCallReturn(glDrawArrays(mode, 0, Vertices.size())); }
		GLCallReturn(glBindVertexArray(0));
	}
	void UnBind() {
		GLCallReturn(glBindVertexArray(0));
	}
};



class ObjFile {
private:
	class Kit {
	public:
		vector<float> Vertices;
		vector<unsigned int> Indices;
		string texture;
	};
	void Init() {
		string texture;
		if (Textures.size() != 0) {
			texture = Location + Textures.front();
			Textures.pop();
		}
		KitVector.push_back({Vertices, Indices, texture});
	}
	queue<string> Textures;
	vector<float> Vertices;
	vector<unsigned int> Indices;
public:
	vector<Kit> KitVector;
	string Location;
	void LoadModel(const char* objfile, const char * mtlfile = nullptr) {
		FILE* objmodel = fopen(objfile, "r");
		if (mtlfile != nullptr) {
			FILE * mtlmodel = fopen(mtlfile, "r");

			char val[128];

			while (true) {
				int res = fscanf(mtlmodel, "%s", val);
				if (res == EOF) break;
				if (strcmp(val, "map_Kd") == 0) {
					fscanf(mtlmodel, "%s", val);
					Textures.push(val);
				}
			}
		}

		vector<glm::vec3> vertices;
		vector<glm::vec2> vtextures;
		vector<glm::vec3> vnormals;

		char val[128];
		long cnt = 0;
		bool newobject = false;
		long vsize = 0, vtsize = 0, vnsize = 0;

		while (true) {
			int res = fscanf(objmodel, "%s", val);
			if (res == EOF) break;

			if (strcmp(val, "usemtl") == 0 && newobject) {
				Init();
				cnt = 0;
				Indices.clear();
				Vertices.clear();
				vsize += vertices.size();
				vertices.clear();
				vtsize += vtextures.size();
				vtextures.clear();
				vnsize += vnormals.size();
				vnormals.clear();
			}
			else if (strcmp(val, "v") == 0) {
				glm::vec3 v;
				fscanf(objmodel, "%f %f %f", &v.x, &v.y, &v.z);
				vertices.push_back(v);
			}
			else if (strcmp(val, "vt") == 0) {
				glm::vec2 vt;
				fscanf(objmodel, "%f %f", &vt.x, &vt.y);
				vtextures.push_back(vt);
			}
			else if (strcmp(val, "vn") == 0) {
				glm::vec3 vn;
				fscanf(objmodel, "%f %f %f", &vn.x, &vn.y, &vn.z);
				vnormals.push_back(vn);
			}
			else if (strcmp(val, "f") == 0) {
				unsigned int v, vt, vn;
				for (int i = 0; i < 3; ++i) {
					fscanf(objmodel, "%d/%d/%d", &v, &vt, &vn);
					Indices.push_back(cnt);
					++cnt;
					Vertices.push_back(vertices[v - vsize - 1].x);
					Vertices.push_back(vertices[v - vsize - 1].y);
					Vertices.push_back(vertices[v - vsize - 1].z);
					Vertices.push_back(vtextures[vt - vtsize - 1].x);
					Vertices.push_back(vtextures[vt - vtsize - 1].y);
					Vertices.push_back(vnormals[vn - vnsize - 1].x);
					Vertices.push_back(vnormals[vn - vnsize - 1].y);
					Vertices.push_back(vnormals[vn - vnsize - 1].z);
				}
				newobject = true;
			}
		}

		vertices.clear();
		vtextures.clear();
		vnormals.clear();
		Init();
		fclose(objmodel);
	}
	void OpenFile(const char * modelname) {
		FILE* objfile = fopen(modelname, "r");


		char val[128];

		while (true) {
			int res = fscanf(objfile, "%s", val);
			if (res == EOF) break;

			if (strcmp(val, "v") == 0) {
				glm::vec3 v;
				fscanf(objfile, "%f %f %f", &v.x, &v.y, &v.z);
				cout << "\033[3;40;31m";
				cout << val << " ";
				cout << v.x << " " << v.y << " " << v.z << "\n";
				cout << "\033[0m";
			}
			else if (strcmp(val, "vt") == 0) {
				glm::vec2 vt;
				fscanf(objfile, "%f %f", &vt.x, &vt.y);
				cout << "\033[3;40;32m";
				cout << val << " ";
				cout << vt.x << " " << vt.y << "\n";
				cout << "\033[0m";
			}
			else if (strcmp(val, "vn") == 0) {
				glm::vec3 vn;
				fscanf(objfile, "%f %f %f", &vn.x, &vn.y, &vn.z);
				cout << "\033[3;40;33m";
				cout << val << " ";
				cout << vn.x << " " << vn.y << " " << vn.z << "\n";
				cout << "\033[0m";
			}
			else if (strcmp(val, "f") == 0) {
				unsigned int v, vt, vn;
				cout << "\033[3;40;34m";
				cout << val << " ";
				for (int i = 0; i < 3; ++i) {
					fscanf(objfile, "%d/%d/%d", &v, &vt, &vn);
					cout << v << " " << vt << " " << vn << " ";
				}
				cout << "\n";
				cout << "\033[0m";
			}
			else cout << val << " ";
		}
		fclose(objfile);
	}
};

#endif
#endif // STB_IMAGE_IMPLEMENTATION