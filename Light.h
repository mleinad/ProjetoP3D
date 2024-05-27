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

private:
	glm::vec3 color;
	float ambientIntesity;

	glm::vec3 direction;
	float diffuseIntesity;

};


class AmbientLight: Light {

public:
	AmbientLight();

	void UseLight(Shader shader);
private:
	
	glm::vec3 ambient;
};


class DirectionalLight : Light {
public:
	DirectionalLight();


	void UseLight(Shader shader);
private:
	glm::vec3 direction;	

	glm::vec3 ambient;		
	glm::vec3 diffuse;		
	glm::vec3 specular;
};


class PointLight :Light {
public:
	PointLight();

	void UseLight(Shader shader);
private:
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

};

class SpotLight :Light {
public:
	SpotLight();
	

	void UseLight(Shader shader);
private:
	glm::vec3 position;		

	glm::vec3 ambient;		
	glm::vec3 diffuse;		
	glm::vec3 specular;		

	float constant;		
	float linear;		
	float quadratic;	

	float spotCutoff, spotExponent;
	glm::vec3 spotDirection;
};