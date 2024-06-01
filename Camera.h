#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
// Funções de matrizes de transformação:
// translate, rotate, scale, frustum, perspective, ortho, entre outras.
#include <glm/gtc/matrix_transform.hpp>
// Funções que manipulam a interação entre apontadores e tipos de vetores e matrizes:
// value_ptr
#include <glm/gtc/type_ptr.hpp>


class Mouse {
public:
	double newInput[2];
	double oldInput[2]{
		0.0f, 0.0f
	};
	double currentMoveX, currentMoveY;
	float angleX, angleY;
	float sense = glm::radians(1.0f);
	bool isPressed = false;
	void CamMove(GLFWwindow* window);
	void CamInit();
};

