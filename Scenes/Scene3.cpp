#include "Scene3.h"
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

void Scene3::onDraw(Renderer &renderer) {
    renderer.drawSphere(x0.position, 0.1f, glm::vec4(0.8,0.4,0,0.9), 1);
    renderer.drawSphere(x1.position, 0.1f, glm::vec4(0.8,0.4,0,0.9), 1);
    renderer.drawLine(x0.position, x1.position, glm::vec3(1,0,0));
}

void Scene3::simulateStep() {
    float distance = glm::distance(x0.position, x1.position);
    glm::vec3 direction = (x1.position - x0.position)/distance;   // from x0 to x1 
    
    // Intermediate half step
    glm::vec3 f1 = SpringForce(s, distance, direction);
    glm::vec3 f0 = -f1;
    glm::vec3 a0 = PFD(x0.mass, f0);
    glm::vec3 a1 = PFD(x1.mass, f1);

    glm::vec3 pos0_ = x0.position + (timeStep/2)*x0.velocity;
    glm::vec3 vel0_ = x0.velocity + (timeStep/2)*a0;

    glm::vec3 pos1_ = x1.position + (timeStep/2)*x1.velocity;
    glm::vec3 vel1_ = x1.velocity + (timeStep/2)*a1;

    // Computing acceleration for the half step
    distance = glm::distance(pos0_, pos1_);
    direction = (x1.position - x0.position)/distance;
    f1 = SpringForce(s, distance, direction);
    f0 = -f1;
    a0 = PFD(x0.mass, f0);
    a1 = PFD(x1.mass, f1);

    // Position after full step
    x0.position = x0.position + timeStep*vel0_;
    x1.position = x1.position + timeStep*vel1_;

    // Velocity after full step
    x0.velocity = x0.velocity + timeStep*a0;
    x1.velocity = x1.velocity + timeStep*a1;
}

void Scene3::onGUI(){
    ImGui::SliderFloat("Time Step", &timeStep, 0.f, 0.05f);
}



glm::vec3 Scene3::SpringForce(Spring spring, float l, glm::vec3 direction) {
    return spring.stiffness * (spring.restLength-l) * direction;
}

glm::vec3 Scene3::PFD(float m, glm::vec3 F) {
    return F / m;
}