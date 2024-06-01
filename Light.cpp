#include "Light.h"


Light::Light()
{

}

void Light::UseLight(Shader* shader)
{

}

PointLight::PointLight(glm::vec3 _ambient, glm::vec3 _diffuse,
	glm::vec3 _specular, glm::vec3 _position,
	float _constant, float _linear, float _quadratic)
{
	_ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;
	_constant = constant;
	_position = position;
	_linear = linear;
	_quadratic = quadratic;
}

void PointLight::UseLight(Shader *shader)
{
	shader->SetUniformVec3("pointLight.position", position);
	shader->SetUniformVec3("pointLight.ambient", ambient);
	shader->SetUniformVec3("pointLight.diffuse", diffuse);
	shader->SetUniformVec3("pointLight.specular", specular);
	shader->SetUniform1f("pointLight.constant", constant);
	shader->SetUniform1f("pointLight.linear", linear);
	shader->SetUniform1f("pointLight.quadratic", quadratic);
}

SpotLight::SpotLight(glm::vec3 _ambient, glm::vec3 _diffuse,
	glm::vec3 _specular, glm::vec3 _position, glm::vec3 spotDirection,
	float _constant, float _linear, float _quadratic, float _spotCutOff,
	float _spotExponent)
{
	ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;
	_constant = constant;
	_position = position;
	_linear = linear;
	_quadratic = quadratic;
	_spotCutOff = spotCutoff;
	_spotExponent = spotExponent;
	spotDirection = spotDirection;
}

void SpotLight::UseLight(Shader* shader)
{
	shader->SetUniformVec3("spotLight.position",position);
	shader->SetUniformVec3("spotLight.ambient", ambient);
	shader->SetUniformVec3("spotLight.diffuse", diffuse);
	shader->SetUniformVec3("spotLight.specular", specular);
	shader->SetUniform1f("spotLight.constant", constant);
	shader->SetUniform1f("spotLight.linear", linear);
	shader->SetUniform1f("spotLight.quadratic", quadratic);
	shader->SetUniform1f("spotLight.spotCutoff", cos(glm::radians(spotCutoff)));
	shader->SetUniform1f("spotLight.spotExponent", cos(glm::radians(spotExponent)));
	shader->SetUniformVec3("spotLight.spotDirection", spotDirection);
}

DirectionalLight::DirectionalLight(glm::vec3 _ambient, glm::vec3 _diffuse,
	glm::vec3 _specular, glm::vec3 _direction) {
	
	_ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;
	_direction = direction;
}


void DirectionalLight::UseLight(Shader *shader)
{
	shader->SetUniformVec3("directionalLight.direction", direction);
	shader->SetUniformVec3("directionalLight.ambient", ambient);
	shader->SetUniformVec3("directionalLight.diffuse", diffuse);
	shader->SetUniformVec3("directionalLight.specular",specular);
}

AmbientLight::AmbientLight(glm::vec3 _ambient)
{
	_ambient = ambient;
}

void AmbientLight::UseLight(Shader *shader)
{
	shader->SetUniformVec3("ambientLight.ambient", ambient);
}

