#include "Scene3.h"
#include <imgui.h>
#include <glm/gtx/quaternion.hpp>

void Scene3::init() {

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

void Scene3::simulateStep() {

    // compute x_tmp at t + deltaTime / 2
    glm::vec3 x_tmp1 = point1.position + point1.velocity * deltaTime / 2.0f;
    glm::vec3 x_tmp2 = point2.position + point2.velocity * deltaTime / 2.0f;

    // compute the force applied by the spring
    glm::vec3 distance = point2.position - point1.position;
    float currentLength = glm::length(distance);
    glm::vec3 force = spring.stiffness * (currentLength - spring.restLength) * distance / currentLength;

    // compute v_tmp at t + deltaTime / 2
    glm::vec3 v_tmp1 = point1.velocity + force / point1.mass * deltaTime / 2.0f;
    glm::vec3 v_tmp2 = point2.velocity - force / point2.mass * deltaTime / 2.0f;

    // compute the new forces with x_tmp and v_tmp
    distance = x_tmp2 - x_tmp1;
    currentLength = glm::length(distance);
    force = spring.stiffness * (currentLength - spring.restLength) * distance / currentLength;

    // compute the new positions
    point1.position += v_tmp1 * deltaTime;
    point2.position += v_tmp2 * deltaTime;

    // compute the new velocities
    point1.velocity += force / point1.mass * deltaTime;
    point2.velocity += - force / point2.mass * deltaTime;

    positions1.push_back(point1.position);
    positions2.push_back(point2.position);
}

void Scene3::onDraw(Renderer& renderer) {

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

void Scene3::onGUI() {
    ImGui::SliderFloat("Delta time", &deltaTime, 0.001f, 0.01f);
}