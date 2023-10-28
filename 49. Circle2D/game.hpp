#pragma once




/*




caracteristici:
-pozitie
-unghi
-scala
-punct de rotatie
-textura -- functie pentru incarcare textura


###################


Circle2D
Polygon2D


Rectangle2D - Polygon2D din 4 puncte

Box -> va contine mai multe poligoane si se va comporta ca o singura forma

*/


#define NODISCARD [[nodiscard]]
#define mut mutable

inline void LoadTexture(uint32_t *texture, const char * file, const int32_t internalformat) {
	int32_t width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	glGenTextures(1, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, internalformat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, NULL);
}


class ShapeShader2D {
public:
	ShapeShader2D() {
		const char* vert_shader =
			"#version 330 core\n"
			"layout (location = 0) in vec2 pos;\n"
			"layout (location = 1) in vec2 texcoord;\n"
			"uniform mat4 matrix;\n"
			"out vec2 tcoord;\n"
			"void main()\n"
			"{ gl_Position = matrix * vec4(pos, 0.0, 1.0);\n"
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

		id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	uint32_t id;
	inline void set_matrix(const glm::mat4 mat) const {
		glUniformMatrix4fv(glGetUniformLocation(id, "matrix"), 1, GL_FALSE, &mat[0][0]);
	}
};



class Shape2D {
public:
	virtual inline void Draw(ShapeShader2D* shader) const {
		matrix = glm::ortho<double>(ortho.x, ortho.y, ortho.z, ortho.w, 0, 0.1);
		matrix = glm::translate(matrix, glm::vec3(this->position, 0));
		matrix = glm::rotate(matrix, glm::radians(this->angle), glm::vec3(0, 0, 1));
		matrix = glm::scale(matrix, glm::vec3(this->scale, 0));


		glUseProgram(shader->id);

		shader->set_matrix(matrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);


		glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, NULL);

		glBindVertexArray(NULL);
		glBindTexture(GL_TEXTURE_2D, NULL);
		glUseProgram(NULL);
	}
	virtual inline void Ortho(const double left, const double right, const double bottom, const double top) const {
		this->ortho = glm::vec4(left, right, bottom, top);
	}
	virtual inline void Translate(const glm::vec2& position) const {
		this->position = position;
	}
	virtual inline void Move(const glm::vec2& position) const {
		this->position += position;
	}
	virtual inline void Angle(const float& angle, bool use_another_point = false , glm::vec2 point = glm::vec2(0)) const {
		this->angle = angle;

		if (use_another_point) {
			float xx = position.x - point.x;
			float yy = position.y - point.y;

			float ang = this->angle * pi / 180.f;

			float x = cos(ang) * xx - sin(ang) * yy + point.x;
			float y = sin(ang) * xx + cos(ang) * yy + point.y;
			position = glm::vec2(x, y);
		}
	}
	virtual inline void Rotate(const float& angle, bool use_another_point = false, glm::vec2 point = glm::vec2(0)) const {
		this->angle += angle;

		if (use_another_point) {
			float xx = position.x - point.x;
			float yy = position.y - point.y;
			
			float ang = this->angle * pi / 180.f;

			float x = cos(ang) * xx - sin(ang) * yy + point.x;
			float y = sin(ang) * xx + cos(ang) * yy + point.y;
			position = glm::vec2(x, y);
		}
	}
	virtual inline void Scale(const glm::vec2 scale) const {
		this->scale = scale;
	}
	virtual inline void Texture(const char* file, const int32_t internalformat) const {
		LoadTexture(&texture, file, internalformat);
	}
protected:
	const float pi = 3.14159265359f;

	uint32_t VAO, VBO, EBO;
	float* vertices = nullptr;
	uint32_t* indices = nullptr;
	uint32_t indices_size = 0U;
	uint64_t vertices_size = 0ULL;

	mut glm::vec4 ortho = glm::vec4(0);
	mut glm::vec2 position = glm::vec2(0);
	mut float angle = 0.f;
	mut glm::vec2 scale = glm::vec2(0);
	mut uint32_t texture = 0U;
	mut glm::mat4 matrix = glm::mat4(1);
};


class Box : public Shape2D {
public:
	Box() noexcept = default;
	~Box() {
		vbox.clear();
	}
	inline void Draw(ShapeShader2D* shader) const override {
		for (Shape2D* sh : vbox) {
			sh->Draw(shader);
		}
	}
	inline void Ortho(const double left, const double right, const double bottom, const double top) const override {
		for (Shape2D* sh : vbox) {
			sh->Ortho(left, right, bottom, top);
		}
	}
	inline void Translate(const glm::vec2& pos) const override {
		for (Shape2D* sh : vbox) {
			sh->Translate(pos);
		}
	}
	inline void Move(const glm::vec2& pos) const override {
		for (Shape2D* sh : vbox) {
			sh->Move(pos);
		}
	}
	inline void Angle(const float& angle, bool use_another_point = false,  glm::vec2 point = glm::vec2(0)) const override {
		for (Shape2D* sh : vbox) {
			sh->Angle(angle, use_another_point, point);
		}
	}
	inline void Rotate(const float& angle, bool use_another_point = false,  glm::vec2 point = glm::vec2(0)) const override {
		for (Shape2D* sh : vbox) {
			sh->Rotate(angle, use_another_point, point);
		}
	}
	inline void Scale(const glm::vec2 scale) const override {
		for (Shape2D* sh : vbox) {
			sh->Scale(scale);
		}
	}
	inline void Texture(const char* file, const int32_t internalformat) const override {
		for (Shape2D* sh : vbox) {
			sh->Texture(file, internalformat);
		}
	}
	inline void Push(Shape2D* shape) const {
		vbox.push_back(shape);
	}
private:
	mutable vector<Shape2D*> vbox;
};









class Circle2D : public Shape2D {
public:
	Circle2D(uint16_t points) {
		float jump = 360.0f / float(points);
		vertices = new float[4 * (points + 1)];
		indices = new uint32_t[3 * (points)];
		vertices[0] = 0.0f;
		vertices[1] = 0.0f;
		vertices[2] = 0.5f;
		vertices[3] = 0.5f;
		uint32_t vertices_index = 3U;
		int32_t indices_index = -1;
		uint32_t index = 1U;


		for (float i = 0; i < 360.f; i += jump, ++index) {
			float delta = (i * pi) / 180.0f;
			float x = sin(delta);
			float y = cos(delta);
			vertices[++vertices_index] = x;
			vertices[++vertices_index] = y;
			vertices[++vertices_index] = (x * 0.5f) + 0.5f;
			vertices[++vertices_index] = (y * 0.5f) + 0.5f;

			if (index > 1) {
				indices[++indices_index] = 0;
				indices[++indices_index] = index - 1;
				indices[++indices_index] = index;
			}
		}

		indices[++indices_index] = 0;
		indices[++indices_index] = index - 1;
		indices[++indices_index] = 1;


		indices_size = indices_index + 1;
		vertices_size = vertices_index + 1;
	
		

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(float), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(uint32_t), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glBindVertexArray(NULL);
	}
};


