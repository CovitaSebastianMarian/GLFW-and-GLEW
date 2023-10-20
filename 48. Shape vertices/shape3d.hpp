#pragma once




class ShapeShader3D {
public:
	unsigned int ID;
	ShapeShader3D() {
		const char* vert_shader =
			"#version 330 core\n"
			"layout (location = 0) in vec3 pos;\n"
			"layout (location = 1) in vec2 texcoord;\n"
			"uniform mat4 matrix;\n"
			"out vec2 tcoord;\n"
			"void main()\n"
			"{ gl_Position = matrix * vec4(pos, 1.0);\n"
			"	tcoord = texcoord; }\n";


		const char* frag_shader =
			"#version 330 core\n"
			"in vec2 tcoord;\n"
			"uniform sampler2D Texture;\n"
			"void main() { gl_FragColor = texture(Texture, tcoord); }\n";


		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vert_shader, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &frag_shader, NULL);
		glCompileShader(fragment);

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	void set_matrix(const glm::mat4 mat) {
		glUniformMatrix4fv(glGetUniformLocation(ID, "matrix"), 1, GL_FALSE, &mat[0][0]);
	}
};


class Shape3D {
private:
	uint32_t VAO, VBO, texture;
public:
	Shape3D(const char* texture_file) {


		int32_t width, height, nrChannels;
		unsigned char* data = stbi_load(texture_file, &width, &height, &nrChannels, 0);


		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, NULL);



		float pos[] = {
		-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,

		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,

		-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,

		-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
		 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f, 1.0f
		};


		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		glBindVertexArray(NULL);

	}
	inline void draw(ShapeShader3D* shader, glm::mat4 matrix) {
		glUseProgram(shader->ID);

		shader->set_matrix(matrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(NULL);
		glBindTexture(GL_TEXTURE_2D, NULL);
		glUseProgram(NULL);
	}
};



ShapeShader3D* shape_shader = nullptr;
Shape3D* shape = nullptr;

void InitGame(int x, int y) {
	shape_shader = new ShapeShader3D();

	shape = new Shape3D("Images/concreate.jpg");

}



float xe = 0, ye = 0, ze = 1;
float xc = 0, yc = 0, zc = 0.1;
void Game(double w, double h) {


	ImGui::Begin("Console");
	if (ImGui::CollapsingHeader("Eye")) {
		ImGui::InputFloat("Xeye", &xe, 1);
		ImGui::InputFloat("Yeye", &ye, 1);
		ImGui::InputFloat("Zeye", &ze, 1);
	}
	if (ImGui::CollapsingHeader("Center")) {
		ImGui::InputFloat("Xcenter", &xc, 1);
		ImGui::InputFloat("Ycenter", &yc, 1);
		ImGui::InputFloat("Zcenter", &zc, 1);
	}
	ImGui::End();


	float time = glfwGetTime();

	glm::mat4 model = glm::ortho<double>(-w / 2, w / 2, -h / 2, h / 2, w / 2, -w / 2);
	model = glm::scale(model, glm::vec3(100));
	model = glm::rotate(model, glm::radians(time * 10), glm::vec3(1, 1, 1));

	glm::mat4 view = glm::lookAt(glm::vec3(xe, ye, ze), glm::vec3(xc, yc, zc), glm::vec3(0, 1, 0));

	glm::mat4 perspective = glm::perspective<double>(glm::radians(85.0f), h / w, 0.01, w);

	glm::mat4 matrix = perspective * view * model;


	shape->draw(shape_shader, matrix);

}
