#include <glm/gtc/type_ptr.hpp> // value_ptr
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective, ...

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include "Object3D.h"


GLfloat ZOOM = 10.0f;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//zoom in
	if (yoffset == 1) {
		
		ZOOM += fabs(ZOOM) * 0.1f;
	}
	//zoom out
	else if (yoffset == -1) {
		
		ZOOM -= fabs(ZOOM) * 0.1f;
	}
	std::cout << "ZOOM = " << ZOOM << std::endl;
}
void print_error(int error, const char* description);


std::vector<glm::vec3> loadTestModel() {
	// 6 faces * 4 vértices por face
	glm::vec3 point[6 * 4] = {
		// Frente
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		// Trás
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		// Direita
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		// Esquerda
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		// Cima
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		// Baixo
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f)
	};

	std::vector<glm::vec3> ret;
	for (auto i : point)
		ret.push_back(i);

	return ret;
}
void display(std::vector<glm::vec3> obj, glm::mat4 mvp) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float* vertex_stream = static_cast<float*>(glm::value_ptr(obj.front()));

	std::vector<glm::vec3> colors{
		glm::vec3(1.0f, 0.0f, 0.0f), // Red
		glm::vec3(1.0f, 1.0f, 0.0f), // Yellow
		glm::vec3(0.0f, 1.0f, 0.0f), // Green
		glm::vec3(0.0f, 1.0f, 1.0f), // Cyan
		glm::vec3(0.0f, 0.0f, 1.0f), // Blue
		glm::vec3(1.0f, 0.0f, 1.0f)  // Magenta
	};

	// Desenha quad em modo imediato
	glBegin(GL_QUADS);
	/* obj.size() * (obj.front().length()) é o mesmo que (6*4)*3 */
	/* 6 faces * 4 vértices por face * 3 coordenadas por vértice */
	for (int nv = 0; nv < 6 * 4 * 3; nv += 3) {
		// Uma cor por face
		glColor3f(colors[nv / (4 * 3)].r, colors[nv / (4 * 3)].g, colors[nv / (4 * 3)].b);
		glm::vec4 vertex = glm::vec4(vertex_stream[nv], vertex_stream[nv + 1], vertex_stream[nv + 2], 1.0f);
		// Cálculo das coordenadas de recorte
		glm::vec4 transformed_vertex = mvp * vertex;
		// Divisão de Perspetiva
		glm::vec4 normalized_vertex = transformed_vertex / transformed_vertex.w;
		// Desenho do vértice
		glVertex3f(normalized_vertex.x, normalized_vertex.y, normalized_vertex.z);
	}
	glEnd();
}
void testCube(glm::mat4 projection, std::vector<glm::vec3> object,float & angle) 
{


	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, ZOOM),	// Posição da câmara no mundo
		glm::vec3(0.0f, 0.0f, -1.0f),	// Direção para a qual a câmara esta apontada
		glm::vec3(0.0f, 1.0f, 0.0f)		// Vector vertical
	);
	// Model
	glm::mat4 model = glm::mat4(1.0f);
	// Vai efetuando uma rotação ao objeto (apenas para podermos ver todas as faces desenhadas).
	model = glm::rotate(model, angle += 0.00001, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	// MVP
	glm::mat4 mvp = projection * view * model;

	display(object, mvp);
}


int main() {
	
	
	GLFWwindow* window;


	glfwSetErrorCallback(print_error);  
	
	
	
	if (!glfwInit()) return -1;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);


	window = glfwCreateWindow(800, 600, "P3D - Trabalho Pratico 1 (Part #1)", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetScrollCallback(window, scrollCallback);


	Object3D ball;

	ball.loadOBJ("Ball1.obj");

	ball.printInfo();

	std::vector<glm::vec3> object = loadTestModel();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(videoMode->width) / float(videoMode->height), 0.1f, 100.f);

	float angle = 0;
	while (!glfwWindowShouldClose(window)) {

		testCube(projection, object, angle);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}





void print_error(int error, const char* description) {
	std::cout << description << std::endl;
}
