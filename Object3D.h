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
#include "Mesh.h"


struct Material
{
    glm::vec3 ambient;
    glm::vec3 difuse;
    glm::vec3 specular;
    float shininess;
    std::string texture;
    std::string mtl_path;

    void loadTexture() {

        //add path

    }


};


class Object3D
{
public:

    Object3D();

    Object3D(const char* path);
    ~Object3D();

    Object3D(const char* path, bool _mtl);

    
    void printInfo(int numLines = -1); // Default to -1, imprime todas as linhas

    //nao utilizado 
    std::vector<Mesh> meshStruct;
    int VertexCount;
    std::vector<glm::vec3>meshVector;
    Material material;

private:

    int getVertexCount();
    bool loadOBJ(const char* path);
    bool loadMTL(const char* path);
    bool loadDefaultMTL();
};


