#include "Physics.h"
#include "glm/glm.hpp"
#include <iostream>



enum Wall { WALL_LEFT, WALL_RIGHT, WALL_FAR, WALL_NEAR, WALL_TOP, WALL_BOTTOM };

struct Ball {
    glm::vec3 v; //Velocity
    glm::vec3 pos; //Position
    float r; //Radius
    glm::vec3 color;
};


//Stores a ball and a wall
struct BallWallPair {
    Ball* ball;
    Wall wall;
};

bool Physics::CheckCollisionIndevidual(glm::mat4 MV1, glm::mat4 MV2)
{
    glm::vec3 center1 = glm::vec3(MV1[3]);
    glm::vec3 center2 = glm::vec3(MV2[3]);

    float distance = glm::distance(center1, center2);

    return distance <= 2.0f;
}

bool Physics::CheckCollisions(glm::mat4 CurrentModelView, std::vector<glm::mat4>ModelViewVector) {

    for (const auto& modelView : ModelViewVector) {
        if (modelView == CurrentModelView) {
            continue;
        }
        if (CheckCollisionIndevidual(CurrentModelView, modelView)) {
            // Handle collision
            std::cout << "Collision detected!" << std::endl;
            return true; 
        }
    }
    return false;
}
