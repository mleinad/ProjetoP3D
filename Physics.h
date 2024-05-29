#pragma once
#include <glm/glm.hpp>
class Physics
{

    
    struct Ball {
        glm::vec3 v; //Velocity
        glm::vec3 pos; //Position
        float r; //Radius
        glm::vec3 color;
    };


public:

    bool CheckCollision(Ball ball1, Ball ball2);
};

