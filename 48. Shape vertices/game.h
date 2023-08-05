#pragma once


class PointShader {
public:
	unsigned int ID;
	PointShader() {
		const char* vert_shader =
			"#version 330 core\n"
			"layout (location = 0) in vec3 pos;\n"
			"uniform mat4 model;\n"
			"void main() { gl_Position = model * vec4(pos, 1.0); }\n";

		const char* frag_shader =
			"#version 330 core\n"
			"uniform vec2 pos;\n"
			"uniform float scale;\n"
			"uniform vec3 color;\n"
			"void main() {\n"
			"	vec2 coord = gl_FragCoord.xy;\n"
			"	vec4 col;\n"
			"	if(distance(coord, pos) <= scale) col = vec4(color, 1.);\n"
			"	gl_FragColor = col;}\n";

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
	void setModel(const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, &mat[0][0]);
	}
	void setScale(const float value) const
	{
		glUniform1f(glGetUniformLocation(ID, "scale"), value);
	}
	void setPositions(const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, "pos"), 1, &value[0]);
	}
	void setColor(const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, "color"), 1, &value[0]);
	}
};



class Point {
private:
	//point coord
	float pos[12] = {
		-1, -1, 0,
		1, -1, 0,
		1, 1, 0,
		-1, 1, 0,
	};
	unsigned int VAO, VBO;
public:
	glm::vec2 position = glm::vec2(0.0);
	glm::vec3 color = glm::vec3(0.0);
	float scale = 0;
	Point() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)(0 * sizeof(float)));


		glBindVertexArray(0);
	}
	void Draw(PointShader *shader, double w, double h) {
		glBindVertexArray(VAO);
		glUseProgram(shader->ID);

		glm::mat4 model(1.0);
		model = glm::ortho<double>(0, w, h, 0, 0, 0.1);
		model = glm::translate(model, glm::vec3(position, 0));
		model = glm::scale(model, glm::vec3(scale));

		shader->setModel(model);

		shader->setScale(scale);
		//h - positions.y is used because in glsl shaders use invert y axis
		shader->setPositions(glm::vec2(position.x, h - position.y));
		shader->setColor(color);

		glDrawArrays(GL_QUADS, 0, 4);
		glUseProgram(0);
		glBindVertexArray(0);
	}
};

class Shape {
private:
	const double pi = 3.14159265359;
	struct vec {
		Point* point = nullptr;
		double angle = 0;
		double dist = 0;
	};

	vec* vector_points = nullptr;
	//center point(the point which is used to rotate and translate the shape) 
	Point* center_point = new Point;

	uint64_t size = 0ULL;
	//returns distance between two points
	_NODISCARD inline double Distance(const Point& a, const Point& b) const {
		double x = a.position.x - b.position.x;
		double y = a.position.y - b.position.y;
		return sqrt(x * x + y * y);
	}
public:
	Shape() noexcept = default;

	inline void Init(double center_point_x_pos, double center_point_y_pos, vector<pair<float, float>> pos) {
		this->size = pos.size();
		vector_points = new vec[size];

		center_point->scale = 10;
		center_point->color = glm::vec3(1, 0, 0);
		center_point->position = glm::vec2(center_point_x_pos, center_point_y_pos);

		for (uint64_t i = 0; i < size; ++i) {

			vector_points[i].point = new Point();
			vector_points[i].point->scale = 10;
			vector_points[i].point->color = glm::vec3(1, 1, 1);
			vector_points[i].point->position = glm::vec2(pos[i].first, pos[i].second);


			float dist = Distance(*center_point, *vector_points[i].point);
			float angle = atan2(vector_points[i].point->position.x - center_point->position.x, vector_points[i].point->position.y - center_point->position.y);

			vector_points[i].angle = angle;
			vector_points[i].dist = dist;
		}
	}

	//translates every point with x and y
	inline void Translate(double x, double y) {
		center_point->position = glm::vec2(x, y);

		for (uint64_t i = 0; i < size; ++i) {

			double x = sin(vector_points[i].angle) * vector_points[i].dist + center_point->position.x;
			double y = cos(vector_points[i].angle) * vector_points[i].dist + center_point->position.y;

			vector_points[i].point->position = glm::vec2(x, y);
		}
	}
	//scales every point with scale coefficient
	inline void Scale(float scale_coefficient) {
		for (uint64_t i = 0; i < size; ++i) {
			vector_points[i].dist *= scale_coefficient;
			double x = sin(vector_points[i].angle) * vector_points[i].dist + center_point->position.x;
			double y = cos(vector_points[i].angle) * vector_points[i].dist + center_point->position.y;

			vector_points[i].point->position = glm::vec2(x, y);
		}
	}
	inline void Rotate(float angle, bool update = false) {
		for (uint64_t i = 0; i < size; ++i) {
			float ang = (vector_points[i].angle + angle);
			if (ang >= 2 * pi) ang -= 2 * pi;

			double x = sin(ang) * vector_points[i].dist + center_point->position.x;
			double y = cos(ang) * vector_points[i].dist + center_point->position.y;

			vector_points[i].point->position = glm::vec2(x, y);
			if (update) vector_points[i].angle = ang;
		}
	}

	inline void Draw(PointShader * shader, double w, double h) {
		for (uint64_t i = 0; i < size; ++i) {
			vector_points[i].point->Draw(shader, w, h);
		}
		center_point->Draw(shader, w, h);
	}
};




PointShader* point_shader = nullptr;
Shape* rect = nullptr;

void InitGame(double w, double h) {

	point_shader = new PointShader;
	rect = new Shape;
	vector<pair<float, float>> pp;
	pp.push_back({ w / 2 - 200, h / 2 - 100 });
	pp.push_back({ w / 2 + 200, h / 2 - 100 });
	pp.push_back({ w / 2 + 200, h / 2 + 100 });
	pp.push_back({ w / 2 - 200, h / 2 + 100 });
	rect->Init(w / 2, h / 2, pp);

}

void Game(double w, double h) {
	auto time = glfwGetTime();
	rect->Rotate(time);
	rect->Draw(point_shader, w, h);

}
