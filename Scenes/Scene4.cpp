#include "Scene4.h"
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>


void Scene4::onDraw(Renderer &renderer) {
    for(auto &point: massPoints) {
        renderer.drawSphere(point.position, 0.1f, glm::vec4(0.8,0.4,0,0.9), 1);
    }
    for(auto &spring: springs) {
        renderer.drawLine(massPoints[spring.ind1].position, massPoints[spring.ind2].position, glm::vec3(1,0,0));
    }
}

void Scene4::simulateStep() {
    if(method == 0) {OneStepEuler();}

    else if(method==1) {OneStepMidpoint();}
}

void Scene4::onGUI() {

}

void Scene4::init() {
    // One big mass in the center and every other around and linked to it
    MassPoint x0 = MassPoint{
        10000.f,
        glm::vec3(0),
        glm::vec3(0),
        glm::vec3(0)
    };
    massPoints.push_back(x0);

    float mass = 10.f;
    float stiffness = 10.f;
    float restLength = 1.f;

    for(int i=1; i<10; i++) {
        glm::vec3 position = glm::vec3((dis(gen)-0.5)*4.f, (dis(gen)-0.5)*4.f, (dis(gen)-0.5)*4.f);
        MassPoint x = MassPoint{
            mass,
            position,
            glm::vec3(0),
            glm::vec3(0)
        };
        massPoints.push_back(x);
        springs.push_back(OtherSpring{
            stiffness,
            restLength,
            0,
            i
        });
    }
}

void Scene4::SpringForces(std::vector<MassPoint> &pointsList) {
    for(OtherSpring spring: springs) {
        MassPoint point1 = pointsList[spring.ind1];
        MassPoint point2 = pointsList[spring.ind2];
        float distance = glm::distance(point1.position, point2.position);
        glm::vec3 direction = (point2.position - point1.position)/distance;   // from x1 to x2 
    
        glm::vec3 f2 = spring.stiffness * (spring.restLength-distance) * direction;
        glm::vec3 f1 = -f2;
        glm::vec3 a1 = PFD(point1.mass, f1);
        glm::vec3 a2 = PFD(point2.mass, f2);

        pointsList[spring.ind1].acceleration += a1;
        pointsList[spring.ind2].acceleration += a2;
    }
}

glm::vec3 Scene4::PFD(float m, glm::vec3 F) {
    return F / m;
}

void Scene4::OneStepEuler() {
    // Computing all forces for every point
    SpringForces(massPoints);

    for(auto &point: massPoints) {
        point.position += timeStep*point.velocity;
        point.velocity += timeStep*point.acceleration;
        // Acceleration back to 0 for next step
        point.acceleration = glm::vec3(0);
    }
}

void Scene4::OneStepMidpoint() {
    // Intermediate half step
    
    // Intermediate list for half step
    std::vector<MassPoint> pointsList = massPoints;

    SpringForces(pointsList);

    for(auto &point: pointsList) {
        point.position += (timeStep/2)*point.velocity;
        point.velocity += (timeStep/2)*point.acceleration;
        // Acceleration back to 0 for next half step
        point.acceleration = glm::vec3(0);
    }

    // Computing acceleration for the half step
    SpringForces(pointsList);
    
    // Caracteristics after full step
    for(int i=0; i<std::size(massPoints); i++) {
        MassPoint point = pointsList[i];
        massPoints[i].position += timeStep*point.velocity;
        massPoints[i].velocity += timeStep*point.acceleration;
        // Acceleration back to 0 for next step
        massPoints[i].acceleration = glm::vec3(0);
    }
}