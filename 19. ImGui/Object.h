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


#ifdef INCLUDE_ERROR_LIBRARY
#ifdef INCLUDE_SHADER_LIBRARY
#ifdef STB_IMAGE_IMPLEMENTATION

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
# CONTENT:
#	-> Object();
#	-> Destruct();
#	-> Init();
#	-> Reload();
#	-> LoadMatrix();
#	-> VertexAttribPointer();
#	-> Shader();
#	-> Texture();
#	-> Perspective();
#	-> Camera();
#	-> Ortho();
#	-> Translate();
#	-> Scale();
#	-> Rotate();
#	-> Bind();
#	-> UnBind();
#
#
###############################################################################*/

class Object {
protected:
	unsigned int VAO, VBO, EBO;
	int location;
	glm::mat4 keeprotate = glm::mat4(1.0);
public:
	unsigned int texture, shader;
	glm::mat4 Matrix = glm::mat4(1.0);
	glm::mat4 perspective = glm::mat4(1.0);
	glm::mat4 camera = glm::mat4(1.0);
	glm::mat4 ortho = glm::mat4(1.0);
	glm::mat4 translate = glm::mat4(1.0);
	glm::mat4 rotate = glm::mat4(1.0);
	glm::mat4 scale = glm::mat4(1.0);

	Object() {}
	void Destruct() {
		GLCall(glDeleteVertexArrays(1, &VAO));
		GLCall(glDeleteBuffers(1, &VBO));
		GLCall(glDeleteTextures(1, &texture));
		GLCall(glDeleteProgram(shader));
	}
	void Init(float positions[], const int size) {
		GLCall(glDeleteVertexArrays(1, &VAO));
		GLCall(glDeleteBuffers(1, &VBO));

		GLCall(glGenVertexArrays(1, &VAO));
		GLCall(glGenBuffers(1, &VBO));

		GLCall(glBindVertexArray(VAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW));
	};
	void Init(float positions[], const int size, unsigned int indices[], const int sizeind) {
		GLCall(glDeleteVertexArrays(1, &VAO));
		GLCall(glDeleteBuffers(1, &VBO));
		GLCall(glDeleteBuffers(1, &EBO));

		GLCall(glGenVertexArrays(1, &VAO));
		GLCall(glGenBuffers(1, &VBO));
		GLCall(glGenBuffers(1, &EBO));

		GLCall(glBindVertexArray(VAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, indices, GL_STATIC_DRAW);
	};
	void Reload(float positions[], const int size) {
		GLCall(glBindVertexArray(VAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW));
	}
	void Reload(float positions[], const int size, unsigned int indices[], const int sizeind) {
		GLCall(glBindVertexArray(VAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, indices, GL_STATIC_DRAW);
	}
	void LoadMatrix() {
		Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void VertexAttribpointer(int index, int start, int count, int jump, int type) {
		GLCall(glVertexAttribPointer(index, count, type, GL_FALSE, jump * sizeof(float), (void*)(start * sizeof(float))));
		GLCall(glEnableVertexAttribArray(index));
	}
	void Shader(const char* filename) {
		ShaderProgramSource source = ParseShader(filename);
		shader = CreateShader(source.VertexSource, source.FragmentSource);
		GLCall(glUseProgram(shader));

		location = glGetUniformLocation(shader, "Matrix");

		Matrix = perspective * camera * (ortho * translate * rotate * scale); // LoadMatrix();
	}
	void Texture(const char* filename, int format, bool flip) {
		GLCall(glDeleteTextures(1, &texture));

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(flip);
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

		GLCall(glGenTextures(1, &texture));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));

		GLCall(glGenerateMipmap(GL_TEXTURE_2D));

		stbi_image_free(data);
	}
	void Perspective(double fovy, double aspect, double zNear, double zFar) {
		perspective = glm::mat4(1.0);
		perspective = glm::perspective(fovy, aspect, zNear, zFar);
		Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
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
	void UnBind() {
		GLCall(glBindVertexArray(0));
	}
};
#endif // STB_IMAGE_IMPLEMENTATION
#endif // INCLUDE_SHADER_LIBRARY
#endif // INCLUDE_ERROR_LIBRARY