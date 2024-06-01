#include "Camera.h"


void Mouse::CamMove(GLFWwindow* window) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwGetCursorPos(window, &this->newInput[0], &this->newInput[1]);
		this->isPressed = true;
		currentMoveY = this->newInput[0] - this->oldInput[0];
		currentMoveX = this->newInput[1] - this->oldInput[1];
		/*if (currentMove < 0) angle -= sense;
		else if (currentMove > 0) angle += sense;
		else {
			angle += 0.0f;
		}*/
		angleY += glm::radians(currentMoveY);
		angleX += glm::radians(currentMoveX);
	}
	else {
		this->isPressed = false;
		angleX += 0.0f;
		angleY += 0.0f;
	}
	glfwGetCursorPos(window, &this->oldInput[0], &this->oldInput[1]);
}