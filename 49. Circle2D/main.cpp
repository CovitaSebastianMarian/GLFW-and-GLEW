#include "Engine/Library.h"
#include "game.hpp"
#include <thread>

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




auto main(int argc, char** argv) -> int {
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




	
	Form2D* cir1 = new Form2D(6);
	cir1->Translate(glm::vec2(100, 100));
	Form2D* cir2 = new Form2D(6);
	cir2->Translate(glm::vec2(200, 100));
	
	Box* box = new Box();
	box->Push(cir1);
	box->Push(cir2);

	ShapeShader2D* sh = new ShapeShader2D();


	box->Texture("Images/concreate.jpg", GL_RGB);
	box->Ortho(0, WindowWidth, WindowHeight, 0);
	box->Scale({ 50, 50 });
	
	

	


	while (!glfwWindowShouldClose(window)) {
		glClearColor(0, 0, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//box->Move({ 1, 0 });
		box->Angle(1, true, { 300, 300 });
		box->Draw(sh);


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
