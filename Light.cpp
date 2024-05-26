#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntesity = 1.0f;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

Light::Light(float red, float green, float blue, float aIntensity,
		float Xdir, float yDir,float zDir, float dIntensity)
{
	color = glm::vec3(red, green, blue);
	direction =glm::vec3(Xdir, yDir, zDir);
	ambientIntesity = aIntensity;
	diffuseIntesity = dIntensity;
}

Light::Light(glm::vec3 _color, float aIntensity, glm::vec3 _direction, float dIntensity)
{
	color = _color;
	direction = _direction;
	ambientIntesity = aIntensity;
	diffuseIntesity = dIntensity;
}

void Light::UseLight(Shader shader)
{
	shader.SetUniform3f("directionalLight.l_color",color.r, color.g, color.b);
	shader.SetUniform1f("directionalLight.ambientIntensity", ambientIntesity);
	shader.SetUniform3f("directionalLight.direction", direction.x, direction.y, direction.z);
	shader.SetUniform1f("directionalLight.diffuseIntensity", diffuseIntesity);
}

void Light::UpdateRotation(float angle)
{
	direction.y = angle;

}


