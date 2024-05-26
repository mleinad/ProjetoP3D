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
#include <algorithm>
#include "Luis/Renderer.h"
#include "Luis/VertexArray.h"
#include "Luis/VertexBuffer.h"
#include "Luis/VertexBufferLayout.h"
#include "Luis/Shader.h"


struct Material
{
    glm::vec3 ambient;
    glm::vec3 difuse;
    glm::vec3 specular;
    float shininess;
    std::string texture;
    std::string mtl_path;

    void loadTexture() {
        //loadtexture
    }
};


class Object3D
{
public:

    Object3D();
    Object3D(const char* path);
    ~Object3D();

    bool loadOBJ(const char* path);
   
    
    bool loadMTL(const char* path);
    void printInfo(int numLines = -1); // Default to -1, imprime todas as linhas

    //output 
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::vector<glm::vec3> mesh;

    
    Material material;


private:

};


