#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <vector>



#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

class Object3D
{
public:
    Object3D();
    ~Object3D();

    bool loadOBJ(const char* path);
    void printInfo(int numLines = -1); // Default to -1, imprime todas as linhas

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

private:
};