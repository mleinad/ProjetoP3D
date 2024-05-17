#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")


GLfloat ZOOM = 10.0f;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	// Se faz zoom in
	if (yoffset == 1) {
		// Incremento no zoom, varia com a distância da câmara
		ZOOM += fabs(ZOOM) * 0.1f;
	}
	// Senão, se faz zoom out
	else if (yoffset == -1) {
		// Incremento no zoom, varia com a distância da câmara
		ZOOM -= fabs(ZOOM) * 0.1f;
	}
	std::cout << "ZOOM = " << ZOOM << std::endl;
}
void print_error(int error, const char* description);



int main() {
	
	
	GLFWwindow* window;


	glfwSetErrorCallback(print_error);
	
	
	if (!glfwInit()) return -1;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);


	window = glfwCreateWindow(videoMode->width, videoMode->height, "P3D - Trabalho Pratico 1 (Part #1)", monitor, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetScrollCallback(window, scrollCallback);


	while (!glfwWindowShouldClose(window)) {

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



void print_error(int error, const char* description) {
	std::cout << description << std::endl;
}
