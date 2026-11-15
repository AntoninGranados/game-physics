#include "Scene2.h"
#include <imgui.h>
#include <glm/gtx/quaternion.hpp>

void Scene2::init() {

    point1.position = glm::vec3(0, 0, 0);
    point1.velocity = glm::vec3(-1, 0, 0);
    point1.mass = 10.0f;
    point1.color = glm::vec4(1, 0, 0, 1);

    point2.position = glm::vec3(0, 2, 0);
    point2.velocity = glm::vec3(1, 0, 0);
    point2.mass = 10.0f;
    point2.color = glm::vec4(0, 0, 1, 1);

    spring.point1 = &point1;
    spring.point2 = &point2;
    spring.restLength = 1.0f;
    spring.stiffness = 40.0f;

    positions1.clear();
    positions2.clear();
    positions1.push_back(point1.position);
    positions2.push_back(point2.position);
}

void Scene2::simulateStep() {

    // compute the force applied by the spring
    glm::vec3 distance = point2.position - point1.position;
    float currentLength = glm::length(distance);
    glm::vec3 force = spring.stiffness * (currentLength - spring.restLength) * distance / currentLength;

    // update the velocity of the points
    point1.velocity += force / point1.mass * deltaTime;
    point2.velocity += - force / point2.mass * deltaTime;

    // update the position of the points
    point1.position += point1.velocity * deltaTime;
    point2.position += point2.velocity * deltaTime;

    positions1.push_back(point1.position);
    positions2.push_back(point2.position);
}

void Scene2::onDraw(Renderer& renderer) {

    // white cube for reference
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    // trail of the points
    for (size_t i = 1; i < positions1.size(); i++) {
        renderer.drawLine(positions1[i - 1], positions1[i], glm::vec4(1, 0, 0, 1));
    }
    for (size_t i = 1; i < positions2.size(); i++) {
        renderer.drawLine(positions2[i - 1], positions2[i], glm::vec4(0, 0, 1, 1));
    }

    // spheres for the points and line for the spring
    renderer.drawSphere(point1.position, 0.1f, point1.color);
    renderer.drawSphere(point2.position, 0.1f, point2.color);
    renderer.drawLine(point1.position, point2.position, glm::vec4(0.5, 0.5, 0.5, 1));
}

void Scene2::onGUI() {
    ImGui::SliderFloat("Delta time", &deltaTime, 0.001f, 0.01f);
}