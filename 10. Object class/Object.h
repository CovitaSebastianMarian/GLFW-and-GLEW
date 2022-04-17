#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include "stb_image.h"
#include <vector>
#include "Shader.h"
#include "error.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

using namespace std;

class Object {
private:
	unsigned int VAO, VBO, texture, shader;
	int location;
	glm::mat4 PT = glm::mat4(1.0);
	glm::mat4 perspective = glm::mat4(1.0);
	glm::mat4 camera = glm::mat4(1.0);
	glm::mat4 ortho = glm::mat4(1.0);
	glm::mat4 translate = glm::mat4(1.0);
	glm::mat4 rotate = glm::mat4(1.0);
	glm::mat4 scale = glm::mat4(1.0);
	

public:

	Object() {}
	void Init(float positions[], const int size) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);

	};
	void VertexAttribpointer(int index, int start, int count, int jump, int type) {
		glVertexAttribPointer(index, count, type, GL_FALSE, jump * sizeof(float), (void*)(start * sizeof(float)));
		glEnableVertexAttribArray(index);
	}
	void Shader(const char * filename) {
		ShaderProgramSource source = ParseShader(filename);
		shader = CreateShader(source.VertexSource, source.FragmentSource);
		glUseProgram(shader);

		location = glGetUniformLocation(shader, "Matrix");

		PT = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Texture(const char * filename, bool flip) {
		glDeleteTextures(1, &texture);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(flip);
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	template<class T> void Perspective(T fovy, T aspect, T zNear, T zFar) {
		perspective = glm::mat4(1.0);
		perspective = glm::perspective(fovy, aspect, zNear, zFar);
		//glm::mat4 PT = glm::mat4(1.0);
		PT = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
		camera = glm::mat4(1.0);
		camera = glm::lookAt(eye, center, up);
		//glm::mat4 PT = glm::mat4(1.0);
		PT = perspective * camera * (ortho * translate * rotate * scale);
	}
	template <class T> void Ortho(T left, T right, T bottmom, T top, T zNear, T zFar) {
		ortho = glm::mat4(1.0);
		ortho = glm::ortho(left, right, bottmom, top, zNear, zFar);
		//glm::mat4 PT = glm::mat4(1.0);
		PT = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Translate(glm::vec3 pos, bool repeat) {
		if(!repeat) translate = glm::mat4(1.0);
		translate = glm::translate(translate, pos);
		//glm::mat4 PT = glm::mat4(1.0);
		PT = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Scale(glm::vec3 dim, bool repeat) {
		if (!repeat) scale = glm::mat4(1.0);
		scale = glm::scale(scale, dim);
		//glm::mat4 PT = glm::mat4(1.0);
		PT = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Rotate(float angle, glm::vec3 dir, bool repeat) {
		if(!repeat)rotate = glm::mat4(1.0);
		rotate = glm::rotate(rotate, (float)glm::radians(angle), dir);
		//glm::mat4 PT = glm::mat4(1.0);
		PT = perspective * camera * (ortho * translate * rotate * scale);
	}

	void Bind() {
		glUseProgram(shader);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(PT));
		glBindVertexArray(VAO);
		glDrawArrays(GL_QUADS, 0, 4);
	}
};