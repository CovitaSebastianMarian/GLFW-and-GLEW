#pragma once
#include <GL/glew.h> // #include <glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include "stb_image.h"
#include <exception>
#include <vector>
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

class Object {
protected:
	unsigned int VAO, VBO, EBO;
	int location;

	vector<float> Model;
	vector<unsigned int> indices;
	long long nr = 0;
public:
	vector<glm::vec3> vertices;
	vector<glm::vec2> vtextures;
	vector<glm::vec3> vnormals;

	unsigned int texture, shader;
	glm::mat4 Matrix = glm::mat4(1.0);
	glm::mat4 perspective = glm::mat4(1.0);
	glm::mat4 lookat = glm::mat4(1.0);
	glm::mat4 ortho = glm::mat4(1.0);
	glm::mat4 translate = glm::mat4(1.0);
	glm::mat4 rotate = glm::mat4(1.0);
	glm::mat4 scale = glm::mat4(1.0);

	Object() {}


	void LoadModelTriangles(const char* modelname) {
		FILE *model = fopen(modelname, "r");

		bool newmodel = true;

		if (model == NULL) {
			cout << "ERROR\n";
		}
		char val[128];
		while (true) {
			int res = fscanf(model, "%s", val);
			if (res == EOF) break;

			if (strcmp(val, "v") == 0) {
				glm::vec3 v;
				fscanf(model, "%f %f %f\n", &v.x, &v.y, &v.z);
				vertices.push_back(v);
			}
			else if (strcmp(val, "vt") == 0) {
				glm::vec2 vt;
				fscanf(model, "%f %f\n", &vt.x, &vt.y);
				vtextures.push_back(vt);
			}
			else if (strcmp(val, "vn") == 0) {
				glm::vec3 vn;
				fscanf(model, "%f %f %f\n", &vn.x, &vn.y, &vn.z);
				vnormals.push_back(vn);
			}
			else if (strcmp(val, "f") == 0) {
				unsigned int v[3], vt[3], vn[3];
				fscanf(model, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v[0], &vt[0], &vn[0], &v[1], &vt[1], &vn[1], &v[2], &vt[2], &vn[2]);

				
				for (int i = 0; i < 3; ++i) {
					nr++;
					glm::vec3 vv = vertices[v[i] - 1];
					glm::vec2 vtt = vtextures[vt[i] - 1];
					glm::vec3 vnn = vnormals[vn[i] - 1];
					Model.push_back(vv.x);
					Model.push_back(vv.y);
					Model.push_back(vv.z);
					Model.push_back(vtt.x);
					Model.push_back(vtt.y);
					Model.push_back(vnn.x);
					Model.push_back(vnn.y);
					Model.push_back(vnn.z);
				}
			}

		}

		for (long long i = 0; i < nr; ++i) {
			indices.push_back(i);
		}

		vertices.clear();
		vtextures.clear();
		vnormals.clear();
		fclose(model);
	}

	
	void LoadModelQuads(const char* modelname) {
		FILE* model = fopen(modelname, "r");

		bool newmodel = true;

		if (model == NULL) {
			cout << "ERROR\n";
		}
		char val[128];
		while (true) {
			int res = fscanf(model, "%s", val);
			if (res == EOF) break;

			if (strcmp(val, "v") == 0) {
				glm::vec3 v;
				fscanf(model, "%f %f %f\n", &v.x, &v.y, &v.z);
				vertices.push_back(v);
			}
			else if (strcmp(val, "vt") == 0) {
				glm::vec2 vt;
				fscanf(model, "%f %f\n", &vt.x, &vt.y);
				vtextures.push_back(vt);
			}
			else if (strcmp(val, "vn") == 0) {
				glm::vec3 vn;
				fscanf(model, "%f %f %f\n", &vn.x, &vn.y, &vn.z);
				vnormals.push_back(vn);
			}
			else if (strcmp(val, "f") == 0) {
				unsigned int v[4], vt[4], vn[4];
				fscanf(model, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &v[0], &vt[0], &vn[0], &v[1], &vt[1], &vn[1], &v[2], &vt[2], &vn[2], &v[3], &vt[3], &vn[3]);

				for (int i = 0; i < 4; ++i) {
					nr++;
					glm::vec3 vv = vertices[v[i] - 1];
					glm::vec2 vtt = vtextures[vt[i] - 1];
					glm::vec3 vnn = vnormals[vn[i] - 1];
					Model.push_back(vv.x);
					Model.push_back(vv.y);
					Model.push_back(vv.z);
					Model.push_back(vtt.x);
					Model.push_back(vtt.y);
					Model.push_back(vnn.x);
					Model.push_back(vnn.y);
					Model.push_back(vnn.z);
				}
			}

		}

		for (long long i = 0; i < nr; ++i) {
			indices.push_back(i);
		}

		vertices.clear();
		vtextures.clear();
		vnormals.clear();
		fclose(model);
	}
	
	
	void Config(int usage) {
		GLCallReturn(glDeleteVertexArrays(1, &VAO));
		GLCallReturn(glDeleteBuffers(1, &VBO));
		GLCallReturn(glDeleteBuffers(1, &EBO));

		GLCallReturn(glGenVertexArrays(1, &VAO));
		GLCallReturn(glGenBuffers(1, &VBO));
		GLCallReturn(glGenBuffers(1, &EBO));

		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, Model.size() * sizeof(float), Model.data(), usage));

		GLCallReturn(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCallReturn(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage));
	}


	void LoadMatrix() {
		Matrix = perspective * lookat * (ortho * translate * rotate * scale);
	}
	void VertexAttribpointer(int index, int start, int count, int jump, int value) {
		GLCallReturn(glEnableVertexAttribArray(index));
		GLCallReturn(glVertexAttribPointer(index, count, value, GL_FALSE, jump * sizeof(float), (void*)(start * sizeof(float))));
	}
	void Shader(string filename) {
		ShaderProgramSource source = ParseShader(filename);
		shader = CreateShader(source.VertexSource, source.FragmentSource);
		//GLCallReturn(glUseProgram(shader));

		location = glGetUniformLocation(shader, "Matrix");

		Matrix = perspective * lookat * (ortho * translate * rotate * scale); // LoadMatrix();
	}
	void Texture(string filename, int format, bool flip, float WRAP_S_param = GL_CLAMP_TO_EDGE, float WRAP_T_param = GL_CLAMP_TO_EDGE, float MIN_FILTER_param = GL_LINEAR_MIPMAP_LINEAR, float  MAG_FILTER_param = GL_LINEAR_MIPMAP_LINEAR) {
		GLCallReturn(glDeleteTextures(1, &texture));

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(flip);
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

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
		Matrix = perspective * lookat * (ortho * translate * rotate * scale);
	}
	void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
		lookat = glm::mat4(1.0);
		lookat = glm::lookAt(eye, center, up);
		Matrix = perspective * lookat * (ortho * translate * rotate * scale);
	}
	void Ortho(double left, double right, double bottmom, double top, double zNear, double zFar) {
		ortho = glm::mat4(1.0);
		ortho = glm::ortho(left, right, bottmom, top, zNear, zFar);
		Matrix = perspective * lookat * (ortho * translate * rotate * scale);
	}
	void Translate(glm::vec3 pos, bool repeat) {
		if (!repeat) translate = glm::mat4(1.0);
		translate = glm::translate(translate, pos);
		Matrix = perspective * lookat * (ortho * translate * rotate * scale);
	}
	void Scale(glm::vec3 dim, bool repeat) {
		if (!repeat) scale = glm::mat4(1.0);
		scale = glm::scale(scale, dim);
		Matrix = perspective * lookat * (ortho * translate * rotate * scale);
	}
	void Rotate(float angle, glm::vec3 dir, bool keep) {
		if (!keep) {
			rotate = glm::mat4(1.0);
		}
		rotate = glm::rotate(rotate, angle, dir);
		Matrix = perspective * lookat * (ortho * translate * rotate * scale);
	}
	void Bind(int mode, int start, int count) {
		GLCallReturn(glUseProgram(shader));
		GLCallReturn(glBindTexture(GL_TEXTURE_2D, texture));
		GLCallReturn(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix)));
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glDrawArrays(mode, start, count));
		GLCallReturn(glBindVertexArray(0));
	}
	void Bind(int mode) {
		GLCallReturn(glUseProgram(shader));
		GLCallReturn(glBindTexture(GL_TEXTURE_2D, texture));
		GLCallReturn(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix)));
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0));
		GLCallReturn(glBindVertexArray(0));
	}
	void UnBind() {
		GLCallReturn(glBindVertexArray(0));
	}
};
#endif
#endif // STB_IMAGE_IMPLEMENTATION