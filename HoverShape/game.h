#pragma once



Shader* shader = nullptr;

unsigned int VAO;

float pos[] = {
	-0.5, -0.5, 0,
	0.5, -0.5, 0,
	0, 0.5, 0,
};


void InitGame(double w, double h) {

	shader = new Shader("Shaders/shader.vert", "Shaders/shader.frag");
	

	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_DYNAMIC_DRAW);

	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));

	glBindVertexArray(NULL);

}


float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}


bool HoverTriangle(vector<glm::vec2> coord, glm::vec2 pos) {
	/*
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pos, coord[0], coord[1]);
	d2 = sign(pos, coord[1], coord[2]);
	d3 = sign(pos, coord[2], coord[0]);

	has_neg = (d1 < 0) && (d2 < 0) && (d3 < 0);
	has_pos = (d1 > 0) && (d2 > 0) && (d3 > 0);

	return (has_neg || has_pos);
	*/
	//asta e pt orice polygon convex
	float d1, d2;
	float size = coord.size();
	d1 = sign(pos, coord[0], coord[1]);
	for (int i = 1; i < size - 1; ++i) {
		d2 = sign(pos, coord[i], coord[i + 1]);
		if (min(d1, d2) < 0 && max(d1, d2) > 0) return false;
	}
	d2 = sign(pos, coord[size - 1], coord[0]);
	if (min(d1, d2) < 0 && max(d1, d2) > 0) return false;

	return true;
}




void Game(double w, double h, float x, float y) {


	glBindVertexArray(VAO);
	glUseProgram(shader->ID);

	//glm::mat4 model = glm::ortho<double>(-1, 1, -1, 1, 0, 0.1);
	//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 0, 1));
	glm::mat4 model = glm::ortho<double>(0, w, h, 0, 0, 0.1);
	model = glm::translate(model, glm::vec3(213, 234, 0));
	model = glm::scale(model, glm::vec3(200));


	shader->setMat4("model", model);
	

	glm::vec2 mousepos = glm::vec2(0.0);
	mousepos.x = x / (w / 2) - 1;
	mousepos.y = (h - y) / (h / 2) - 1;
	

	vector<glm::vec2> vv2;
	for (uint64_t i = 0; i < sizeof(pos) / sizeof(float); i += 3) {
		glm::vec4 v = glm::vec4(pos[i], pos[i + 1], pos[i + 2], 1);
		v = model * v;
		vv2.push_back(glm::vec2(v.x, v.y));
	}

	
	glm::vec3 color = glm::vec3(1.0);

	if (HoverTriangle(vv2, glm::vec2(mousepos.x, mousepos.y))) color = glm::vec3(0, 0, 1);
	else color = glm::vec3(1, 0, 0);
	
	shader->setVec3("color", color);
	
	
	glDrawArrays(GL_POLYGON, 0, sizeof(pos) / sizeof(float) / 3);

	glUseProgram(NULL);
	glBindVertexArray(NULL);
	





}





