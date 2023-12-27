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
				"uniform sampler2D set_texture;\n"
				"void main() { gl_FragColor = texture(set_texture, tcoord); }\n";


			GLuint vertex, fragment;

			id = glCreateProgram();

			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vert_shader, NULL);
			glCompileShader(vertex);
			glAttachShader(id, vertex);
			glDeleteShader(vertex);

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &frag_shader, NULL);
			glCompileShader(fragment);
			glAttachShader(id, fragment);
			glDeleteShader(fragment);

			glLinkProgram(id);
		}
		uint32_t id;
		inline void set_matrix(const glm::mat4 mat) const {
			glUniformMatrix4fv(glGetUniformLocation(id, "matrix"), 1, GL_FALSE, &mat[0][0]);
		}
	};


	/*
	*  Shape2D -> Pipeline2D -> Form2D
	*          				 -> ...
	*/

	class Shape2D {
	public:
		inline glm::vec2 get_position() const {
			return position;
		}
		inline glm::mat4 get_matrix() const {
			return matrix;
		}
		inline glm::vec2 get_scale() const {
			return scale;
		}
		inline float get_angle() const {
			return angle;
		}
		inline float* get_vertices() const {
			return vertices.get();
		}
		inline uint64_t get_vertices_size() const {
			return vertices_size;
		}
		inline float* get_texture_coords() const {
			return texture_coords.get();
		}
		inline uint64_t get_texture_coords_size() const {
			return texture_coords_size;
		}
		inline uint32_t* get_indices() const {
			return indices.get();
		}
		inline uint32_t get_indices_size() const {
			return indices_size;
		}
		inline void load_matrix() const {
			matrix = glm::ortho<double>(orthographic.x, orthographic.y, orthographic.z, orthographic.w, 0, 0.1);
			matrix = glm::translate(matrix, glm::vec3(this->position, 0));
			//while (this->angle >= 360.f) this->angle -= 360.f;
			matrix = glm::rotate(matrix, glm::radians(this->angle), glm::vec3(0, 0, 1));
			matrix = glm::scale(matrix, glm::vec3(this->scale, 0));
		}
	protected:
		const float pi = 3.14159265359F;

		uint32_t VAO, VBO_vertices, VBO_texture_coords, EBO;
		unique_ptr<float[]> vertices;
		unique_ptr<float[]> texture_coords;
		unique_ptr<uint32_t[]> indices;

		uint32_t indices_size = 0U;
		uint64_t vertices_size = 0ULL;
		uint64_t texture_coords_size = 0ULL;

		mutable glm::vec4 orthographic = glm::vec4(0);
		mutable glm::vec2 position = glm::vec2(0);
		mutable float angle = 0.f;
		mutable glm::vec2 scale = glm::vec2(0);
		mutable glm::mat4 matrix = glm::mat4(1);
	};



	class Pipeline2D : public Shape2D {
	public:
		inline void draw(const ShapeShader2D& shader, const ShapeTexture2D& texture) const {

			load_matrix();

			glUseProgram(shader.id);
			shader.set_matrix(matrix);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture.get_texture());
			glBindVertexArray(VAO);

			glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, NULL);

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}
		inline void set_orthographic(const double left, const double right, const double bottom, const double top) const {
			this->orthographic = glm::vec4(left, right, bottom, top);
		}
		inline void set_position(const glm::vec2& position) const {
			this->position = position;
		}
		inline void move(const glm::vec2& position) const {
			this->position += position;
		}
		inline void set_angle(const float& angle, bool use_another_point = false, glm::vec2 point = glm::vec2(0)) const {
			this->angle = angle;

			if (use_another_point) {
				float xx = position.x - point.x;
				float yy = position.y - point.y;

				float ang = angle * pi / 180.f;

				float x = cos(ang) * xx - sin(ang) * yy + point.x;
				float y = sin(ang) * xx + cos(ang) * yy + point.y;
				position = glm::vec2(x, y);
			}
		}
		inline void rotate(const float& angle, bool use_another_point = false, glm::vec2 point = glm::vec2(0)) const {
			this->angle += angle;

			if (use_another_point) {
				float xx = position.x - point.x;
				float yy = position.y - point.y;

				float ang = angle * pi / 180.f;

				float x = cos(ang) * xx - sin(ang) * yy + point.x;
				float y = sin(ang) * xx + cos(ang) * yy + point.y;
				position = glm::vec2(x, y);
			}
		}
		inline void set_scale(const glm::vec2 scale) const {
			this->scale = scale;
		}
	};

	/*
	class Box {
	public:
		Box() noexcept = default;
		~Box() {
			vbox.clear();
		}
		inline void draw(const ShapeShader2D& shader, const Texture& texture) const {
			for (Pipeline2D* sh : vbox) {
				sh->draw(shader, texture);
			}
		}
		inline void set_orthographic(const double left, const double right, const double bottom, const double top) const {
			for (Pipeline2D* sh : vbox) {
				sh->set_orthographic(left, right, bottom, top);
			}
		}
		inline void set_position(const glm::vec2& pos) const {
			for (Pipeline2D* sh : vbox) {
				sh->set_position(pos);
			}
		}
		inline void move(const glm::vec2& pos) const {
			for (Pipeline2D* sh : vbox) {
				sh->move(pos);
			}
		}
		inline void set_angle(const float& angle, bool use_another_point = false, glm::vec2 point = glm::vec2(0)) const {
			for (Pipeline2D* sh : vbox) {
				sh->set_angle(angle, use_another_point, point);
			}
		}
		inline void rotate(const float& angle, bool use_another_point = false, glm::vec2 point = glm::vec2(0)) const {
			for (Pipeline2D* sh : vbox) {
				sh->rotate(angle, use_another_point, point);
			}
		}
		inline void set_scale(const glm::vec2 scale) const {
			for (Pipeline2D* sh : vbox) {
				sh->set_scale(scale);
			}
		}
		inline void push(Pipeline2D* form) const {
			vbox.push_back(form);
		}
		inline Pipeline2D* operator[](uint64_t index) const {
			return vbox[index];
		}
		inline vector<Pipeline2D*> get_pipeline2D_container() const {
			return vbox;
		}
	private:
		mutable vector<Pipeline2D*> vbox;
	};
	*/


	class Form2D : public Pipeline2D {
	public:
		Form2D(bool use_dsa, uint16_t points_number, float start_from = 0.f) {
			float jump = 360.0f / float(points_number);

			vertices = make_unique<float[]>(static_cast<uint64_t>(2 * (points_number + 1)));
			texture_coords = make_unique<float[]>(static_cast<uint64_t>(2 * (points_number + 1)));
			indices = make_unique<uint32_t[]>(static_cast<uint64_t>(3 * points_number));


			vertices[0] = 0.0f;
			vertices[1] = 0.0f;
			texture_coords[0] = 0.5f;
			texture_coords[1] = 0.5f;
			uint32_t vertices_index = 1U;
			uint32_t texture_coords_index = 1U;
			int32_t indices_index = -1;
			uint32_t index = 1U;
			

			for (float i = start_from; i < 360.f + start_from; i += jump, ++index) {
				float delta = (i * pi) / 180.f;
				float x = sin(delta);
				float y = cos(delta);
				vertices[++vertices_index] = x;
				vertices[++vertices_index] = y;
				texture_coords[++texture_coords_index] = (x + 1.f) * 0.5f;
				texture_coords[++texture_coords_index] = (y + 1.f) * 0.5f;

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
			texture_coords_size = texture_coords_index + 1;

			if (use_dsa) {
				glCreateVertexArrays(1, &VAO);
				glCreateBuffers(1, &VBO_vertices);
				glCreateBuffers(1, &VBO_texture_coords);
				glCreateBuffers(1, &EBO);
			

				glNamedBufferData(VBO_vertices, vertices_size * sizeof(float), vertices.get(), GL_DYNAMIC_DRAW);
				glNamedBufferData(VBO_texture_coords, texture_coords_size * sizeof(float), texture_coords.get(), GL_DYNAMIC_DRAW);
				glNamedBufferData(EBO, indices_size * sizeof(uint32_t), indices.get(), GL_DYNAMIC_DRAW);


				glEnableVertexArrayAttrib(VAO, 0);
				glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
				glVertexArrayAttribBinding(VAO, 0, 0);


				glEnableVertexArrayAttrib(VAO, 1);
				glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 0);
				glVertexArrayAttribBinding(VAO, 1, 1);


				glVertexArrayVertexBuffer(VAO, 0, VBO_vertices, 0, 2 * sizeof(float));
				glVertexArrayVertexBuffer(VAO, 1, VBO_texture_coords, 0, 2 * sizeof(float));
				glVertexArrayElementBuffer(VAO, EBO);
			}
			else {
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO_vertices);
				glGenBuffers(1, &VBO_texture_coords);
				glGenBuffers(1, &EBO);


				glBindVertexArray(VAO);


				glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
				glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(float), vertices.get(), GL_DYNAMIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
				//glBindBuffer(GL_ARRAY_BUFFER, 0);


				glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coords);
				glBufferData(GL_ARRAY_BUFFER, texture_coords_size * sizeof(float), texture_coords.get(), GL_DYNAMIC_DRAW);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
				//glBindBuffer(GL_ARRAY_BUFFER, 0);


				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(uint32_t), indices.get(), GL_DYNAMIC_DRAW);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				

				glBindVertexArray(0);
			}
		}
		~Form2D() {
			vertices.release();
			indices.release();
		}
	};