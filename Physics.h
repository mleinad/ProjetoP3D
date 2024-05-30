#pragma once
#include <glm/glm.hpp>
#include <vector>
class Physics
{

    
    struct Ball {
        glm::vec3 v; //Velocity
        glm::vec3 pos; //Position
        float r; //Radius
        glm::vec3 color;
    };


public:


    bool CheckCollisions(glm::mat4 CurrentModelView, std::vector<glm::mat4>ModelViewVector);

private:
    bool CheckCollisionIndevidual(glm::mat4 MV1, glm::mat4 MV2);

    glm::mat4 ModelView;
};

