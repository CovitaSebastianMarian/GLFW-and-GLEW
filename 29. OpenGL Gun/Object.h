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
#include <list>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>



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
public:
	vector<float> VectorPositions;
	vector<unsigned int> VectorIndices;
	unsigned int texture, shader;
	glm::mat4 Matrix = glm::mat4(1.0);
	glm::mat4 perspective = glm::mat4(1.0);
	glm::mat4 lookat = glm::mat4(1.0);
	glm::mat4 ortho = glm::mat4(1.0);
	glm::mat4 translate = glm::mat4(1.0);
	glm::mat4 rotate = glm::mat4(1.0);
	glm::mat4 scale = glm::mat4(1.0);

	Object() {}
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
	void DynamicInit(int usage) {
		GLCallReturn(glDeleteVertexArrays(1, &VAO));
		GLCallReturn(glDeleteBuffers(1, &VBO));

		GLCallReturn(glGenVertexArrays(1, &VAO));
		GLCallReturn(glGenBuffers(1, &VBO));

		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, VectorPositions.size() * sizeof(float), VectorPositions.data(), usage));
		
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
	void DynamicInitInd(int usage) {
		GLCallReturn(glDeleteVertexArrays(1, &VAO));
		GLCallReturn(glDeleteBuffers(1, &VBO));
		GLCallReturn(glDeleteBuffers(1, &EBO));

		GLCallReturn(glGenVertexArrays(1, &VAO));
		GLCallReturn(glGenBuffers(1, &VBO));
		GLCallReturn(glGenBuffers(1, &EBO));

		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, VectorPositions.size() * sizeof(float), VectorPositions.data(), usage));

		GLCallReturn(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCallReturn(glBufferData(GL_ELEMENT_ARRAY_BUFFER, VectorIndices.size() * sizeof(unsigned int), VectorIndices.data(),  usage));
	};
	void Reload(float positions[], const int size, int usage) {
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, size, positions, usage));
	}
	void Reload(float positions[], const int size, unsigned int indices[], const int sizeind, int usage) {
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, size, positions, usage));

		GLCallReturn(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCallReturn(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, indices, usage));
	}
	void DynamicReload(int usage) {
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, VectorPositions.size() * sizeof(float), VectorPositions.data(), usage));
	}
	void DynamicReloadInd(int usage) {
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, VectorPositions.size() * sizeof(float), VectorPositions.data(), usage));

		GLCallReturn(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCallReturn(glBufferData(GL_ELEMENT_ARRAY_BUFFER, VectorIndices.size() * sizeof(float), VectorIndices.data(), usage));
	}
	void LoadMatrix() {
		Matrix = perspective * lookat * (ortho * translate * rotate * scale);
	}
	void VertexAttribpointer(int index, int start, int count, int jump, int type) {
		GLCallReturn(glVertexAttribPointer(index, count, type, GL_FALSE, jump * sizeof(float), (void*)(start * sizeof(float))));
		GLCallReturn(glEnableVertexAttribArray(index));
	}
	void Shader(const char* filename) {
		ShaderProgramSource source = ParseShader(filename);
		shader = CreateShader(source.VertexSource, source.FragmentSource);
		//GLCallReturn(glUseProgram(shader));

		location = glGetUniformLocation(shader, "Matrix");

		Matrix = perspective * lookat * (ortho * translate * rotate * scale); // LoadMatrix();
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
	void Bind(int mode, int count, int type, const void* indices) {
		GLCallReturn(glUseProgram(shader));
		GLCallReturn(glBindTexture(GL_TEXTURE_2D, texture));
		GLCallReturn(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix)));
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glDrawElements(mode, count, type, indices));
		GLCallReturn(glBindVertexArray(0));
	}
	void UnBind() {
		GLCallReturn(glBindVertexArray(0));
	}
};
#endif
#endif // STB_IMAGE_IMPLEMENTATION