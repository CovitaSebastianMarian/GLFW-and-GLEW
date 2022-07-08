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

class Materials {
private:
	class Emerald {
	public:
		const glm::vec3 ambient = glm::vec3(0.0215, 0.1745, 0.0215);
		const glm::vec3 diffuse = glm::vec3(0.07568, 0.61424, 0.07568);
		const glm::vec3 specular = glm::vec3(0.633, 0.727811, 0.633);
		const float shininess = 0.6 * 128;
	};
	class Jade {
	public:
		const glm::vec3 ambient = glm::vec3(0.135, 0.2225, 0.1575);
		const glm::vec3 diffuse = glm::vec3(0.54, 0.89, 0.63);
		const glm::vec3 specular = glm::vec3(0.316228, 0.316228, 0.316228);
		const float shininess = 0.1 * 128;
	};
	class Obsidian {
	public:
		const glm::vec3 ambient = glm::vec3(0.05375, 0.05, 0.06625);
		const glm::vec3 diffuse = glm::vec3(0.18275, 0.17, 0.22525);
		const glm::vec3 specular = glm::vec3(0.332741, 0.328634, 0.346435);
		const float shininess = 0.3 * 128;
	};
	class Pearl {
	public:
		const glm::vec3 ambient = glm::vec3(0.25, 0.20725, 0.20725);
		const glm::vec3 diffuse = glm::vec3(1, 0.829, 0.829);
		const glm::vec3 specular = glm::vec3(0.296648, 0.296648, 0.296648);
		const float shininess = 0.88 * 128;
	};
	class Ruby {
	public:
		const glm::vec3 ambient = glm::vec3(0.1745, 0.01175, 0.01175);
		const glm::vec3 diffuse = glm::vec3(0.61424, 0.04136, 0.04136);
		const glm::vec3 specular = glm::vec3(0.727811, 0.626959, 0.626959);
		const float shininess = 0.6 * 128;
	};
	class Turquoise {
	public:
		const glm::vec3 ambient = glm::vec3(0.1, 0.18725, 0.1745);
		const glm::vec3 diffuse = glm::vec3(0.396, 0.74151, 0.69102);
		const glm::vec3 specular = glm::vec3(0.297254, 0.30829, 0.306678);
		const float shininess = 0.1 * 128;
	};
	class Brass {
	public:
		const glm::vec3 ambient = glm::vec3(0.329412, 0.223529, 0.027451);
		const glm::vec3 diffuse = glm::vec3(0.780392, 0.568627, 0.113725);
		const glm::vec3 specular = glm::vec3(0.992157, 0.941176, 0.807843);
		const float shininess = 0.21794872 * 128;
	};
	class Bronze {
	public:
		const glm::vec3 ambient = glm::vec3(0.2125, 0.1275, 0.054);
		const glm::vec3 diffuse = glm::vec3(0.714, 0.4284, 0.18144);
		const glm::vec3 specular = glm::vec3(0.393548, 0.271906, 0.166721);
		const float shininess = 0.2 * 128;
	};
	class Chrome {
	public:
		const glm::vec3 ambient = glm::vec3(0.25, 0.25, 0.25);
		const glm::vec3 diffuse = glm::vec3(0.4, 0.4, 0.4);
		const glm::vec3 specular = glm::vec3(0.774597, 0.774597, 0.774597);
		const float shininess = 0.6 * 128;
	};
	class Copper {
	public:
		const glm::vec3 ambient = glm::vec3(0.19125, 0.0735, 0.0225);
		const glm::vec3 diffuse = glm::vec3(0.7038, 0.27048, 0.0828);
		const glm::vec3 specular = glm::vec3(0.256777, 0.137622, 0.086014);
		const float shininess = 0.1 * 128;
	};
	class Gold {
	public:
		const glm::vec3 ambient = glm::vec3(0.24725, 0.1995, 0.0745);
		const glm::vec3 diffuse = glm::vec3(0.75164, 0.60648, 0.22648);
		const glm::vec3 specular = glm::vec3(0.628281, 0.555802, 0.366065);
		const float shininess = 0.4 * 128;
	};
	class Silver {
	public:
		const glm::vec3 ambient = glm::vec3(0.19225, 0.19225, 0.19225);
		const glm::vec3 diffuse = glm::vec3(0.50754, 0.50754, 0.50754);
		const glm::vec3 specular = glm::vec3(0.508273, 0.508273, 0.508273);
		const float shininess = 0.4 * 128;
	};
	class Black_plastic {
	public:
		const glm::vec3 ambient = glm::vec3(0.0, 0.0, 0.0);
		const glm::vec3 diffuse = glm::vec3(0.01, 0.01, 0.01);
		const glm::vec3 specular = glm::vec3(0.50, 0.50, 0.50);
		const float shininess = 0.25 * 128;
	};
	class Cyan_plastic {
	public:
		const glm::vec3 ambient = glm::vec3(0.0, 0.1, 0.06);
		const glm::vec3 diffuse = glm::vec3(0.0, 0.50980392, 0.50980392);
		const glm::vec3 specular = glm::vec3(0.50196078, 0.50196078, 0.50196078);
		const float shininess = 0.25 * 128;
	};
	class Green_plastic {
	public:
		const glm::vec3 ambient = glm::vec3(0.0, 0.0, 0.0);
		const glm::vec3 diffuse = glm::vec3(0.1, 0.35, 0.1);
		const glm::vec3 specular = glm::vec3(0.45, 0.55, 0.45);
		const float shininess = 0.25 * 128;
	};
	class Red_plastic {
	public:
		const glm::vec3 ambient = glm::vec3(0.0, 0.0, 0.0);
		const glm::vec3 diffuse = glm::vec3(0.5, 0.0, 0.0);
		const glm::vec3 specular = glm::vec3(0.7, 0.6, 0.6);
		const float shininess = 0.25 * 128;
	};
	class White_plastic {
	public:
		const glm::vec3 ambient = glm::vec3(0.0, 0.0, 0.0);
		const glm::vec3 diffuse = glm::vec3(0.55, 0.55, 0.55);
		const glm::vec3 specular = glm::vec3(0.70, 0.70, 0.70);
		const float shininess = 0.25 * 128;
	};
	class Yellow_plastic {
	public:
		const glm::vec3 ambient = glm::vec3(0.0, 0.0, 0.0);
		const glm::vec3 diffuse = glm::vec3(0.5, 0.5, 0.0);
		const glm::vec3 specular = glm::vec3(0.60, 0.60, 0.50);
		const float shininess = 0.25 * 128;
	};
	class Black_rubber {
	public:
		const glm::vec3 ambient = glm::vec3(0.02, 0.02, 0.02);
		const glm::vec3 diffuse = glm::vec3(0.01, 0.01, 0.01);
		const glm::vec3 specular = glm::vec3(0.4, 0.4, 0.4);
		const float shininess = 0.078125 * 128;
	};
	class Cyan_rubber {
	public:
		const glm::vec3 ambient = glm::vec3(0.0, 0.05, 0.05);
		const glm::vec3 diffuse = glm::vec3(0.4, 0.5, 0.5);
		const glm::vec3 specular = glm::vec3(0.04, 0.7, 0.7);
		const float shininess = 0.078125 * 128;
	};
	class Green_rubber {
	public:
		const glm::vec3 ambient = glm::vec3(0.0, 0.05, 0.0);
		const glm::vec3 diffuse = glm::vec3(0.4, 0.5, 0.4);
		const glm::vec3 specular = glm::vec3(0.04, 0.7, 0.04);
		const float shininess = 0.078125 * 128;
	};
	class Red_rubber {
	public:
		const glm::vec3 ambient = glm::vec3(0.05, 0.0, 0.0);
		const glm::vec3 diffuse = glm::vec3(0.5, 0.4, 0.4);
		const glm::vec3 specular = glm::vec3(0.7, 0.04, 0.04);
		const float shininess = 0.078125 * 128;
	};
	class White_rubber {
	public:
		const glm::vec3 ambient = glm::vec3(0.05, 0.05, 0.05);
		const glm::vec3 diffuse = glm::vec3(0.5, 0.5, 0.5);
		const glm::vec3 specular = glm::vec3(0.7, 0.7, 0.7);
		const float shininess = 0.078125 * 128;
	};
	class Yellow_rubber {
	public:
		const glm::vec3 ambient = glm::vec3(0.05, 0.05, 0.05);
		const glm::vec3 diffuse = glm::vec3(0.5, 0.5, 0.4);
		const glm::vec3 specular = glm::vec3(0.7, 0.7, 0.04);
		const float shininess = 0.078125 * 128;
	};

public:
	Emerald emerald;
	Jade jade;
	Obsidian obsidian;
	Pearl pearl;
	Ruby ruby;
	Turquoise turquoise;
	Brass brass;
	Bronze bronze;
	Chrome chrome;
	Copper copper;
	Gold gold;
	Silver silver;
	Black_plastic black_plastic;
	Cyan_plastic cyan_plastic;
	Green_plastic green_plastic;
	Red_plastic red_plastic;
	White_plastic white_plastic;
	Yellow_plastic yellow_plastic;
	Black_rubber black_rubber;
	Cyan_rubber cyan_rubber;
	Green_rubber green_rubber;
	Red_rubber red_rubber;
	White_rubber white_rubber;
	Yellow_rubber yellow_rubber;
} Material;


class Object {
protected:
	unsigned int VAO, VBO, EBO;
	int location;
	int mlocation;
	class Cam {
	public:
		glm::vec3 eye = glm::vec3(0);
		glm::vec3 center = glm::vec3(0);
		glm::vec3 up = glm::vec3(0);
	};
	
public:
	Cam Camera;
	vector<float> Vertices;
	vector<unsigned int> Indices;
	unsigned int texture, shader;
	glm::mat4 Matrix = glm::mat4(1.0);
	glm::mat4 Model = glm::mat4(1.0);
	glm::mat4 perspective = glm::mat4(1.0);
	glm::mat4 camera = glm::mat4(1.0);
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
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), usage));

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
	void Config(int usage) {
		GLCallReturn(glDeleteVertexArrays(1, &VAO));
		GLCallReturn(glDeleteBuffers(1, &VBO));
		GLCallReturn(glDeleteBuffers(1, &EBO));

		GLCallReturn(glGenVertexArrays(1, &VAO));
		GLCallReturn(glGenBuffers(1, &VBO));
		GLCallReturn(glGenBuffers(1, &EBO));

		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCallReturn(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), usage));

		GLCallReturn(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCallReturn(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), usage));
	};
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
		Model = glm::mat4(1.0);
		Matrix = glm::mat4(1.0);
		Model = ortho * translate * rotate * scale;
		Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void VertexAttribpointer(int index, int start, int count, int jump, int type) {
		GLCallReturn(glVertexAttribPointer(index, count, type, GL_FALSE, jump * sizeof(float), (void*)(start * sizeof(float))));
		GLCallReturn(glEnableVertexAttribArray(index));
	}
	void Shader(const char* filename) {
		ShaderProgramSource source = ParseShader(filename);
		shader = CreateShader(source.VertexSource, source.FragmentSource);
		GLCallReturn(glUseProgram(shader));

		location = glGetUniformLocation(shader, "Matrix");
		mlocation = glGetUniformLocation(shader, "Model");

		LoadMatrix();
	}

	void setVec(const char* name, float x) {
		int loc = glGetUniformLocation(shader, name);
		glUniform1f(loc, x);
	}
	void setVec(const char* name, float x, float y) {
		int loc = glGetUniformLocation(shader, name);
		glUniform2f(loc, x, y);
	}
	void setVec(const char* name, float x, float y, float z) {
		glUseProgram(shader);
		int loc = glGetUniformLocation(shader, name);
		glUniform3f(loc, x, y, z);
	}
	void setVec(const char* name, float x, float y, float z, float w) {
		int loc = glGetUniformLocation(shader, name);
		glUniform4f(loc, x, y, z, w);
	}

	void setVec(const char* name, glm::vec1 x) {
		int loc = glGetUniformLocation(shader, name);
		glUniform1f(loc, x.x);
	}
	void setVec(const char* name, glm::vec2 x) {
		int loc = glGetUniformLocation(shader, name);
		glUniform2f(loc, x.x, x.y);
	}
	void setVec(const char* name, glm::vec3 x) {
		int loc = glGetUniformLocation(shader, name);
		glUniform3f(loc, x.x, x.y, x.z);
	}
	void setVec(const char* name, glm::vec4 x) {
		int loc = glGetUniformLocation(shader, name);
		glUniform4f(loc, x.x, x.y, x.z, x.w);
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
		LoadMatrix();
		//Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
		
		Camera.eye = glm::vec3(0);
		Camera.eye = eye;
		Camera.center = glm::vec3(0);
		Camera.center = center;
		Camera.up = glm::vec3(0);
		Camera.up = up;

		camera = glm::mat4(1.0);
		camera = glm::lookAt(eye, center, up);
		LoadMatrix();
		//Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Ortho(double left, double right, double bottmom, double top, double zNear, double zFar) {
		ortho = glm::mat4(1.0);
		ortho = glm::ortho(left, right, bottmom, top, zNear, zFar);
		LoadMatrix();
		//Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Translate(glm::vec3 pos, bool repeat) {
		if (!repeat) translate = glm::mat4(1.0);
		translate = glm::translate(translate, pos);
		LoadMatrix();
		//Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Scale(glm::vec3 dim, bool repeat) {
		if (!repeat) scale = glm::mat4(1.0);
		scale = glm::scale(scale, dim);
		LoadMatrix();
		//Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}
	void Rotate(float angle, glm::vec3 dir, bool keep) {
		if (!keep) {
			rotate = glm::mat4(1.0);
		}
		rotate = glm::rotate(rotate, angle, dir);
		LoadMatrix();
		//Matrix = perspective * camera * (ortho * translate * rotate * scale);
	}

	void Bind(int mode, int start, int count) {
		GLCallReturn(glUseProgram(shader));
		GLCallReturn(glBindTexture(GL_TEXTURE_2D, texture));
		GLCallReturn(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix)));
		GLCallReturn(glUniformMatrix4fv(mlocation, 1, GL_FALSE, glm::value_ptr(Model)));
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glDrawArrays(mode, start, count));
		GLCallReturn(glBindVertexArray(0));
	}
	void Bind(int mode, int count, int type, const void* indices) {
		GLCallReturn(glUseProgram(shader));
		GLCallReturn(glBindTexture(GL_TEXTURE_2D, texture));
		GLCallReturn(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix)));
		GLCallReturn(glUniformMatrix4fv(mlocation, 1, GL_FALSE, glm::value_ptr(Model)));
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glDrawElements(mode, count, type, indices));
		GLCallReturn(glBindVertexArray(0));
	}
	void Bind(int mode) {
		GLCallReturn(glUseProgram(shader));
		GLCallReturn(glBindTexture(GL_TEXTURE_2D, texture));
		GLCallReturn(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix)));
		GLCallReturn(glUniformMatrix4fv(mlocation, 1, GL_FALSE, glm::value_ptr(Model)));
		GLCallReturn(glBindVertexArray(VAO));
		GLCallReturn(glDrawElements(mode, Indices.size(), GL_UNSIGNED_INT, 0));
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
		string name;
		vector<float> Vertices;
		vector<unsigned int> Indices;
		string texture;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess, illum;
	} kit;


	vector<float> Vertices;
	vector<unsigned int> Indices;

	long count = -1;


	void OpenMTLfile(const char * filename) {
		FILE* file = fopen(filename, "r");

		if (file == nullptr) return;
		char val[128];

		int seconarycount = -1;

		while (true) {
			int res = fscanf(file, "%s", val);
			if (res == EOF) break;

			if (strcmp(val, "newmtl") == 0) {
				KitVector.push_back(kit);
				seconarycount++;
				fscanf(file, "%s", val);
				KitVector[seconarycount].name = val;
			}
			else if (strcmp(val, "illum") == 0) {
				float x;
				fscanf(file, "%f", &x);
				KitVector[seconarycount].illum = x;
			}
			else if (strcmp(val, "Ka") == 0) {
				float a, b, c;
				fscanf(file, "%f %f %f", &a, &b, &c);
				KitVector[seconarycount].ambient = glm::vec3(a, b, c);
			}
			else if (strcmp(val, "Kd") == 0) {
				float a, b, c;
				fscanf(file, "%f %f %f", &a, &b, &c);
				KitVector[seconarycount].diffuse = glm::vec3(a, b, c);
			}
			else if (strcmp(val, "Ks") == 0) {
				float a, b, c;
				fscanf(file, "%f %f %f", &a, &b, &c);
				KitVector[seconarycount].specular = glm::vec3(a, b, c);
			}
			else if (strcmp(val, "Ns") == 0) {
				float x;
				fscanf(file, "%f", &x);
				KitVector[seconarycount].shininess = x;
			}
			else if (strcmp(val, "map_Kd") == 0) {
				fscanf(file, "%s", val);
				char *tex = new char[ TextureLocation.length() + strlen(val)];
				strcpy(tex, TextureLocation.c_str());
				strcat(tex, val);
				KitVector[seconarycount].texture = tex;
			}
		}
	}

public:
	vector<Kit> KitVector;
	string TextureLocation;
	string MTLlocation;

	void LoadModel(const char* objfile) {
		FILE* objmodel = fopen(objfile, "r");

		vector<glm::vec3> vertices;
		vector<glm::vec2> vtextures;
		vector<glm::vec3> vnormals;

		char val[128];
		long cnt = 0;
		long vsize = 0, vtsize = 0, vnsize = 0;

		while (true) {
			int res = fscanf(objmodel, "%s", val);
			if (res == EOF) break;

			if (strcmp(val, "mtllib") == 0) {
				fscanf(objmodel, "%s", val);
				char* mtlfile = new char[MTLlocation.length() + strlen(val)];
				strcpy(mtlfile, MTLlocation.c_str());
				strcat(mtlfile, val);
				OpenMTLfile(mtlfile);
			}
			else if (strcmp(val, "usemtl") == 0) {
				fscanf(objmodel, "%s", val);
				cnt = 0;
				vsize += vertices.size();
				vertices.clear();
				vtsize += vtextures.size();
				vtextures.clear();
				vnsize += vnormals.size();
				vnormals.clear();
				count++;
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
					KitVector[count].Indices.push_back(cnt);
					++cnt;
					KitVector[count].Vertices.push_back(vertices[v - vsize - 1].x);
					KitVector[count].Vertices.push_back(vertices[v - vsize - 1].y);
					KitVector[count].Vertices.push_back(vertices[v - vsize - 1].z);
					KitVector[count].Vertices.push_back(vtextures[vt - vtsize - 1].x);
					KitVector[count].Vertices.push_back(vtextures[vt - vtsize - 1].y);
					KitVector[count].Vertices.push_back(vnormals[vn - vnsize - 1].x);
					KitVector[count].Vertices.push_back(vnormals[vn - vnsize - 1].y);
					KitVector[count].Vertices.push_back(vnormals[vn - vnsize - 1].z);

				}
			}
		}

		vertices.clear();
		vtextures.clear();
		vnormals.clear();
		fclose(objmodel);
	}
	void OpenFile(const char* modelname) {
		FILE* objfile = fopen(modelname, "r");


		char val[128];

		while (true) {
			int res = fscanf(objfile, "%s", val);
			if (res == EOF) break;


			if (strcmp(val, "mtllib") == 0) {
				fscanf(objfile, "%s", val);
				cout << "\033[3;40;35m";
				cout << val << "\n";
				cout << "####################################################################\n";
				char mtlfile[128];
				strcpy(mtlfile, "Models/");
				strcat(mtlfile, val);
				OpenFile(mtlfile);
				cout << "####################################################################\n";
				cout << "\033[0m";
			}
			else if (strcmp(val, "v") == 0) {
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