#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Engine/Shader.h"
#include "Engine/Model.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glm/glm.hpp"

double WindowHeight = 700;
double WindowWidth = 1200;
double WindowPosX, WindowPosY;

void key(GLFWwindow* window, int key, int scanmode, int state, int mods) {
	if (key == GLFW_KEY_ESCAPE && state == GLFW_RELEASE) glfwDestroyWindow(window), exit(0);
}
bool ResizeWindow(GLFWwindow * window) {
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	if (w != WindowWidth || h != WindowHeight) {
		WindowHeight = h;
		WindowWidth = w;
		glViewport(0, 0, WindowWidth, WindowHeight);
		return true;
	}
	return false;
}

auto main(int argc, char** argv) -> int {
	if (!glfwInit()) return -1;

	GLFWwindow* window;
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	//glfwWindowHint(GLFW_FLOATING, GL_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);
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
	glViewport(0, 0, WindowWidth, WindowHeight);


	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	unsigned int windowicons[3];
	for (int i = 0; i < 3; ++i) {
		int width, height, nrChannels;
		string filename;
		switch (i)
		{
		case 0:
			filename = "Images/underlineicon.png";
			break;
		case 1:
			filename = "Images/squareicon.png";
			break;
		case 2:
			filename = "Images/xicon.png";
			break;
		}
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

		glGenTextures(1, &windowicons[i]);
		glBindTexture(GL_TEXTURE_2D, windowicons[i]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}




	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
		colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
		colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		//colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		//colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(8.00f, 8.00f);
		style.FramePadding = ImVec2(5.00f, 5.00f);
		style.CellPadding = ImVec2(6.00f, 6.00f);
		style.ItemSpacing = ImVec2(6.00f, 6.00f);
		style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
		style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
		style.IndentSpacing = 25;
		style.ScrollbarSize = 15;
		style.GrabMinSize = 10;
		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = 1;
		style.TabBorderSize = 1;
		style.WindowRounding = 0;
		style.ChildRounding = 0;
		style.FrameRounding = 0;
		style.PopupRounding = 0;
		style.ScrollbarRounding = 9;
		style.GrabRounding = 0;
		style.LogSliderDeadzone = 4;
		style.TabRounding = 0;
	}







	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	double xnpos = 0, ynpos = 0;
	WindowPosX = (double(mode->width) - WindowWidth) / 2.0, WindowPosY = (double(mode->height) - WindowHeight) / 2.0;
	glfwSetWindowPos(window, WindowPosX, WindowPosY);
	bool movewindow = false;
	bool fullscreen = false;



	while (!glfwWindowShouldClose(window)) {
		glClearColor(0, 0, 0.2, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ResizeWindow(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/*
		ImGui::Begin("Launcher version 0.1 5/3/2023", &isopen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(WindowWidth, WindowHeight));



		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			//if (ImGui::IsItemHovered()) {
				glfwGetCursorPos(window, &xnpos, &ynpos);
				xnpos += WindowPosX;
				ynpos += WindowPosY;
				movewindow = true;
			//}
			//else movewindow = false;
		}
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && movewindow) {
			double xnpos2, ynpos2;
			glfwGetCursorPos(window, &xnpos2, &ynpos2);
			xnpos2 += WindowPosX;
			ynpos2 += WindowPosY;

			WindowPosX += (xnpos2 - xnpos);
			WindowPosY += (ynpos2 - ynpos);

			glfwSetWindowPos(window, WindowPosX, WindowPosY);
			xnpos = xnpos2;
			ynpos = ynpos2;
		}

		ImGui::End();
		*/

		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				glfwGetCursorPos(window, &xnpos, &ynpos);
				xnpos += WindowPosX;
				ynpos += WindowPosY;
				movewindow = true;
			}
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && movewindow && !fullscreen) {
				double xnpos2, ynpos2;
				glfwGetCursorPos(window, &xnpos2, &ynpos2);
				xnpos2 += WindowPosX;
				ynpos2 += WindowPosY;

				WindowPosX += (xnpos2 - xnpos);
				WindowPosY += (ynpos2 - ynpos);

				glfwSetWindowPos(window, WindowPosX, WindowPosY);
				xnpos = xnpos2;
				ynpos = ynpos2;
			}

			ImGui::BeginChild("##", ImVec2(WindowWidth - 21 * 3 - 40, 22), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::Text("Vector launcher version 0.2.1, latest update on 5/21/2023");
			ImGui::EndChild();
			


			for (int i = 0; i < 3; ++i) {
				if (ImGui::ImageButton(ImTextureID(windowicons[i]), ImVec2(21, 21), ImVec2(0, 0), ImVec2(1, 1), 1))
				{
					switch (i)
					{
					case 0:
						glfwIconifyWindow(window);
						break;
					case 1:
						if (!fullscreen) {
							glfwMaximizeWindow(window);
							ResizeWindow(window);
							fullscreen = true;
						}
						else {
							glfwRestoreWindow(window);
							ResizeWindow(window);
							fullscreen = false;
						}
						break;
					case 2:
						exit(0);
						break;
					}
				}
			}







			ImGui::EndMainMenuBar();
		}
		





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