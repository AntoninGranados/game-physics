#include "Scene2.h"
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

void Scene2::onDraw(Renderer &renderer) {
    renderer.drawSphere(x0.position, 0.1f, glm::vec4(0.8,0.4,0,0.9), 1);
    renderer.drawSphere(x1.position, 0.1f, glm::vec4(0.8,0.4,0,0.9), 1);
    renderer.drawLine(x0.position, x1.position, glm::vec3(1,0,0));
}

void Scene2::simulateStep() {
    float distance = glm::distance(x0.position, x1.position);
    glm::vec3 direction = (x1.position - x0.position)/distance;   // from x0 to x1 
    glm::vec3 f1 = SpringForce(s, distance, direction);
    glm::vec3 f0 = -f1;
    glm::vec3 a0 = PFD(x0.mass, f0);
    glm::vec3 a1 = PFD(x1.mass, f1);

    // For Euler, first we use the actual velocity to compute the next position, then we use the actual acceleration to compute the next velocity
    x0.position = x0.position + timeStep*x0.velocity;
    x0.velocity = x0.velocity + timeStep*a0;

    x1.position = x1.position + timeStep*x1.velocity;
    x1.velocity = x1.velocity + timeStep*a1;
}

void Scene2::onGUI(){
    ImGui::SliderFloat("Time Step", &timeStep, 0.f, 0.05f);
}



glm::vec3 Scene2::SpringForce(Spring spring, float l, glm::vec3 direction) {
    return spring.stiffness * (spring.restLength-l) * direction;
}

glm::vec3 Scene2::PFD(float m, glm::vec3 F) {
    return F / m;
}



