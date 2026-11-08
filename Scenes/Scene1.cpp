#include "Scene1.h"
#include <glm/gtx/quaternion.hpp>
#include <cmath>


glm::vec3 Scene1::SpringForce(Spring spring, float l, glm::vec3 direction) {
    return spring.stiffness * (spring.restLength-l) * direction;
}

glm::vec3 Scene1::PFD(float m, glm::vec3 F) {
    return F / m;
}

void Scene1::OneStepEuler(MassPoint &point0, MassPoint &point1, Spring spring) {
    float distance = glm::distance(point0.position, point1.position);
    glm::vec3 direction = (point1.position - point0.position)/distance;   // from x0 to x1 
    glm::vec3 f1 = SpringForce(spring, distance, direction);
    glm::vec3 f0 = -f1;
    glm::vec3 a0 = PFD(point0.mass, f0);
    glm::vec3 a1 = PFD(point1.mass, f1);

    // For Euler, first we use the actual velocity to compute the next position, then we use the actual acceleration to compute the next velocity
    point0.position = point0.position + timeStep*point0.velocity;
    point0.velocity = point0.velocity + timeStep*a0;

    point1.position = point1.position + timeStep*point1.velocity;
    point1.velocity = point1.velocity + timeStep*a1;
}

void Scene1::OneStepMidpoint(MassPoint &point0, MassPoint &point1, Spring spring) {
    float distance = glm::distance(point0.position, point1.position);
    glm::vec3 direction = (point1.position - point0.position)/distance;   // from x0 to x1 
    
    // Intermediate half step
    glm::vec3 f1 = SpringForce(spring, distance, direction);
    glm::vec3 f0 = -f1;
    glm::vec3 a0 = PFD(point0.mass, f0);
    glm::vec3 a1 = PFD(point1.mass, f1);

    glm::vec3 pos0_ = point0.position + (timeStep/2)*point0.velocity;
    glm::vec3 vel0_ = point0.velocity + (timeStep/2)*a0;

    glm::vec3 pos1_ = point1.position + (timeStep/2)*point1.velocity;
    glm::vec3 vel1_ = point1.velocity + (timeStep/2)*a1;

    // Computing acceleration for the half step
    distance = glm::distance(pos0_, pos1_);
    direction = (point1.position - point0.position)/distance;
    f1 = SpringForce(spring, distance, direction);
    f0 = -f1;
    a0 = PFD(point0.mass, f0);
    a1 = PFD(point1.mass, f1);

    // Position after full step
    point0.position = point0.position + timeStep*vel0_;
    point1.position = point1.position + timeStep*vel1_;

    // Velocity after full step
    point0.velocity = point0.velocity + timeStep*a0;
    point1.velocity = point1.velocity + timeStep*a1;
}

void Scene1::init() {
    std::cout << "Initial Values:" << std::endl;
    std::cout << "Masspoint 0" << std::endl;
    std::cout << "position: (" << x0.position.x << ", " << x0.position.y << ", " << x0.position.z << ")" << std::endl;
    std::cout << "velocity: (" << x0.velocity.x << ", " << x0.velocity.y << ", " << x0.velocity.z << ")" << std::endl;
    std::cout << "Masspoint 1" << std::endl;
    std::cout << "position: (" << x1.position.x << ", " << x1.position.y << ", " << x1.position.z << ")" << std::endl;
    std::cout << "velocity: (" << x1.velocity.x << ", " << x1.velocity.y << ", " << x1.velocity.z << ")" << std::endl;

    OneStepMidpoint(x0, x1, s);
    
    std::cout << "After " << timeStep << "s using Midpoint Method:" << std::endl;
    std::cout << "Masspoint 0" << std::endl;
    std::cout << "position: (" << x0.position.x << ", " << x0.position.y << ", " << x0.position.z << ")" << std::endl;
    std::cout << "velocity: (" << x0.velocity.x << ", " << x0.velocity.y << ", " << x0.velocity.z << ")" << std::endl;
    std::cout << "Masspoint 1" << std::endl;
    std::cout << "position: (" << x1.position.x << ", " << x1.position.y << ", " << x1.position.z << ")" << std::endl;
    std::cout << "velocity: (" << x1.velocity.x << ", " << x1.velocity.y << ", " << x1.velocity.z << ")" << std::endl;

    // Initial values
    x0 = MassPoint{
        10.f,
        glm::vec3(0),
        glm::vec3(-1,0,0)
    };
    x1 = MassPoint{
        10.f,
        glm::vec3(0,2,0),
        glm::vec3(1,0,0)
    };

    OneStepEuler(x0, x1, s);

    std::cout << "After " << timeStep << "s using Euler Method:" << std::endl;
    std::cout << "Masspoint 0" << std::endl;
    std::cout << "position: (" << x0.position.x << ", " << x0.position.y << ", " << x0.position.z << ")" << std::endl;
    std::cout << "velocity: (" << x0.velocity.x << ", " << x0.velocity.y << ", " << x0.velocity.z << ")" << std::endl;
    std::cout << "Masspoint 1" << std::endl;
    std::cout << "position: (" << x1.position.x << ", " << x1.position.y << ", " << x1.position.z << ")" << std::endl;
    std::cout << "velocity: (" << x1.velocity.x << ", " << x1.velocity.y << ", " << x1.velocity.z << ")" << std::endl;
}