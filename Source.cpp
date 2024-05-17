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


int main() {

    if (!glfwInit()) return -1;

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "new window", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    GLint data;

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &data);

    printf("max att: %d\n", data);


	return 0;
}