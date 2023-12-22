#include "Engine/Library.h"
#include "2D.hpp"

double WindowHeight = 700;
double WindowWidth = 1200;



void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
	if (key == GLFW_KEY_ESCAPE && state == GLFW_RELEASE) glfwDestroyWindow(window);
}
void ResizeWindow(GLFWwindow * window) {
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	WindowHeight = h;
	WindowWidth = w;

	glViewport(0, 0, WindowWidth, WindowHeight);
}

void mousebutton(GLFWwindow * window, int button, int action, int mods) {

}




int main() {
	//srand(time(0));
	if (!glfwInit()) return -1;

	GLFWwindow* window;
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WindowWidth, WindowHeight, "", nullptr, nullptr);


	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetKeyCallback(window, key);
	glfwSetMouseButtonCallback(window, mousebutton);
	glfwSetWindowRefreshCallback(window, ResizeWindow);
	glViewport(0, 0, WindowWidth, WindowHeight);


	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	



	

	auto sh = make_unique<ShapeShader2D>();

	auto form = make_unique<Form2D>(36);
	form->set_orthographic(0, WindowWidth, WindowHeight, 0);
	form->set_texture("Images/concreate.jpg", GL_RGB);
	form->set_position({ WindowWidth / 2, WindowHeight / 2});
	form->set_scale({ 300, 300 });



	form->load_matrix();
	float x = form->get_vertices()[4];
	float y = form->get_vertices()[5];
	glm::vec4 pos = glm::vec4(x, y, 0, 1);
	pos = form->get_matrix() * pos;
	pos.x = (pos.x + 1) * WindowWidth / 2;
	pos.y = (pos.y + 1) * WindowHeight / 2;
	pos.y = WindowHeight - pos.y;



	
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0, 0, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		
		
		form->rotate(sin(glfwGetTime()), true, {pos.x, pos.y});
		form->draw(sh.get());

		

		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
