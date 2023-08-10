#pragma once



class RectangleShader {
public:
	unsigned int ID;
	RectangleShader() {
		const char* vert_shader =
			"#version 330 core\n"
			"layout (location = 0) in vec3 pos;\n"
			"uniform mat4 matrix;\n"
			"void main() { gl_Position = matrix * vec4(pos, 1.0); }\n";

		const char* frag_shader =
			"#version 330 core\n"
			"uniform vec3 color;\n"
			"void main() {\n"
			"	gl_FragColor = vec4(color, 1);}\n";

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
	void set_color(const glm::vec3 col)  {
		glUniform3fv(glGetUniformLocation(ID, "color"), 1, &col[0]);
	}
};


class Rectangle {
private:
	unsigned int VAO, VBO;

public:
	Rectangle() {
		/*
		float pos[] = {
			-1, -1, -1, //0
			1, -1, -1,  //1
			1, -1, 1,   //2
			-1, -1, 1,  //3

			-1, 1, -1,  //4
			1, 1, -1,   //5
			1, 1, 1,    //6
			-1, 1, 1,   //7
		};
		unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3,

			3, 0, 7,
			7, 4, 0,

			0, 1, 5,
			5, 4, 1,

			1, 2, 5,
			5, 6, 2,

			2, 3, 6,
			6, 7, 3,


			4, 5, 6,
			4, 6, 7
		};
		*/
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

		glGenVertexArrays(1, &VBO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));

		
		glBindVertexArray(NULL);
	}
	inline void Draw(RectangleShader * shader, glm::mat4 matrix, glm::vec3 color) {
		glBindVertexArray(VAO);
		glUseProgram(shader->ID);

		shader->set_matrix(matrix);
		shader->set_color(color);

		glLineWidth(10);
		glDrawArrays(GL_LINE_LOOP, 0, 35);
		
		glBindVertexArray(NULL);
	}
};




RectangleShader* rect_sh = nullptr;
Rectangle* rect = nullptr;



void InitGame(double w, double h) {
	
	rect_sh = new RectangleShader;
	rect = new Rectangle;


}


float xe = 0, ye = 0, ze = 0;
float xc = 0, yc = 0, zc = 0;
void Game(double w, double h, float x, float y) {


	ImGui::Begin("Console");
	if(ImGui::CollapsingHeader("Eye")) {
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

	glm::mat4 pers = glm::perspective<double>(glm::radians(85.0f), h / w, 0.001, w);

	glm::mat4 matrix = pers * view * model;
	

	rect->Draw(rect_sh, matrix, glm::vec3(1, 0, 0));







}





