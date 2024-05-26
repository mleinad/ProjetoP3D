#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Luis/Shader.h"

class Light
{
public:
	Light();
	Light(float red, float green, float blue, float aIntensity,
		float Xdir, float yDir,float zDir, float dIntensity);
	Light(glm::vec3 _color, float aIntensity, glm::vec3 _direction, float dIntensity);

	void UseLight(Shader shader);

	void UpdateRotation(float angle);

private:
	glm::vec3 color;
	float ambientIntesity;

	glm::vec3 direction;
	float diffuseIntesity;

};

