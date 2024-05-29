#include "Physics.h"
#include "glm/glm.hpp"



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

bool Physics::CheckCollision(Ball ball1, Ball ball2)
{
    float distanceSquared = glm::distance(ball1.pos, ball2.pos);

    float sumOfRadSquared = (ball1.r + ball2.r) * (ball1.r + ball2.r);

    return distanceSquared <= sumOfRadSquared;
}
