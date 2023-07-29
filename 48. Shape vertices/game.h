#pragma once
//include the class after include glew, glfw, glm, vector and using namespace std
//use Shader class
Shader* shader = nullptr;

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
	void Draw(double w, double h) {
		glBindVertexArray(VAO);
		glUseProgram(shader->ID);

		glm::mat4 model(1.0);
		model = glm::ortho<double>(0, w, h, 0, 0, 0.1);
		model = glm::translate(model, glm::vec3(position, 0));
		model = glm::scale(model, glm::vec3(scale));

		shader->setMat4("model", model);

		shader->setFloat("scale", scale);
		//h - positions.y is used because in glsl shaders use invert y axis
		shader->setVec2("pos", glm::vec2(position.x, h - position.y));
		shader->setVec3("color", color);
		
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
	Shape() {}
	
	//After setting possition
	inline void Init(vector<pair<float, float>> pos) {
		this->size = pos.size();
		vector_points = new vec[size];

		center_point->scale = 10;
		center_point->color = glm::vec3(1, 0, 0);

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
	inline void SetPosition(double x, double y) {
		center_point->position = glm::vec2(x, y);
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
	inline void SetAngle(float angle, bool update = false) {
		for (uint64_t i = 0; i < size; ++i) {
			float ang = (vector_points[i].angle + angle);
			if (ang >= 2 * pi) ang -= 2 * pi;

			double x = sin(ang) * vector_points[i].dist + center_point->position.x;
			double y = cos(ang) * vector_points[i].dist + center_point->position.y;

			vector_points[i].point->position = glm::vec2(x, y);
			if(update) vector_points[i].angle = ang;
		}
	}

	inline void Draw(double w, double h) {
		for (uint64_t i = 0; i < size; ++i) {
			vector_points[i].point->Draw(w, h);
		}
		center_point->Draw(w, h);
	}
};

Shape* rect = nullptr;
Point* p = nullptr;

//w = window width, h = window height
void InitGame(double w, double h) {

	shader = new Shader("Shaders/shader.vert", "Shaders/shader.frag");
	
	rect = new Shape();
	rect->SetPosition(w / 2, h / 2);

	vector<pair<float, float>> pp;
	pp.push_back({w / 2 - 200, h / 2 - 100});
	pp.push_back({w / 2 + 200, h / 2 - 100});
	pp.push_back({w / 2 + 200, h / 2 + 100});
	pp.push_back({w / 2 - 200, h / 2 + 100});
	rect->Init(pp);
}

//w = window width, h = window height
void Game(double w, double h) {

	auto time = glfwGetTime();
	rect->SetAngle(time);
	rect->Draw(w, h);


}

