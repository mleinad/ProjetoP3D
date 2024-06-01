#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Luis/Shader.h"

class Light
{
public:
	Light();

	virtual void UseLight(Shader *shader);

protected:

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};


class AmbientLight:public Light {

public:
	AmbientLight(glm::vec3 _ambient);

	void UseLight(Shader *shader);
private:
};


class DirectionalLight :public Light {
public:
	DirectionalLight(glm::vec3 _ambient, glm::vec3 _diffuse,
		glm::vec3 _specular, glm::vec3 _direction);

	void UseLight(Shader* shader);
private:
	glm::vec3 direction;	
};


class PointLight : public Light {
public:
	PointLight(glm::vec3 _ambient, glm::vec3 _diffuse,
		glm::vec3 _specular, glm::vec3 _position,
		float _constant, float _linear, float _quadratic);

	void UseLight(Shader *shader);
private:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

};

class SpotLight :public Light {
public:
	SpotLight(glm::vec3 _ambient, glm::vec3 _diffuse,
		glm::vec3 _specular, glm::vec3 _position, glm::vec3 spotDirection,
		float _constant, float _linear, float _quadratic, float _spotCutOff,
		float _spotExponent);
	

	void UseLight(Shader *shader);

private:
	glm::vec3 position;		

	float constant;		
	float linear;		
	float quadratic;	

	float spotCutoff, spotExponent;
	glm::vec3 spotDirection;
};