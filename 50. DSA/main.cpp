#include "Engine/Library.h"
#include "game.h"

double WindowHeight = 700;
double WindowWidth = 1200;



void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
	if (key == GLFW_KEY_ESCAPE && state == GLFW_RELEASE) glfwDestroyWindow(window);
}
void ResizeWindow(GLFWwindow* window) {
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	WindowHeight = h;
	WindowWidth = w;

	glViewport(0, 0, WindowWidth, WindowHeight);
}

void mousebutton(GLFWwindow* window, int button, int action, int mods) {

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





	TexturedTriangle t;

	cout << glGetString(GL_VERSION);



	while (!glfwWindowShouldClose(window)) {
		glClearColor(0, 0, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		auto tme = glfwGetTime();
		t.animate(0, sin(tme), 0);
		t.draw();
		




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