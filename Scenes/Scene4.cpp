#include "Scene4.h"
#include <imgui.h>
#include <glm/gtx/quaternion.hpp>

void Scene4::init() {

    pointsEuler.clear();
    springsEuler.clear();
    pointsMidpoint.clear();
    springsMidpoint.clear();

    collider.position = glm::vec3(0, -1, 0);
    collider.velocity = glm::vec3(0);
    collider.acceleration = glm::vec3(0);
    collider.radius = 1.0f;
    collider.isFixed = true;

    // create every point in a grid pattern with the top row fixed in place
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Point4 pointEuler;
            pointEuler.position = glm::vec3((width / 2 - x) * restLength, 0, (height / 2 - y) * restLength);
            pointEuler.velocity = glm::vec3(0);
            pointEuler.acceleration = glm::vec3(0);
            pointEuler.radius = 0.05f;
            pointEuler.isFixed = (y == 0);
            pointsEuler.push_back(pointEuler);
            Point4 pointMidpoint;
            pointMidpoint.position = glm::vec3((width / 2 - x) * restLength, 0, (height / 2 - y) * restLength);
            pointMidpoint.velocity = glm::vec3(0);
            pointMidpoint.acceleration = glm::vec3(0);
            pointMidpoint.radius = 0.05f;
            pointMidpoint.isFixed = (y == 0);
            pointsMidpoint.push_back(pointMidpoint);
        }
    }

    // create springs linking each point to its neighbors
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            if (x < width - 1) {
                Spring4 springEuler;
                springEuler.point1 = &pointsEuler[index];
                springEuler.point2 = &pointsEuler[index + 1];
                springsEuler.push_back(springEuler);
                Spring4 springMidpoint;
                springMidpoint.point1 = &pointsMidpoint[index];
                springMidpoint.point2 = &pointsMidpoint[index + 1];
                springsMidpoint.push_back(springMidpoint);
            }
            if (y < height - 1) {
                Spring4 springEuler;
                springEuler.point1 = &pointsEuler[index];
                springEuler.point2 = &pointsEuler[index + width];
                springsEuler.push_back(springEuler);
                Spring4 springMidpoint;
                springMidpoint.point1 = &pointsMidpoint[index];
                springMidpoint.point2 = &pointsMidpoint[index + width];
                springsMidpoint.push_back(springMidpoint);
            }
        }
    }
}

void Scene4::oneStepEuler() {

    for (Point4& point : pointsEuler) {
        point.acceleration = glm::vec3(0);
    }

    // compute the force applied by the spring to each point linked to this spring
    for (Spring4& spring : springsEuler) {

        // compute the force applied by the spring
        float currentLength = glm::length(spring.point2->position - spring.point1->position);
        glm::vec3 direction = glm::normalize(spring.point2->position - spring.point1->position);
        glm::vec3 force = stiffness * (currentLength - restLength) * direction;

        // update the acceleration of the points
        spring.point1->acceleration += force / mass;
        spring.point2->acceleration += - force / mass;
    }

    // compute the effect of gravity and damping on each point and update their position
    for (Point4& point : pointsEuler) {
        point.acceleration += glm::vec3(0, 0, gravity) - damping * point.velocity / mass;
        point.velocity += point.acceleration * deltaTime;
        if (point.isFixed) {
            point.velocity = glm::vec3(0);
        }
        point.position += point.velocity * deltaTime;
    }

    // detect collisions with collider
    for (Point4& point: pointsEuler) {
        float distance = glm::length(point.position - collider.position);
        if (distance < point.radius + collider.radius) {
            glm::vec3 direction = glm::normalize(point.position - collider.position);
            float overlap = point.radius + collider.radius - distance;
            point.position += direction * overlap;
        }
    }
}

void Scene4::oneStepMidpoint() {

    for (Point4& point : pointsMidpoint) {
        point.acceleration = glm::vec3(0);
    }

    for (Spring4& spring : springsMidpoint) {

        float currentLength = glm::length(spring.point2->position - spring.point1->position);
        glm::vec3 direction = glm::normalize(spring.point2->position - spring.point1->position);
        glm::vec3 force = stiffness * (currentLength - restLength) * direction;

        // compute acceleration at t
        spring.point1->acceleration += force / mass;
        spring.point2->acceleration += - force / mass;
    }

    for (Point4& point : pointsMidpoint) {

        // compute position_tmp at t + deltaTime / 2
        point.position_tmp = point.position + point.velocity * deltaTime / 2.0f;

        // compute acceleration at t
        point.acceleration += glm::vec3(0, 0, gravity) - damping * point.velocity / mass;

        // compute velocity_tmp at t + deltaTime / 2
        point.velocity_tmp = point.velocity + point.acceleration * deltaTime / 2.0f;
        if (point.isFixed) {
            point.velocity_tmp = glm::vec3(0);
        }
        point.position += point.velocity_tmp * deltaTime;
    }

    // reset acceleration
    for (Point4& point : pointsMidpoint) {
        point.acceleration = glm::vec3(0);
    }

    // compute the new forces with position_tmp and velocity_tmp
    for (Spring4& spring : springsMidpoint) {

        float currentLength = glm::length(spring.point2->position_tmp - spring.point1->position_tmp);
        glm::vec3 direction = glm::normalize(spring.point2->position_tmp - spring.point1->position_tmp);
        glm::vec3 force = stiffness * (currentLength - restLength) * direction;

        // compute acceleration at t + deltaTime / 2
        spring.point1->acceleration += force / mass;
        spring.point2->acceleration += - force / mass;
    }

    for (Point4& point : pointsMidpoint) {
        point.acceleration += glm::vec3(0, 0, gravity) - damping * point.velocity_tmp / mass;
        point.velocity += point.acceleration * deltaTime;
    }

    // detect collisions with collider
    for (Point4& point: pointsMidpoint) {
        float distance = glm::length(point.position - collider.position);
        if (distance < point.radius + collider.radius) {
            glm::vec3 direction = glm::normalize(point.position - collider.position);
            float overlap = point.radius + collider.radius - distance;
            point.position += direction * overlap;
        }
    }
}

void Scene4::simulateStep() {

    oneStepEuler();
    oneStepMidpoint();

    if (ImGui::IsKeyDown(ImGuiKey_W))
        collider.position.x += 0.01f;
    if (ImGui::IsKeyDown(ImGuiKey_S))
        collider.position.x -= 0.01f;
    if (ImGui::IsKeyDown(ImGuiKey_A))
        collider.position.y += 0.01f;
    if (ImGui::IsKeyDown(ImGuiKey_D))
        collider.position.y -= 0.01f;
    if (ImGui::IsKeyDown(ImGuiKey_Q))
        collider.position.z += 0.01f;
    if (ImGui::IsKeyDown(ImGuiKey_E))
        collider.position.z -= 0.01f;
}

void Scene4::onDraw(Renderer& renderer) {

    // colors
    glm::vec4 eulerColor(1, 0, 0, useMidpoint? 0.5f : 1.0f);
    glm::vec4 midpointColor(0, 1, 0, useMidpoint? 1.0f : 0.5f);

    // draw the Euler simulation
    for (Spring4& spring : springsEuler) {
        renderer.drawLine(spring.point1->position, spring.point2->position, eulerColor);
    }
    for (Point4& point : pointsEuler) {
        renderer.drawSphere(point.position, point.radius, eulerColor);
    }

    // draw the midpoint simulation
    for (Spring4& spring : springsMidpoint) {
        renderer.drawLine(spring.point1->position, spring.point2->position, midpointColor);
    }
    for (Point4& point : pointsMidpoint) {
        renderer.drawSphere(point.position, point.radius, midpointColor);
    }

    // draw the collider
    renderer.drawSphere(collider.position, collider.radius, glm::vec4(0, 0, 1, 1));
}

void Scene4::onGUI() {

    ImGui::Text("Cloth parameters");
    ImGui::SliderInt("Width", &width, 2, 50);
    ImGui::SliderInt("Height", &height, 2, 50);

    ImGui::Text("Spring parameters");
    ImGui::SliderFloat("Stiffness", &stiffness, 100.0f, 10000.0f);
    ImGui::SliderFloat("Rest length", &restLength, 0.1f, 1.0f);

    ImGui::Text("Point parameters");
    ImGui::SliderFloat("Mass", &mass, 0.1f, 10.0f);

    ImGui::Text("Simulation parameters");
    ImGui::SliderFloat("Delta time", &deltaTime, 0.001f, 0.01f);
    ImGui::SliderFloat("Damping", &damping, 0.0f, 10.0f);

    auto initialize = ImGui::Button("Initialize");
    auto switchMethod = ImGui::Button("Switch between Euler and Midpoint");
    if (initialize)
        init();
    if (switchMethod)
        useMidpoint = !useMidpoint;
}