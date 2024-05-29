#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include <glm/glm.hpp>

#define DM_MESH = sizeof(Mesh)

struct Mesh
{
	glm::vec3 vertices;
	glm::vec3 normals;
	glm::vec2 uvs;
};
