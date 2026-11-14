#include "Scene4.h"
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>


void Scene4::onDraw(Renderer &renderer) {
    for(auto &point: massPoints) {
        renderer.drawSphere(point.position, radius, glm::vec4(0.8,0.4,0,1), 1);
    }
    for(auto &spring: springs) {
        renderer.drawLine(massPoints[spring.ind1].position, massPoints[spring.ind2].position, glm::vec3(1,0,0));
    }

    renderer.drawQuad(floor.position, floor.rotationAngles, glm::vec2(scale, scale), floor.color);

}

void Scene4::simulateStep() {
    if(method == 0) {
        //timeStep = 0.005f;  // For UI debugging
        OneStepEuler();
    }

    else if(method==1) {
        //timeStep = 0.1f;
        OneStepMidpoint();
    }

    // Check collisions
    CheckCollisionFloor();
}

void Scene4::onGUI() {
    // Choice for the method
    ImGui::RadioButton("Euler", &method, 0);
    ImGui::RadioButton("Midpoint", &method, 1);

    // TimeStep
    ImGui::SliderFloat("Time step", &timeStep, 0.f, 0.05f);

    /*
    // Don't work here since the characteristics for each objects are defined once when the object is created (in struct)
    // We can use these general characteristics in the formulas instead of the objects' characteristics if we want to change them during the simulation
    ImGui::SliderFloat("Big mass", &bigMass, 0.1f, 50000.f);
    ImGui::SliderFloat("Masses", &mass, 0.001f, 1000.f);
    ImGui::SliderFloat("Stiffness", &stiffness, 0.f, 1000.f);
    ImGui::SliderFloat("Rest Length", &restLength, 0.1f, 50.f);
    */

}

void Scene4::init() {
    // One big mass in the center and every other around and linked to it
    MassPoint x0 = MassPoint{
        bigMass,
        glm::vec3(0),
        glm::vec3(0),
        glm::vec3(0)
    };
    massPoints.push_back(x0);

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


void Scene4::Forces(std::vector<MassPoint> &pointsList) {
    GravityForce(pointsList);
    SpringForces(pointsList);

    // If the mass in the center is fixed (other ways to do to avoid computation)
    pointsList[0].acceleration = glm::vec3(0);
}


void Scene4::SpringForces(std::vector<MassPoint> &pointsList) {
    for(OtherSpring spring: springs) {
        MassPoint point1 = pointsList[spring.ind1];
        MassPoint point2 = pointsList[spring.ind2];
        float distance = glm::distance(point1.position, point2.position);
        glm::vec3 direction = (point2.position - point1.position)/distance;   // from x1 to x2 
    
        glm::vec3 f2 = spring.stiffness * (spring.restLength-distance) * direction;
        glm::vec3 f1 = -f2;
        glm::vec3 a1 = f1/point1.mass;
        glm::vec3 a2 = f2/point2.mass;

        pointsList[spring.ind1].acceleration += a1;
        pointsList[spring.ind2].acceleration += a2;
    }
}


void Scene4::GravityForce(std::vector<MassPoint> &pointsList) {
    glm::vec3 gravityAccel = glm::vec3(0, 0, -9.81f);
    for (auto& point : pointsList){
        point.acceleration += gravityAccel;
    }
}


void Scene4::OneStepEuler() {
    // Computing all forces for every point
    Forces(massPoints);

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

    Forces(pointsList);

    for(auto &point: pointsList) {
        point.position += (timeStep/2)*point.velocity;
        point.velocity += (timeStep/2)*point.acceleration;
        // Acceleration back to 0 for next half step
        point.acceleration = glm::vec3(0);
    }

    // Computing acceleration for the half step
    Forces(pointsList);
    
    // Characteristics after full step
    for(int i=0; i<std::size(massPoints); i++) {
        MassPoint point = pointsList[i];
        massPoints[i].position += timeStep*point.velocity;
        massPoints[i].velocity += timeStep*point.acceleration;
        // Acceleration back to 0 for next step
        massPoints[i].acceleration = glm::vec3(0);
    }
}


void Scene4::CheckCollisionFloor() {
    // Only works for the floor, general planes need another method (with surface normal)
    for(auto &point : massPoints) {
        if(point.position.z - radius <= floorHeight) {
            point.position.z = floorHeight + radius;
        }
    }
}